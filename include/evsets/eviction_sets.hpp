//
// Created by rf on 1/13/22.
//

#ifndef LIBCACHEMONEY_INCLUDE_EVSETS_EVICTION_SETS_HPP
#define LIBCACHEMONEY_INCLUDE_EVSETS_EVICTION_SETS_HPP

#include <cstdint>
#include <vector>
#include <array>
#include <elem.hpp>
#include <l3.hpp>
#include <intrinsics.hpp>
#include <iostream>
#include <utils.hpp>

typedef elem<uintptr_t> element;
using namespace cache;

namespace eviction_sets {

void accsess_prime_pattern_safe(const std::array<element *, l3::assoc()> &evset) {
  const static size_t REPEATS = 3;
  const static size_t STRIDES = 4;

  for (int r = 0; r < REPEATS; r++) {
	for (int i = 0; i < l3::assoc() - 3; i += STRIDES) {
	  intrinsics::maccess::nullgaurd(evset[i]); //0
	  intrinsics::maccess::nullgaurd(evset[i + 1]); //1
	  intrinsics::maccess::nullgaurd(evset[0]); //S
	  intrinsics::maccess::nullgaurd(evset[0]); //S
	  intrinsics::maccess::nullgaurd(evset[i + 2]); //2
	  intrinsics::maccess::nullgaurd(evset[0]); //S
	  intrinsics::maccess::nullgaurd(evset[0]); //S
	  intrinsics::maccess::nullgaurd(evset[i + 3]); //3
	  intrinsics::maccess::nullgaurd(evset[i]); //0
	  intrinsics::maccess::nullgaurd(evset[i + 1]); //1
	  intrinsics::maccess::nullgaurd(evset[i + 2]); //2
	  intrinsics::maccess::nullgaurd(evset[i + 3]); //3
	}
  }
}

void accsess_prime_pattern(const std::array<element *, l3::assoc()> &evset) {
  const static size_t REPEATS = 3;
  const static size_t STRIDES = 4;

  for (int r = 0; r < REPEATS; r++) {
	for (int i = 0; i < l3::assoc() - 3; i += STRIDES) {
	  intrinsics::maccess::normal(evset[i]); //0
	  intrinsics::maccess::normal(evset[i + 1]); //1
	  intrinsics::maccess::normal(evset[0]); //S
	  intrinsics::maccess::normal(evset[0]); //S
	  intrinsics::maccess::normal(evset[i + 2]); //2
	  intrinsics::maccess::normal(evset[0]); //S
	  intrinsics::maccess::normal(evset[0]); //S
	  intrinsics::maccess::normal(evset[i + 3]); //3
	  intrinsics::maccess::normal(evset[i]); //0
	  intrinsics::maccess::normal(evset[i + 1]); //1
	  intrinsics::maccess::normal(evset[i + 2]); //2
	  intrinsics::maccess::normal(evset[i + 3]); //3
	}
  }
}

std::array<element *, l3::assoc()> construct_inclusive_brute_force(uintptr_t victim,
																   element *start,
																   size_t SIZE) {
  std::array<element *, l3::assoc()> evset{};
  uint32_t size = 0;
  uintptr_t physicalAddress = utils::get_physical_address(victim);
  uint32_t set = l3::get_physical_cache_set(physicalAddress);
  uint32_t slice = l3::get_physical_slice(physicalAddress);

  element *guess = start;
  uint64_t guessSet = -1, guessSlice = -1;
  uintptr_t guessPhysicalAddress = -1;

  while (size < l3::assoc()) {
	while (set!=guessSet || guessSlice!=slice || guessPhysicalAddress==physicalAddress) {
//	  if ((uintptr_t)guess >= (uintptr_t)guess + SIZE)
//		[[unlikely]]
//			throw std::runtime_error("out of bounds");
//	  else [[likely]] {
	  guess++;
	  guessPhysicalAddress = utils::get_physical_address((uintptr_t)guess);
	  guessSet = l3::get_physical_cache_set(guessPhysicalAddress);
	  guessSlice = l3::get_physical_slice(guessPhysicalAddress);
//	  }
	}
	if (size!=0) [[unlikely]] {
	  evset[size] = guess;
	  guess->prev = evset[size - 1];
	  guess->prev->next = guess;
	} else {
	  guess->prev = NULL;
	  evset[0] = guess;
	}

	size++;
	guess++;

  }

  evset[evset.size() - 1]->next = NULL;
  return evset;
}

inline void accsess_ev(const std::array<element *, l3::assoc()> &evset, element *target) {
  const static size_t REPEATS = 3;
  for (int r = 0; r < REPEATS; r++) {
	for (int i = 0; i < cache::l3::assoc(); i++) {
	  throw std::runtime_error("OUTDATED");
	  intrinsics::maccess::double_fenced(target);
	  intrinsics::maccess::nullgaurd(evset[i]);
	}
  }
}

bool verify_eviction_set(const std::array<element *, cache::l3::assoc()> &S) {
  if (S.size()==0)
	return true;
  uintptr_t address = (uintptr_t)S[0];
  uintptr_t phy = utils::get_physical_address((uintptr_t)address);
  uint64_t set = cache::l3::get_physical_cache_set(phy);
  uint64_t slice = cache::l3::get_physical_slice(phy);
  for (auto addy : S) {
	uintptr_t phy = utils::get_physical_address((uintptr_t)addy);
	uint64_t _set = cache::l3::get_physical_cache_set(phy);
	uint64_t _slice = cache::l3::get_physical_slice(phy);
	if (_set!=set || _slice!=slice)
	  return false;
  }
  return true;
}






void zig_zag(element *target, element *evset) {
  while (evset) {
	intrinsics::maccess::normal(target);
	intrinsics::maccess::normal(evset);
	evset = evset->next;
  }
}

inline
void
traverse_list_skylake(element *ptr) {
  while (ptr && ptr->next && ptr->next->next) {
	intrinsics::maccess::normal(ptr);
	intrinsics::maccess::normal(ptr->next);
	intrinsics::maccess::normal(ptr->next->next);
	intrinsics::maccess::normal(ptr);
	intrinsics::maccess::normal(ptr->next);
	intrinsics::maccess::normal(ptr->next->next);
	ptr = ptr->next;
  }
}

inline
void
traverse_list_asm_skylake(element *ptr) {
  __asm__ volatile
  (
  "test %%rcx, %%rcx;"
  "jz out;"
  "loop:"
  "movq (%%rcx), %%rax;" // memread 0
  "test %%rax, %%rax;" //
  "jz out;"
  "movq (%%rax), %%rax;" // memread 1
  "test %%rax, %%rax;" //
  "jz out;"
  "movq (%%rax), %%rax;" // memread 2
  "movq (%%rcx), %%rcx;" // memread 0, ptr is now 1
  "movq (%%rcx), %%rax;" // memread 1
  "movq (%%rax), %%rax;" // memread 2
  "test %%rcx, %%rcx;"
  "jnz loop;"
  "out:"
  : // no output
  : "c" (ptr) // ptr in rcx
  : "cc", "memory"
  );
}

static inline int comp(const void *a, const void *b) {
  return (*(uint64_t *)a - *(uint64_t *)b);
}

bool ps_evset_test(const std::array<element *, cache::l3::assoc()> &evset, uintptr_t victim, int threshold = 203) {

  int time[evset.size()], time_acc = 0;

  // Place TARGET in LLC
  intrinsics::maccess::double_fenced(victim);

  for (int test = 0; test < evset.size(); test++) {

	traverse_list_skylake(evset[0]);
	traverse_list_skylake(evset[0]);
	traverse_list_skylake(evset[0]);
	traverse_list_skylake(evset[0]);

	intrinsics::lfence(); intrinsics::mfence();
	// Measure TARGET (and place in LLC for next iteration)
	time[test] = intrinsics::memaccesstime::fenced(victim);
	time_acc += time[test];
  }
  qsort(time, evset.size(), sizeof(int), comp);
  return time[evset.size()/2] > threshold;
}

std::array<element *, cache::l3::assoc()> construct_eviction_set_v2(uintptr_t target,
																	element *start,
																	size_t SIZE, uint64_t threshold = 203) {

  const static uint32_t MAX_ATTEMPTS = 32;

  std::array<element *, cache::l3::assoc()> evset{};
  uint32_t size = 0;

  element *guess = start;

  for (uint32_t attempt = 0; attempt < MAX_ATTEMPTS; attempt++) {
	while (size < cache::l3::assoc()) {
	  intrinsics::maccess::normal(target);
	  intrinsics::lfence();
	  intrinsics::mfence();

//	  intrinsics::maccess::normal()

	}

	if(ps_evset_test(evset, target))
	  break;
  }
  return evset;

}

std::array<element *, cache::l3::assoc()> construct_eviction_set(element *target,
																 element *start,
																 size_t SIZE, uint64_t threshold = 203) {
  std::array<element *, cache::l3::assoc()> evset{};
  uint32_t size = 0;

  element *guess = start;

  while (size < cache::l3::assoc()) {
	intrinsics::memaccesstime::fenced(target);
	intrinsics::memaccesstime::fenced(target);
	do {
	  guess++;

	  if ((uintptr_t)guess >= (uintptr_t)guess + SIZE)
		[[unlikely]] throw std::runtime_error("out of bounds");

//	  zig_zag(target, evset[0]);
	  accsess_prime_pattern_safe(evset);
	} while (intrinsics::memaccesstime::fenced(target) < threshold);

	if (size!=0) [[unlikely]] {
	  evset[size] = guess;
	  guess->prev = evset[size - 1];
	  guess->prev->next = guess;
	} else {
	  guess->prev = NULL;
	  evset[0] = guess;
	}

	size++;
	guess++;

  }

//  print_eviction_set_information(evset);
  return evset;
}

}

#endif //LIBCACHEMONEY_INCLUDE_EVSETS_EVICTION_SETS_HPP
