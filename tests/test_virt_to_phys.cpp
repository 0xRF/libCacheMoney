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

using namespace utils;
using namespace eviction_sets;
using namespace cache;

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

//
//#define PAGE_BITS 12
//#define LINE_BITS 6
//#define SLICE_BITS 3
//#define SET_BITS 10
//
//#define PAGE_SIZE2 (1 << PAGE_BITS)
//#define LINE_SIZE (1 << LINE_BITS)
//#define CACHE_SLICES (1 << SLICE_BITS)
//#define CACHE_SETS (1 << SET_BITS)
//
//unsigned long long
//vtop(unsigned pid, unsigned long long vaddr) {
//  char path[1024];
//  sprintf(path, "/proc/%u/pagemap", pid);
//  int fd = open(path, O_RDONLY);
//  if (fd < 0) {
//	return -1;
//  }
//
//  unsigned long long paddr = -1;
//  unsigned long long index = (vaddr/PAGE_SIZE2)*sizeof(paddr);
//  if (pread(fd, &paddr, sizeof(paddr), index)!=sizeof(paddr)) {
//	return -1;
//  }
//  close(fd);
//  paddr &= 0x7fffffffffffff;
//  return (paddr << PAGE_BITS) | (vaddr & (PAGE_SIZE2 - 1));
//}
//
//unsigned int
//count_bits(unsigned long long n) {
//  unsigned int count = 0;
//  while (n) {
//	n &= (n - 1);
//	count++;
//  }
//  return count;
//}
//
//unsigned long long
//ptos(unsigned long long paddr, unsigned long long bits) {
//  unsigned long long ret = 0;
//  unsigned long long mask[3] = {0x1b5f575440ULL, 0x2eb5faa880ULL, 0x3cccc93100ULL}; // according to Maurice et al.
//
//  printf("BITS: 0x%llx 0x%llx\n", count_bits(mask[2] & paddr), __builtin_popcount(mask[2] & paddr));
//
//  switch (bits) {
//  case 3: ret = (ret << 1) | (unsigned long long)(count_bits(mask[2] & paddr)%2);
//  case 2: ret = (ret << 1) | (unsigned long long)(count_bits(mask[1] & paddr)%2);
//  case 1: ret = (ret << 1) | (unsigned long long)(count_bits(mask[0] & paddr)%2);
//  default: break;
//  }
//  return ret;
//}
//
//void
//check(unsigned int pid, unsigned long long *virtual_addresses, unsigned int length) {
//  unsigned int cache_sets = 1024;
//
//  unsigned long long paddr = 0, cacheset = 0, slice = 0;
//  uintptr_t physicalAddress = 0, cacheSet = 0, pSlice = 0;
//
//  for (unsigned int i = 0; i < length; i++) {
//	paddr = vtop(pid, virtual_addresses[i]);
//	cacheset = (paddr >> LINE_BITS) & (CACHE_SETS - 1);
//	slice = ptos(paddr, SLICE_BITS);
//
//	physicalAddress = utils::get_physical_address((uintptr_t)virtual_addresses[i]);
//	cacheSet = l3::get_physical_cache_set(physicalAddress);
//	pSlice = l3::get_physical_slice(physicalAddress);
//
//	printf("element pfn: 0x%llx - 0x%llx\n", paddr, physicalAddress);
//	printf("cache set: 0x%llx - 0x%llx\n", cacheset, cacheSet);
//	printf("slice: 0x%llx - 0x%llx\n\n", slice, pSlice);
//  }
//}


//

bool test_l3_evset(const std::array<element *, l3::assoc()> &evset, element *target) {

    //Wait until evicted from te cache
    int cnt = 0;
    uint64_t time = 0;
    std::vector<uint64_t> times{};
    while ((time = intrinsics::memaccesstime::fenced(target)) < 50) {
        accsess_prime_pattern(evset);
        times.push_back(time);
    }
    for (auto time: times)
        std::cout << time << " ";
    std::cout << std::endl;

    return true;

}

int main() {
    using namespace std::chrono;
    typedef high_resolution_clock::time_point time_point;

    //The L3 cache size is 8MB so double that
    const static size_t SIZE = 1024 * 1024 * 8 * 4;
    element *buffer = (element *) malloc(SIZE);
    std::cout << "malloc'd: " << malloc_usable_size(buffer) << '/' << SIZE << std::endl;
    memset(buffer, 0xCC, SIZE);

    element *target = buffer;
    buffer++;

    time_point t1 = std::chrono::high_resolution_clock::now();

    auto S = eviction_sets::construct_eviction_set(target, buffer, SIZE);

    time_point t2 = high_resolution_clock::now();
    milliseconds ms = duration_cast<milliseconds>(t2 - t1);
    std::cout << ms.count() << " ms\n";

    test_l3_evset(S, target);


//  auto S = eviction_sets::construct_inclusive_brute_force(buffer, (SIZE)/sizeof(element), l1::speed());

//  check((unsigned int)getpid(), (unsigned long long *)S.data(), (unsigned int)S.size());
    return 0;
}


//
//
//  register uint32_t dst asm("r13") = 0;
//  for (register uint32_t i asm("r14") = 0; i < 5; i++) {
//	asm("popcnt %1, %0"
//	: "=r" (dst)
//	: "r" (i));
//	std::cout << dst << std::endl;
////	asm("popcnt %r14, 5");
//  }
//  std::cout << std::endl;
//
//  for (uint32_t i = 0; i < 5; i++) {
//	std::cout << count_bits(i) << ' ';
//  }
//  std::cout << std::endl;
//
//  for (uint32_t i = 0; i < 5; i++) {
//	std::cout << std::popcount(i) << ' ';
//  }
//  std::cout << std::endl;
//
//
//  {
//	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
//	for (uint32_t i = 0; i < 100000; i++) {
//	  count_bits(i);
//	}
//	high_resolution_clock::time_point t2 = high_resolution_clock::now();
//	nanoseconds ns = duration_cast<nanoseconds>(t2 - t1);
//	std::cout << "evset countbits " << ns.count() << " ns\n";
//  }
//  {
//	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
//	for (uint32_t i = 0; i < 100000; i++) {
//	  std::popcount(i);
//	}
//	high_resolution_clock::time_point t2 = high_resolution_clock::now();
//	nanoseconds ns = duration_cast<nanoseconds>(t2 - t1);
//	std::cout << "std::popcount   " << ns.count() << " ns\n";
//  }
//
//  {
//	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
//	  for (uint32_t i = 0; i < 100000; i++) {
//	  __builtin_popcount(i);
//	}
//	high_resolution_clock::time_point t2 = high_resolution_clock::now();
//	nanoseconds ns = duration_cast<nanoseconds>(t2 - t1);
//	std::cout << "builtin         " << ns.count() << " ns\n";
//  }
//
//  return 0;


