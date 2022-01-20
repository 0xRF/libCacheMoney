//
// Created by rf on 1/20/22.
//

#ifndef LIBCACHEMONEY_INCLUDE_VIRT_TO_PHYS_HPP
#define LIBCACHEMONEY_INCLUDE_VIRT_TO_PHYS_HPP

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <linked_list.hpp>
#include <utils.hpp>
#include <l3.hpp>

#define PAGE_BITS 12
#define LINE_BITS 6
#define SLICE_BITS 3
#define SET_BITS 10

#define PAGE_SIZE2 (1 << PAGE_BITS)
#define LINE_SIZE (1 << LINE_BITS)
#define CACHE_SLICES (1 << SLICE_BITS)
#define CACHE_SETS (1 << SET_BITS)

unsigned long long
vtop(unsigned pid, unsigned long long vaddr) {
//  char path[1024];
  const char *path = "/proc/self/pagemap";
//  sprintf(path, "/proc/self/pagemap", pid);
  int fd = open(path, O_RDONLY);
  if (fd < 0) {
	return -1;
  }

  unsigned long long paddr = -1;
  unsigned long long index = (vaddr/PAGE_SIZE2)*sizeof(paddr);
  if (pread(fd, &paddr, sizeof(paddr), index)!=sizeof(paddr)) {
	return -1;
  }
  close(fd);
  paddr &= 0x7fffffffffffff;
  return (paddr << PAGE_BITS) | (vaddr & (PAGE_SIZE2 - 1));
}

unsigned int
count_bits(unsigned long long n) {
  unsigned int count = 0;
  while (n) {

	n &= (n - 1);
	count++;
  }
  return count;
}

unsigned int
nbits(unsigned long long n) {
  unsigned int ret = 0;
  n = n >> 1;
  while (n > 0) {
	n >>= 1;
	ret++;
  }
  return ret;
}

unsigned long long
ptos(unsigned long long paddr, unsigned long long bits) {
  unsigned long long ret = 0;
  unsigned long long mask[3] = {0x1b5f575440ULL, 0x2eb5faa880ULL, 0x3cccc93100ULL}; // according to Maurice et al.

  switch (bits) {
  case 3: ret = (ret << 1) | (unsigned long long)(count_bits(mask[2] & paddr)%2);
  case 2: ret = (ret << 1) | (unsigned long long)(count_bits(mask[1] & paddr)%2);
  case 1: ret = (ret << 1) | (unsigned long long)(count_bits(mask[0] & paddr)%2);
  default: break;
  }
  return ret;
}

void
check(unsigned int pid, linked_list *list) {
  unsigned int cache_sets = 1024;

  unsigned long long paddr = 0, cacheset = 0, slice = 0;

  node *ptr = list->start;
  while (ptr) {
	paddr = vtop(pid, (unsigned long long)ptr);
	cacheset = (paddr >> LINE_BITS) & (CACHE_SETS - 1);
	slice = ptos(paddr, SLICE_BITS);
	printf(" - element pfn: 0x%llx, cache set: 0x%llx, slice: 0x%llx\n", paddr, cacheset, slice);
	ptr = ptr->next;
  }
}

void print_eviction_set_information(linked_list *list) {
  node *ptr = list->start;
  while (ptr) {
	uintptr_t phy = utils::get_physical_address((uintptr_t)ptr);
	uint64_t set = cache::l3::get_physical_cache_set(phy);
	uint64_t slice = cache::l3::get_physical_slice(phy);
	printf(" # pfn: 0x%llx, ", phy);
	printf("cache set: 0x%llx, ", set);
	printf("slice 0x%llx\n", slice);

	unsigned long long paddr = vtop(getpid(), (unsigned long long)ptr);
	unsigned long cacheset = (paddr >> LINE_BITS) & (CACHE_SETS - 1);
	slice = ptos(paddr, SLICE_BITS);
//printf(" - pfn: 0x%llx, cache set: 0x%llx, slice: 0x%llx\n", paddr, cacheset, slice);

	ptr = ptr->next;
  }
}

#endif //LIBCACHEMONEY_INCLUDE_VIRT_TO_PHYS_HPP
