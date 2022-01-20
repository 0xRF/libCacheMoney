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
  for (int i = 0; i < 500; i++) {
	//Reload target into the cache
	for (int j = 0; j < 10; j++)
	  intrinsics::memaccesstime::fenced(target);
	accsess_prime_pattern_safe(evset);
	times.push_back(intrinsics::memaccesstime::fenced(target));
  }

  int cnt = 0;
  for (auto time : times)
	if (time > 50) // 50 is approx a value which worked on the prime+scope implemenation consitently
	  cnt++;
  std::cout << "Possibly evicted: " << cnt << "/500" << std::endl;

  return true;

}

int main() {
  using namespace std::chrono;
  typedef high_resolution_clock::time_point time_point;

  //The L3 cache size is 8MB so double that
  const static size_t SIZE = 1024*1024*8*4;
  element *buffer = (element *)malloc(SIZE);
  std::cout << "malloc'd: " << malloc_usable_size(buffer) << '/' << SIZE << std::endl;
  memset(buffer, 0xCC, SIZE);

  uintptr_t target = (uintptr_t)buffer;
  buffer++;

  time_point t1 = std::chrono::high_resolution_clock::now();

  auto S = eviction_sets::construct_inclusive_brute_force(target, buffer, SIZE);
  test_l3_evset(S, target);

  time_point t2 = high_resolution_clock::now();
  milliseconds ms = duration_cast<milliseconds>(t2 - t1);
  std::cout << ms.count() << " ms\n";
  return 0;
}

