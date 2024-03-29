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
#include <intrinsics.hpp>
#include <utils.hpp>
#include <vector>
#include <l1.hpp>
#include <bitset>
#include <exception>
#include <stdexcept>

namespace utils {
uint64_t get_memory_baseline_speed(uint64_t iterations) {
  auto times = std::vector<uint64_t>();
  auto pTest = new uintptr_t;
  uintptr_t page_start = utils::get_page_start((uintptr_t)pTest);

  for (uint64_t i = 0; i < iterations; i++) {
	intrinsics::clflush(page_start);
	uint64_t time = intrinsics::memaccesstime::fenced(page_start);
	times.push_back(time);
  }
  delete pTest;
  return *std::min_element(times.begin(), times.end());
}

void cycle_wait(uint64_t delay) {
  const uint64_t start_time = intrinsics::rdtscp64();
  while (intrinsics::rdtscp64() - start_time <= delay) {
  }
}

void prefetch(uintptr_t start, size_t size) {
  for (uintptr_t ptr = start; ptr < start + sizeof(uintptr_t)*size; ptr += sizeof(uintptr_t)) {
	intrinsics::prefetch0(ptr);
  }
}
void prefetch_range(uintptr_t start, uintptr_t end) {
  for (uintptr_t ptr = start; ptr < end; ptr += sizeof(uintptr_t)) {
	intrinsics::prefetch0(ptr);
  }
}

uintptr_t get_aligned_address(uintptr_t buffer, size_t size) {
  for (uintptr_t address = buffer; address < buffer + size; address++)
	if (utils::is_page_start(address) && cache::l1::is_start_of_cache_line(address) && get_address_set(address)==0)
	  return address;

  throw std::runtime_error("Could not find an aligned address");
}

//    size_t get_cache_set_outdated(uintptr_t address) {
//        auto bits = std::bitset<sizeof(uintptr_t) * 8>(address);
//        auto computed = (bits & get_index_bitmask());
//        return (computed >> 6).to_ulong();
//    }
uint64_t get_address_set(uintptr_t address) {
  return (address/cache::l1::line_size())%cache::l1::set_count();
}


constexpr std::bitset<sizeof(uintptr_t)*8> get_index_bitmask() {
  std::bitset<sizeof(uintptr_t)*8> mask;
  for (int i = 6; i < 12; i++)
	mask[i] = true;
  return mask;
}

constexpr std::bitset<sizeof(uintptr_t)*8> get_tag_bitmask() {
  std::bitset<sizeof(uintptr_t)*8> mask;
  for (int i = 13; i < 64; i++)
	mask[i] = true;
  return mask;
}

size_t get_address_tag(uintptr_t address) {
  auto bits = std::bitset<sizeof(uintptr_t)*8>(address);
  auto computed = (bits & get_tag_bitmask());
  return (computed >> 12).to_ulong();
}

constexpr int strtoi(const char *s) {
  int sum = 0;
  char ch;
  char sign = *s;
  if (*s=='-' || *s=='+')
	s++;
  while ((ch = *s++) >= '0' && ch <= '9') {
	sum = sum*10 - (ch - '0');
  }
  if (sign!='-') {
	sum = -sum;
  }
  return sum;
}

}
