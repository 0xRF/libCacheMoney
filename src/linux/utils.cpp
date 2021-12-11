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

#include <unistd.h>
#include <utils.hpp>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <vector>
#include "intrinsics.hpp"

namespace utils {
    uintptr_t get_page_start(uintptr_t address) {
        return address & ~(getpagesize() - 1);
    }

    int has_privilege() {
        uid_t uid = getuid(), euid = geteuid();
        return (euid <= 0 || uid != euid);
    }

    uintptr_t map_shared_object(const char *file) {

        int fd = open(file, O_RDONLY);
        struct stat info;

        if (fstat(fd, &info) != 0) return 0;

        auto mapped_address = (uintptr_t) mmap(nullptr, info.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

        close(fd);

        return mapped_address;
    }

    uint64_t get_memory_baseline_speed(uint64_t iterations) {
        auto times = std::vector<uint64_t>();
        auto pTest = new uintptr_t;
        uintptr_t page_start = utils::get_page_start((uintptr_t )pTest);

        for (uint64_t i = 0; i < iterations; i++) {
            intrinsics::clflush(page_start);
            uint64_t time = intrinsics::memaccesstime(page_start);
            times.push_back(time);
        }
        delete pTest;
        return *std::min_element(times.begin(), times.end());
    }

    uint64_t get_cache_baseline_speed(uint64_t iterations) {
        auto times = std::vector<uint64_t>();
        auto pTest = new uintptr_t;
        uintptr_t page_start = utils::get_page_start((uintptr_t )pTest);

        for (uint64_t i = 0; i < iterations; i++) {
            uint64_t time = intrinsics::memaccesstime(page_start);
            times.push_back(time);
        }
        delete pTest;
        return *std::min_element(times.begin(), times.end()) ;
    }


    void cycle_wait(uint64_t delay) {
        uint64_t start_time = intrinsics::rdtscp64();
        while(intrinsics::rdtscp64() - start_time <= delay){}
    }
}