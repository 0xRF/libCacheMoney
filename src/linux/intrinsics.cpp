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


#include <intrinsics.hpp>

namespace intrinsics {
    uint64_t rdtsc() {
        uint64_t high, low;
        asm("rdtsc" : "=a"(high), "=d"(low) : : "ebx", "ecx");
        return high | (low << 32);
    }

    uint32_t memaccesstime(uintptr_t address) {
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

    uint64_t rdtscp64() {
        uint32_t low, high;
        asm volatile("rdtscp" : "=a"(low), "=d"(high)::"ecx");
        return (((uint64_t) high) << 32) | low;
    }

    void clflush(uintptr_t address) {
        asm("cpuid");
        asm("clflush (%0)"::"r"(address));
    }
}
