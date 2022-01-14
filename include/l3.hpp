//
// Created by rf on 1/12/22.
//

#ifndef LIBCACHEMONEY_INCLUDE_L3_HPP
#define LIBCACHEMONEY_INCLUDE_L3_HPP

#include <cstddef>
#include <cstdint>
#include <nmmintrin.h>
#include <bitset>
#include <bit>

namespace cache::l3 {
uint64_t speed(uint64_t iterations = 10000);

bool is_start_of_cache_line(uintptr_t address);
[[nodiscard]] consteval size_t size() { return 8*1024*1024; } //with total cache size being 16MB
[[nodiscard]] consteval size_t line_size() { return 64; }
[[nodiscard]] consteval size_t assoc() { return 16; }
[[nodiscard]] consteval size_t set_size() { return assoc()*line_size(); }
[[nodiscard]] consteval size_t set_count() { return size()/set_size(); }

[[nodiscard]] consteval size_t get_page_bits() { return 12; } // p
[[nodiscard]] consteval size_t get_line_bits() { return 6; } // l
[[nodiscard]] consteval size_t get_slice_bits() { return 3; } // s
[[nodiscard]] consteval size_t get_set_bits() { return 10; } // c
[[nodiscard]] consteval size_t get_page_size() { return 1 << get_page_bits(); }
[[nodiscard]] consteval size_t get_line_size() { return 1 << get_line_bits(); }
[[nodiscard]] consteval size_t get_slices_count() { return 1 << get_slice_bits(); }
[[nodiscard]] consteval size_t get_cache_sets() { return 1 << get_set_bits(); }

inline uint64_t get_physical_address_cache_set(uintptr_t physicalAddress) {
  return (physicalAddress >> get_line_bits()) & (get_cache_sets() - 1);
}

//  return std::popcount(physicalAddress & 0x3cccc93100ULL)% 2; // TODO add paper reference
inline uint64_t get_physical_slice(uintptr_t physicalAddress) {
  uint64_t ret = __builtin_popcount(0x3cccc93100ULL & physicalAddress)%2;
  ret = (ret << 1) | __builtin_popcount(0x2eb5faa880ULL & physicalAddress)%2;
  ret = (ret << 1) | __builtin_popcount(0x1b5f575440ULL & physicalAddress)%2;
  return ret;
}

} // namespace cache
#endif //LIBCACHEMONEY_INCLUDE_L3_HPP
