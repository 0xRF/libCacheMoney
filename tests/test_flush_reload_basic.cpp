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
