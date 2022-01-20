//

// Created by rf on 1/17/22.
//

#include "evsets/eviction_set_l3.hpp"
#include "l3.hpp"
#include "intrinsics.hpp"
#include <random>
#include <vector>
#include <iostream>

using namespace cache;

bool eviction_set_l3::set_valid(linked_list &evset, uintptr_t victim) {

  if (evset.length < l3::assoc())
	return false;

  uint64_t time = 0;

  //Loads Victim into LLC
  intrinsics::maccess::double_fenced(victim);

  for (int i = 0; i < evset.length; i++) {

	//Repeated 4 times in paper code
	traverse_list_skylake(evset.start);
	traverse_list_skylake(evset.start);
	traverse_list_skylake(evset.start);
	traverse_list_skylake(evset.start);
	intrinsics::lfence();
	intrinsics::mfence();

	time += intrinsics::memaccesstime::normal(victim);
  }
  bool ret = time/evset.length > THRESHOLD;

  if (ret) [[unlikely]] {
	printf("Test succ w/ size of %zu\n", evset.length);
  }
  return time/evset.length > THRESHOLD;
}

linked_list eviction_set_l3::set_create(uintptr_t bufferStart, size_t bufferSize, uintptr_t victim) {

  linked_list guessPool = {
	  .start = nullptr,
	  .head = nullptr,
	  .length = 0
  };
//  guessPool.reserve(bufferSize/l3::get_page_size());

  //As buffer start is aligned to the start of a page, fill up a guess pool with
//  for (size_t i = 0; i < bufferSize/sizeof(uintptr_t); i++)
//	guessPool.push_back((node *)(bufferStart + i*sizeof(uintptr_t)));

  for (size_t i = 0; i < bufferSize/l3::get_page_size() - l3::get_page_bits()*2; i++)
	list_push_back(&guessPool, (node *)(bufferStart + i*l3::get_page_size()));

  //Shuffle the elements to prevent cache lines being predicted and loaded
  //Might be better to just check for uniqueness and consitnelty generate a random number each time
  //TODO go back to the previous method where it did not require this to be done at runtime
// std::shuffle(guessPool.begin(), guessPool.end(), std::mt19937(std::random_device()()));

  size_t len = l3::assoc();

  linked_list set = {
	  .start = nullptr,
	  .head = nullptr,
	  .length = 0
  };

  uint64_t attempts = 0;
  while (attempts < 256) {
	while (set.length < len) {
	  traverse_zigzag_victim(set.start, victim);
	  traverse_zigzag_victim(set.start, victim);
//	  if (guessPool.length==0) [[unlikely]]
//		break;
	  set_add(set, guessPool, victim);
	}

	if (!set_valid(set, victim)) {
	  if (len < l3::assoc()*2)
		[[likely]]
			len++;
	  else
		set_reduce(set, guessPool, victim);
	} else
	  break;
	attempts++;
  }

  //set_valid(set,victim);
  intrinsics::maccess::double_fenced(victim);
  set_reduce(set, guessPool, victim);
  intrinsics::maccess::double_fenced(victim);
  std::cout << "Did it work: " << set_valid(set, victim) << std::endl;

  return set;
}

void eviction_set_l3::set_add(linked_list &set, linked_list &guessPool, uintptr_t victim) {

  for (uint32_t attempt = 0; attempt < MAX_RETRIES; attempt++) {

	//put in cache
	intrinsics::maccess::normal(guessPool.head);
	intrinsics::lfence();
	intrinsics::mfence();
	uint64_t time = intrinsics::memaccesstime::normal(victim);

	if (time > THRESHOLD - 20 && time < 2*THRESHOLD) [[unlikely]] {
	  list_push_back(&set, list_pop_back(&guessPool));
	}
	if (guessPool.length==0) [[unlikely]] {
	  throw std::runtime_error("Should not be possible");
	}
  }
}

void eviction_set_l3::set_reduce(linked_list &set, linked_list &guesspool, uintptr_t victim) {

  const static size_t IDEAL_SIZE = l3::assoc();
  const static size_t MIN_SIZE = l3::assoc();
  size_t intialSize = set.length;

  traverse_zigzag_victim(set.start, victim);
  traverse_zigzag_victim(set.start, victim);
  if (!set_valid(set, victim))
	return;

  node *start = set.start;
  while (set.length > IDEAL_SIZE && start!=set.start) {
	node *tmp = list_pop_front(&set);
//	  if (!set_valid(set, victim) && bIntiallyValid) [[likely]]
	if (set_valid(set, victim))
	  [[likely]]
		  list_push_front(&guesspool, tmp); //Re add the address which is not valid to the guess pool
	else
	  list_push_back(&set, tmp);
  }

  printf("Reduced list to size of %zu from %zu\n", set.length, intialSize);
}
