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

#ifndef LIBCACHEMONEY_UTILS_HPP
#define LIBCACHEMONEY_UTILS_HPP

#include <cstdint>

namespace utils {
    [[nodiscard]] uintptr_t get_page_start(uintptr_t address);

    [[nodiscard]] int has_privilege();

    [[nodiscard]] uintptr_t map_shared_object(const char *filename);

    [[nodiscard]] uint64_t get_memory_baseline_speed(uint64_t iterations = 10000);

    [[nodiscard]] uint64_t get_cache_baseline_speed(uint64_t iterations = 10000);

    void cycle_wait(uint64_t delay);
}

#endif