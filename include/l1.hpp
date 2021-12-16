//
// Created by rf on 12/14/21.
//

#ifndef LIBCACHEMONEY_L1_HPP
#define LIBCACHEMONEY_L1_HPP

#include <cstddef>
#include <cstdint>
namespace cache {
namespace l1 {
size_t size();
uint64_t speed(uint64_t iterations = 10000);
size_t line_size();
bool is_start_of_cache_line(uintptr_t address);
} // namespace l1
} // namespace cache
#endif // LIBCACHEMONEY_L1_HPP
