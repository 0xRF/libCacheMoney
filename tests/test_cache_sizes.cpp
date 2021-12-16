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

#include <iostream>
#include <vector>

using namespace cache;

#define ARRAY_SIZE 1048576

int main(void) {
    register size_t size asm("r8") = 32 * 1024;
    register uint64_t speed asm("r9") = l1::speed();
    register size_t line_size asm("r10") = l1::line_size();
    register int misses asm("r11") = 0;
    register int page_size asm("r12") = 4096;
    register size_t i asm("rcx") = 0;

    auto buffer = (char *) malloc(ARRAY_SIZE);
    register uintptr_t start_address asm("rbx") =
            (uintptr_t) utils::get_page_start((uintptr_t) buffer + 4096);

    for (i = 0; i < ARRAY_SIZE; i += 8) {
        start_address = (uintptr_t) (start_address + i);
        if (l1::is_start_of_cache_line(start_address)) {
            printf("Cache Line Start %lx\n", start_address);
            std::cout << start_address << std::endl;
            break;
        }
    }//Should always be 0??, just here for completeness

    i = size;
    //when u dont wanna memset
    asm("xor %rax, %rax");
    asm("mov %rbx, %rdi");
    asm("rep stosb");

//    8 x 32 KB 8-way set associative instruction caches
//    Hence 64 bit cache lines, 8 asosciates per cache

    //walk cache lines
//    for (i = 0; i < size; i++)

    return 0;
}

int fmain(void) {
    register size_t size asm("r8") = 32 * 1024;
    register uint64_t speed asm("r9") = l1::speed();
    register size_t line_size asm("r10") = l1::line_size();
    register int misses asm("r11") = 0;
    register int page_size asm("r12") = 4096;
    register size_t i asm("rcx") = 0;

    auto buffer = (char *) malloc(ARRAY_SIZE);
    register char *start_address asm("rbx") =
            (char *) utils::get_page_start((uintptr_t) buffer + 4096);

    i = ARRAY_SIZE - 4096;
    asm("xor %rax, %rax");
    asm("mov %rbx, %rdi");
    asm("rep stosb");

    for (i = 0; i < ARRAY_SIZE; i++) {
        if (i == 4096) {
            if (((uintptr_t) (start_address + i) % 4096) == 0) {
                start_address = start_address + i;
                break;
            }
        }
        if ((uintptr_t) (start_address + i % 4096) == 0) {
            start_address = start_address + i;
            break;
        }
    }
    return 0;
}
