//
// Created by rf on 12/14/21.
//

#ifndef LIBCACHEMONEY_L1_HPP
#define LIBCACHEMONEY_L1_HPP

#include <cstddef>
#include <cstdint>

namespace cache::l1 {
        uint64_t speed(uint64_t iterations = 10000);
        bool is_start_of_cache_line(uintptr_t address);
        size_t size();
        size_t line_size();
        size_t assoc();
        size_t set_size();
        size_t set_count();
    } // namespace cache
#endif // LIBCACHEMONEY_L1_HPP
