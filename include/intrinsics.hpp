/*
 * Copyright 2016 CSIRO
 *
 * This file is part of Mastik.
 *
 * Mastik is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Mastik is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Mastik.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBMONEY_INTRINSICS_H
#define LIBMONEY_INTRINSICS_H

#include <cstdint>

namespace intrinsics {

inline uint64_t rdtsc() {
  uint64_t high, low;
  asm("rdtsc" : "=a"(high), "=d"(low) : : "ebx", "ecx");
  return high | (low << 32);
}

inline void lfence() {
  asm volatile("lfence");
}

inline void mfence() {
  asm volatile("mfence");
}

namespace memaccesstime {
inline uint32_t fenced(void *address) {
  uint32_t rv;
  asm volatile("mfence\n"
			   "lfence\n"
			   "rdtscp\n"
			   "mov %%eax, %%esi\n"
			   "mov (%1), %%eax\n"
			   "rdtscp\n"
			   "sub %%esi, %%eax\n"
  : "=&a"(rv)
  : "r"(address)
  : "ecx", "edx", "esi");
  return rv;
}

inline uint32_t fenced(uintptr_t address) {
  return fenced((void *)address);
}

inline uint32_t normal(void *address) {
  uint32_t rv;
  asm volatile("rdtscp\n"
			   "mov %%eax, %%esi\n"
			   "mov (%1), %%eax\n"
			   "rdtscp\n"
			   "sub %%esi, %%eax\n"
  : "=&a"(rv)
  : "r"(address)
  : "ecx", "edx", "esi");
  return rv;
}

inline uint32_t normal(uintptr_t address) {
  return normal((void *)address);
}

}

inline uint64_t rdtscp64() {
  uint32_t low, high;
  asm volatile("rdtscp" : "=a"(low), "=d"(high)::"ecx");
  return (((uint64_t)high) << 32) | low;
}

inline void clflush(uintptr_t address) {
  asm("cpuid");
  asm("clflush (%0)"::"r"(address));
}

inline void prefetch0(uintptr_t address) {
//    __builtin_prefetch((void*)address);
//TODO implement
}

inline void fence() {
  asm volatile("lfence;mfence");
}

namespace maccess {

inline void double_fenced(void *address) {
  asm volatile("lfence;mfence");
  __asm__ volatile ("movq (%0), %%rax\n" : : "c" (address) : "r15");
  asm volatile("lfence;mfence");
}

inline void double_fenced(uintptr_t address) {
  double_fenced((void *)address);
}

inline void nullgaurd(void *address) {
  if (address!=nullptr)
	  __asm__ volatile ("movq (%0), %%rax\n" : : "c" (address) : "r15");
  asm volatile("lfence;mfence");
}

inline void normal(void *address) {
  __asm__ volatile ("movq (%0), %%rax\n" : : "c" (address) : "r15");
}

inline void normal(uintptr_t address) {
  double_fenced((void *)address);
}

}

} // namespace intrinsics

#endif // LIBMONEY_INTRINSICS_H
