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

typedef elem<uintptr_t> element;

namespace eviction_sets {


void accsess_prime_pattern(const std::array<element*, cache::l3::assoc()>& evset, element* target){
  const static size_t REPEATS = 3;
  const static size_t STRIDES = 4;

  for (int r = 0; r < REPEATS; r++) {
	for (int i = 0; i < cache::l3::assoc() - 3; i += STRIDES) {
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

std::array<element *, cache::l3::assoc()> construct_inclusive_brute_force(element *victim,
																		  element *start,
																		  size_t SIZE) {
  std::array<element *, cache::l3::assoc()> evset{};
  uint32_t size = 0;
  uintptr_t physicalAddress = utils::get_physical_address((uintptr_t)victim);
  uint32_t set = cache::l3::get_physical_cache_set(physicalAddress);
  uint32_t slice = cache::l3::get_physical_slice(physicalAddress);

  element *guess = start;
  uint64_t guessSet = 0, guessSlice = 0;
  uintptr_t guessPhysicalAddress = 0;

  while (size < cache::l3::assoc()) {
	while (set!=guessSet || guessSlice!=slice || guessPhysicalAddress==physicalAddress) {
	  if ((uintptr_t)guess >= (uintptr_t)guess + SIZE)
		[[unlikely]]
			throw std::runtime_error("out of bounds");
	  else [[likely]] {
		guess++;
		guessPhysicalAddress = utils::get_physical_address((uintptr_t)guess);
		guessSet = cache::l3::get_physical_cache_set(guessPhysicalAddress);
		guessSlice = cache::l3::get_physical_slice(guessPhysicalAddress);
	  }
	}
	evset[size++] = guess;
  }

  return evset;
}

inline void accsess_ev(const std::array<element *, cache::l3::assoc()> &evset, element *target) {
  const static size_t REPEATS = 3;
  for (int r = 0; r < REPEATS; r++) {
	for (int i = 0; i < cache::l3::assoc(); i++) {
	  intrinsics::maccess::fenced(target);
	  intrinsics::maccess::nullgaurd(evset[i]);
	}
  }
}

std::array<element *, cache::l3::assoc()> construct_eviction_set(element *target,
																 element *start,
																 size_t SIZE, uint64_t threshold = 60) {
  std::array<element *, cache::l3::assoc()> evset{};
  uint32_t size = 0;

  element *guess = start;

  while (size < cache::l3::assoc()) {
	intrinsics::memaccesstime::fenced(target);
	do {
	  guess++;

	  if ((uintptr_t)guess >= (uintptr_t)guess + SIZE) [[unlikely]]
			throw std::runtime_error("out of bounds");

	  accsess_ev(evset, target);
	} while (intrinsics::memaccesstime::fenced(target) < threshold);

//	evset[size]->next = guess;
//	guess->prev = evset[size];
	evset[size++] = guess;
  }

  return evset;
}

}

#endif //LIBCACHEMONEY_INCLUDE_EVSETS_EVICTION_SETS_HPP
