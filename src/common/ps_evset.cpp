//
// Created by rf on 1/20/22.
//
#include "evsets/ps_evset.hpp"
#include "utils.hpp"
#include <cstdio>
#include <l3.hpp>
#include <stdexcept>

using namespace cache;

linked_list set_create(linked_list &guessPool, uintptr_t victim) {

  linked_list set = {};

  size_t length = l3::assoc();

extend:
  while (set.length < length) {

	intrinsics::maccess::normal(victim);
	asm volatile("lfence;mfence");

	traverse_zigzag_victim(set.start, victim);
	traverse_zigzag_victim(set.start, victim);

	set_add(set, guessPool, victim);
  }

  if(!set_valid(set, victim)) {
	if (++length < 48) [[likely]] {
	  goto extend;
	} else
	  throw std::runtime_error("Failed to genearate");
  }
  set_reduce(set, guessPool, victim);

  return set;
}
bool set_add(linked_list &set, linked_list &guessPool, uintptr_t victim) {

  uint32_t attempt = 0;
  while (attempt < MAX_RETRIES) {
	intrinsics::maccess::normal(guessPool.head);
	intrinsics::lfence();
	intrinsics::mfence();
	uint64_t time = intrinsics::memaccesstime::normal(victim);

	if (time > THRESHOLD - 20 && time < 2*THRESHOLD) [[unlikely]] {
	  list_push_back(&set, list_pop_back(&guessPool));
	  return true;
	}
	else
	{
	  list_push_front(&guessPool, list_pop_back(&guessPool));
	}
	attempt++;
  }
  return false;
}

bool set_reduce(linked_list &set, linked_list &guessPool, uintptr_t victim) {

  node *intial = set.start;

  size_t length = set.length;
  size_t intialLength = set.length;

  node* ptr = nullptr;
  for(int i = 0; i < set.length; i++){
	ptr = list_pop_front(&set);

	if(set_valid(set, victim)) {
	  list_push_front(&guessPool, ptr);
//	  if(set.length == l3::assoc())
//		return true;
	}
	  else {
	  list_push_back(&set, ptr);
	}
  }
//  do{
//	node* elem = list_pop_front(&set);
//
//	if(set_valid(set, victim)) [[likely]] // if it is still a valid set, add it back to the guess pool, else
//	  list_push_front(&guessPool, elem);
//	else
//	  list_push_back(&set, elem);
//
//	if(--length == 0)
//	  return false;
//  } while (set.length > l3::assoc());

//  printf("Reduced set size from %d to %d\n", intialLength, set.length);

  return false;
}




bool set_valid(linked_list &set , uintptr_t victim) {

//  if (set.length < l3::assoc()) [[unlikely]]
//  	throw std::runtime_error("Should not occur");
//

  //Loads Victim into LLC

  intrinsics::maccess::double_fenced(victim);

  uint64_t time = 0;
  for (int i = 0; i < 10; i++) {

	node* ptr = set.start;
	traverse_list_skylake(ptr);
	traverse_list_skylake(ptr);
	traverse_list_skylake(ptr);
	traverse_list_skylake(ptr);
	asm volatile ("lfence; mfence");
//	intrinsics::lfence();
//	intrinsics::mfence();
	time+= intrinsics::memaccesstime::normal(victim);
  }

//  if (time/set.length > THRESHOLD) [[unlikely]] {
  if (time/10 > THRESHOLD) [[unlikely]] {
//	printf("succ w/ size of %zu\n", set.length);
	return true;
  }

  return false;
}
linked_list brute_force_set(linked_list &guessPool, uintptr_t victim) {

  linked_list set{};

  uintptr_t physicalAddress = utils::get_physical_address(victim);
  uint64_t cacheSet = cache::l3::get_physical_cache_set(physicalAddress);
  uint64_t slice = cache::l3::get_physical_slice(physicalAddress);

  uint64_t len = l3::assoc();
  do{
  while(set.length != len){

	node* tmp = list_pop_back(&guessPool);
	uintptr_t physicalAddress2 = utils::get_physical_address((uintptr_t)tmp);
	uint64_t cacheSet2 = cache::l3::get_physical_cache_set(physicalAddress2);
	uint64_t slice2 = cache::l3::get_physical_slice(physicalAddress2);

	if(cacheSet2 == cacheSet && slice == slice2) [[unlikely]]
	  list_push_back(&set, tmp);
	else
		list_push_front(&guessPool, tmp);
  }
//  len++;
  } while(!set_valid(set, victim));
//  set_reduce(set, guessPool, victim);
 return set;
}

