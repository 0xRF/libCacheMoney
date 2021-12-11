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
#include <iostream>

int main(int argc, char **argv) {
    uint64_t cache_speed = utils::get_cache_baseline_speed();
    uint64_t memory_speed = utils::get_memory_baseline_speed();
    std::cout << "Memory Speed " << memory_speed << std::endl;
    std::cout << "Cache Speed " << cache_speed << std::endl;
    return memory_speed > cache_speed ? EXIT_SUCCESS : EXIT_FAILURE;
}