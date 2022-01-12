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

#include "intrinsics.hpp"
#include "l3.hpp"
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <utils.hpp>
#include <cstring>
#include <cstdio>
#include <vector>
#include <bitset>

namespace utils {

bool is_page_start(uintptr_t address) { return address%getpagesize()==0; }

uintptr_t get_page_start(uintptr_t address) {
  return address & ~(getpagesize() - 1);
}



bool has_privilege() {
  uid_t uid = getuid(), euid = geteuid();
  return (euid <= 0 || uid!=euid);
}

//  * Bits 0-54  page frame number (PFN) if present
//	* Bits 0-4   swap type if swapped
//	* Bits 5-54  swap offset if swapped
//	* Bit  55    pte is soft-dirty (see Documentation/vm/soft-dirty.txt)
//  * Bit  56    page exclusively mapped (since 4.2)
//  * Bits 57-60 zero
//	* Bit  61    page is file-page or shared-anon (since 3.5)
//  * Bit  62    page swapped
//  * Bit  63    page present
//TODO wrap an api around this
uintptr_t get_physical_address(uintptr_t virtualAddress) {
  static const char *FILE = "/proc/self/pagemap";

//TOOD utils should be independent of other resources fix this
  const size_t PAGE_SIZE = cache::l3::get_page_size();

  int fd = open(FILE, O_RDONLY);

  if (fd < 0) {
	return -1;
  }

  uintptr_t paddr = -1;
  uint32_t index = get_page_index(virtualAddress);
  if (pread(fd, &paddr, sizeof(uintptr_t), index)!=sizeof(uintptr_t)) {
	return -1;
  }
  close(fd);
  paddr &= 0x7fffffffffffff;
  //TODO utils should be indepndent of other page resousces
  return (paddr << cache::l3::get_page_bits() | (virtualAddress & (PAGE_SIZE - 1)));
}

uint32_t get_page_index(uintptr_t virtualAddress) {
  return (virtualAddress/getpagesize())*sizeof(uintptr_t);
}

} // namespace utils
