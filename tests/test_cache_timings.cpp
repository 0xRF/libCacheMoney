/*
libcachemoney
copyright (c) 2021 0xrf

libcachemoney is free software: you can redistribute it and/or modify
        it under the terms of the gnu general public license as published by
the free software foundation, either version 3 of the license, or
(at your option) any later version.

libcachemoney is program is distributed in the hope that it will be useful,
        but without any warranty; without even the implied warranty of
merchantability or fitness for a particular purpose.  see the
gnu general public license for more details.

you should have received a copy of the gnu general public license
along with libcachemoney.  if not, see <http://www.gnu.org/licenses/>.
 */

#include <cstdint>
#include <intrinsics.hpp>
#include <l1.hpp>
#include <random>
#include <utils.hpp>

#include <bitset>
#include <iostream>
#include <vector>
#include <iomanip>

using namespace cache;

constexpr size_t get_array_size() { return 1024 * 1024; } // 1 MiB

uintptr_t get_buffer_start_aligned_to_page(uintptr_t buffer) {
    for (uintptr_t address = buffer; address < buffer + get_array_size();
         address++) {
        if (utils::is_page_start(address))
            return address;
    }
    return 0;
}

constexpr std::bitset<sizeof(uintptr_t) * 8> get_index_bitmask() {
    std::bitset<sizeof(uintptr_t) * 8> mask;
    for (int i = 6; i < 12; i++)
        mask[i] = true;
    return mask;
}

/// Populate RCX with zeroes
inline void memsett(uintptr_t buffer) { // ensure works on windows and linux
    register size_t RCX asm("rcx") = cache::l1::size();
    register uintptr_t RDI asm("rdi") = buffer;
    asm("xor %rax, %rax");
    asm("rep stosb");
}

size_t get_cache_set(uintptr_t address) {
    return (address / cache::l1::line_size()) % cache::l1::set_count();
}


uintptr_t get_aligned_address(uintptr_t buffer) {
    for (uintptr_t address = buffer; address < buffer + get_array_size();
         address++) {
        if (utils::is_page_start(address) && cache::l1::is_start_of_cache_line(address) && get_cache_set(address == 0))
            return address;
    }
    return 0;
}

size_t get_cache_set_outdated(uintptr_t address) {
//    if (!cache::l1::is_start_of_cache_line(address))
//        return -1;
    auto bits = std::bitset<sizeof(uintptr_t) * 8>(address);
    auto computed = (bits & get_index_bitmask());
    return (computed >> 6).to_ulong();
}

int main() {
    auto true_buffer_start = (uintptr_t) malloc(get_array_size());
    auto address = get_aligned_address(true_buffer_start);

    register size_t RCX asm("rcx") = 0;
    register size_t set_count asm("r8") = l1::set_count();
    register size_t line_size asm("r9") = l1::line_size();
    register auto timings asm("r10") = (uintptr_t *) malloc(sizeof(uintptr_t) * set_count);
    register uint64_t addy asm("r11") = 0;
    register uint64_t R12 asm("r12") = 0;
    register uint64_t R13 asm("r13") = 10000;


    memsett(address);
    for (R12 = 0; R12 < R13; R12++) {
        for (RCX = 0; RCX < set_count; RCX++) {
            addy = address + RCX * line_size;
            timings[RCX] = intrinsics::memaccesstime(addy);
        }
    }

    auto cache_sets = std::array<uintptr_t, 64>();

    return 0;
}
