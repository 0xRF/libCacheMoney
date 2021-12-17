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
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <utils.hpp>
#include <vector>

namespace utils {

bool is_page_start(uintptr_t address) { return address % getpagesize() == 0; }

uintptr_t get_page_start(uintptr_t address) {
  return address & ~(getpagesize() - 1);
}

bool has_privilege() {
  uid_t uid = getuid(), euid = geteuid();
  return (euid <= 0 || uid != euid);
}

uintptr_t map_shared_object(const char *file) {

  int fd = open(file, O_RDONLY);
  struct stat info {};

  if (fstat(fd, &info) != 0)
    return 0;

  auto mapped_address =
      (uintptr_t)mmap(nullptr, info.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

  close(fd);

  return mapped_address;
}

} // namespace utils
