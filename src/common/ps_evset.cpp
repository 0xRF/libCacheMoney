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
	intrinsics::lfence();
	intrinsics::mfence();

	traverse_zigzag_victim(set.start, victim);
	traverse_zigzag_victim(set.start, victim);

	set_add(set, guessPool, victim);
  }

  intrinsics::maccess::double_fenced(victim);
  intrinsics::maccess::double_fenced(victim);

  if(!set_valid(set, victim)) {
	if(++length < l3::assoc()*2) {
//	  length++;
	  goto extend;
	}
	else
	{
	}
  }

  intrinsics::maccess::double_fenced(victim);
  intrinsics::maccess::double_fenced(victim);
  set_reduce(set, guessPool, victim);
//
  intrinsics::maccess::double_fenced(victim);
  intrinsics::maccess::double_fenced(victim);
  if(!set_valid(set, victim))
	goto extend;

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
	attempt++;
  }
  return false;
}

bool set_reduce(linked_list &set, linked_list &guessPool, uintptr_t victim) {

  if(set.length < l3::assoc()) [[unlikely]]
	throw std::runtime_error("Impossible scenario");

  node *intial = set.start;
  if(!set_valid(set, victim))
	return false;

  size_t length = set.length;
  size_t intialLength = set.length;
  do{
	node* elem = list_pop_front(&set);

	if(set_valid(set, victim)) [[likely]] // if it is still a valid set, add it back to the guess pool, else
	  list_push_front(&guessPool, elem);
	else
	  list_push_back(&set, elem);

	length--;
	if(--length == 0)
	  return false;
  } while (set.length > l3::assoc());

  printf("Reduced set size from %d to %d\n", intialLength, set.length);

  return true;
}




bool set_valid(linked_list &set , uintptr_t victim) {

  if (set.length < l3::assoc()) [[unlikely]]
  	throw std::runtime_error("Should not occur");


  //Loads Victim into LLC
  intrinsics::maccess::double_fenced(victim);

  uint64_t time = 0;
  for (int i = 0; i < 10; i++) {
	traverse_list_skylake(set.start);
	traverse_list_skylake(set.start);
	traverse_list_skylake(set.start);
	traverse_list_skylake(set.start);
	intrinsics::lfence();
	intrinsics::mfence();
	time+= intrinsics::memaccesstime::normal(victim);
  }

//  if (time/set.length > THRESHOLD) [[unlikely]] {
  if (time / 10 > THRESHOLD) [[unlikely]] {
	printf("succ w/ size of %zu\n", set.length);
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
  }
  while(!set_valid(set, victim));
  set_reduce(set, guessPool, victim);
 return set;
}

