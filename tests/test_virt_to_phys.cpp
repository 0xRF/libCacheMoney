//
// Created by rf on 1/12/22.
//

#include <chrono>
#include <malloc.h>
#include <bit>
#include <utils.hpp>
#include <bitset>
#include <cstdint>
#include <l3.hpp>
#include <l1.hpp>
#include <evsets/eviction_sets.hpp>
#include <cstring>
#include <algorithm>
#include <numeric>

using namespace utils;
using namespace eviction_sets;
using namespace cache;

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

bool test_l3_evset(const std::array<element *, l3::assoc()> &evset, uintptr_t target) {



  //Wait until evicted from te cache
  uint64_t time = 0;
  std::vector<uint64_t> times{};
  for (int i = 0; i < 10; i++) {
	accsess_prime_pattern(evset);
	accsess_prime_pattern(evset);
	accsess_prime_pattern(evset);
	accsess_prime_pattern(evset);
	intrinsics::memaccesstime::normal(evset[0]);
	intrinsics::memaccesstime::normal(evset[0]);
	intrinsics::memaccesstime::normal(target);
	time = intrinsics::memaccesstime::normal(evset[0]);

	std::cout << time << ' ';
//	times.push_back(intrinsics::memaccesstime::fenced(target));
  }

  for(auto elem : evset){
	uintptr_t physicalAddress = utils::get_physical_address((uintptr_t)elem);
	uint32_t set = l3::get_physical_cache_set(physicalAddress);
	uint32_t slice = l3::get_physical_slice(physicalAddress);

	printf("Slice: %d\n", slice);
	printf("Set : %d\n", set);
	printf("Phys: 0x%llx\n", physicalAddress);
  }
  printf("\n");

  return true;
}

int main() {

  using namespace std::chrono;
  typedef high_resolution_clock::time_point time_point;

  uint32_t meme = 0 ;
  //The L3 cache size is 8MB so double that
  const static size_t SIZE = 1024*1024*8*4;
  element *buffer = (element *)malloc(SIZE);
  std::cout << "malloc'd: " << malloc_usable_size(buffer) << '/' << SIZE << std::endl;
  memset(buffer, 0xCC, SIZE);

  uintptr_t target = (uintptr_t)&meme;
  buffer = (element*)((uintptr_t)buffer + (target & (4096-1)));

  time_point t1 = std::chrono::high_resolution_clock::now();

  auto S = eviction_sets::construct_inclusive_brute_force(target, buffer, SIZE);

  printf("set: ");
  for(auto a : S)
	printf("0x%llx ", a);
  printf("\n");

  test_l3_evset(S, target);

  time_point t2 = high_resolution_clock::now();
  milliseconds ms = duration_cast<milliseconds>(t2 - t1);
  std::cout << ms.count() << " ms\n";
  return 0;
}

