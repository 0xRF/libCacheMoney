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

#include <utils.hpp>
#include <intrinsics.hpp>
#include <attacks/flush_reload.hpp>
#include <iostream>
#include <cassert>

using namespace cache_money;

int main(int argc, char **argv) {

    auto targetAddress = (uintptr_t *) malloc(8);
    flush_reload fr((uintptr_t) targetAddress);

    ///Should fail as the address the target address is being cleared from the cache
    assert(fr.attack(10, {}, nullptr, 80000) == false);

    //Should SUCCEED AS THE TRIGGER FUNCTION WILL BE CONSTANTLY CALLED RELOADING TARGET ADDRESS IN CACHE
    auto trigger = [&]() -> void {
        *targetAddress += 1;
    };
    assert(fr.attack(10, {}, trigger) == true);

    return EXIT_SUCCESS;
}
