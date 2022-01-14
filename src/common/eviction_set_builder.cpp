//
// Created by rf on 1/11/22.
//
#include <eviction_set_builder.hpp>
#include <intrinsics.hpp>

//bool eviction_set_builder::test(const std::vector<uintptr_t> &S, size_t a, uint64_t threshold) {
//  //Load the entire eviction set into L1 cache
//  for (auto address : S)
//	intrinsics::maccess(address);
//
//  //Count the misses, if more than a misses
//  int misses = 0;
//  for (auto address : S) {
//
//	if (intrinsics::memaccesstime(address) > threshold)
//	  misses++;
//
//	if (misses > a)
//	  return true;
//  }
//
//  return false;
//}

bool eviction_set_builder::test(elem<eviction_set_element> *S, size_t a, uint64_t threshold) {
  //Load the entire eviction set into L1 cache
  elem<eviction_set_element>* evs = S;
  while (evs) {
	evs = evs->next;
  }
  evs = S;
  register uint64_t time asm("r15") = 0;

  //Do the second iteration
  while (evs->next) {
	time = intrinsics::memaccesstime::fenced((uintptr_t)evs->next);
	intrinsics::lfence();
	evs->value.time = time;
	evs = evs->next;
  }

  evs = S;

  //Count the misses, if more than a misses
  int misses = 0;
  while (evs) {
	if(evs->value.time > threshold)
	  misses++;
	evs = evs->next;
  }

  //atm as how the evsets are constructed the condition for what is an ev do not match the papers
  //this can be easily fixed later tho
  return misses < a || (a == 0 && misses == 0);
}
