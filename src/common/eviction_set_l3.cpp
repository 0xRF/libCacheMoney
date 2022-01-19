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

  uint64_t time = 0;
  uint64_t length = 0;

  //Loads Victim into LLC
  intrinsics::maccess::double_fenced(victim);
  intrinsics::maccess::double_fenced(victim);

  node *ptr = evset.start;

  while (ptr) {

	//Repeated 4 times in paper code
	traverse_list_skylake(evset.start);
	traverse_list_skylake(evset.start);
	traverse_list_skylake(evset.start);
	traverse_list_skylake(evset.start);
	intrinsics::lfence();
	intrinsics::mfence();

	time += intrinsics::memaccesstime::normal(victim);
	length++;

	ptr = ptr->next;
  }

  return time/length > THRESHOLD;
}

node *eviction_set_l3::set_create(uintptr_t bufferStart, size_t bufferSize, uintptr_t victim) {

  std::vector<node *> guessPool;
//  guessPool.reserve(bufferSize/l3::get_page_size());

  //As buffer start is aligned to the start of a page, fill up a guess pool with
  for (size_t i = 0; i < bufferSize/sizeof(uintptr_t); i++)
	guessPool.push_back((node *)(bufferStart + i*sizeof(uintptr_t)));

  std::cout << guessPool.size() << std::endl;

  //Shuffle the elements to prevent cache lines being predicted and loaded
  // Might be better to just check for uniqueness and consitnelty generate a random number each time
  //TODO go back to the previous method where it did not require this to be done at runtime
  std::shuffle(guessPool.begin(), guessPool.end(), std::mt19937(std::random_device()()));

  size_t len = l3::assoc();

  linked_list set = {
	  .start = nullptr,
	  .head = nullptr,
	  .length = 0
  };

  uint64_t attempts = 0;
  while (attempts < MAX_RETRIES) {
	while (set.length < len) {
	  traverse_zigzag_victim(set.start, victim);
	  traverse_zigzag_victim(set.start, victim);
	  set_add(set, guessPool, victim);
	}

	if (!set_valid(set, victim)) {
	  if (len < 128) [[likely]]
			len++;
	}
	else
	  break;
  }

//  traverse_zigzag_victim(set.start, victim);
//  do {
//
//  } while (!set_valid(set, victim) && !set_valid(set, victim));

  std::cout << "Set Size " << set.length << std::endl;

  std::cout << "Did it work: " << set_valid(set, victim) << std::endl;
  intrinsics::maccess::double_fenced(victim);
  set_reduce(set, victim);
  intrinsics::maccess::double_fenced(victim);
  std::cout << "Did it work: " << set_valid(set, victim) << std::endl;

  return nullptr;
}

void eviction_set_l3::set_add(linked_list &set, std::vector<node *> &guessPool, uintptr_t victim) {

  for (uint32_t attempt = 0; attempt < MAX_RETRIES; attempt++) {
	//put in cache
	intrinsics::maccess::normal(guessPool.back());
	intrinsics::lfence();
	intrinsics::mfence();
	uint64_t time = intrinsics::memaccesstime::normal(victim);

	if (time > THRESHOLD - 20 && time < 2*THRESHOLD) [[unlikely]] {

	  if (set.length!=0)
		[[likely]]
			list_push_back(&set, guessPool.back());
	  else
		[[unlikely]]
			list_init(&set, guessPool.back());
	}
	guessPool.pop_back();

	if (guessPool.empty())
	  return;
  }

}

///
/// \param set
/// \param victim
/// \return false if the reduction
void eviction_set_l3::set_reduce(linked_list &set, uintptr_t victim) {

  const static size_t IDEAL_SIZE = l3::assoc();
  const static size_t MIN_SIZE = l3::assoc();

  traverse_zigzag_victim(set.start, victim);
  traverse_zigzag_victim(set.start, victim);
  bool bIntiallyvalid = set_valid(set,victim);

  node *start = set.start;
  while (set.length > IDEAL_SIZE && start!=set.start) {
	node *tmp = list_pop_front(&set);
	if (!set_valid(set, victim) && bIntiallyvalid)
	  list_push_back(&set, tmp);
  }

  printf("Reduced list to size of %zu\n", set.length);
}
