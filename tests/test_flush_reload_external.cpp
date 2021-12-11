/*
 * This file is part of the libCacheMoney (https://github.com/0xRF/libCacheMoney)
 * Copyright (c) 2021 0xRF
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <utils.hpp>
#include <attacks/flush_reload.hpp>
#include <cassert>

#include <stdio.h>

using namespace cache_money;

int main(int argc, char **argv) {

    uintptr_t targetAddress = (uintptr_t) puts;

    flush_reload fr((uintptr_t) targetAddress);

    ///Should fail as the address the target address is being cleared from the cache
//    assert(fr.attack(10, {}, nullptr, 80000) == false);

    assert(fr.attack(10, {}, {}) == true);

    return EXIT_SUCCESS;
}