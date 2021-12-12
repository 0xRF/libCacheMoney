/*
libCacheMoney
Copyright (C) 2021 0xRF

libCacheMoney is free software: you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

libCacheMoney is program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with libCacheMoney.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <intrinsics.hpp>
#include <intrin.h>

namespace intrinsics {
    uint64_t rdtsc() {
        return __rdtsc();
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
        return 0;
//        return __rdtscp();
    }


    void clflush(uintptr_t address) {
        //asm("cpuid");
        _mm_clflush((void*)address);
    }
}
