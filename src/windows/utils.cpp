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
#include <fcntl.h>
#include <sys/stat.h>
#include <vector>
#include "intrinsics.hpp"
#include <windows.h>

namespace utils {
    uintptr_t get_page_start(uintptr_t address) {
        SYSTEM_INFO si;
        GetSystemInfo(&si);
        return address & ~(si.dwPageSize - 1);
    }

    bool has_privilege() {
        return false; //TODO IMPLEMENT
    }

    uintptr_t map_shared_object(const char *file) {
        return 0; //TOOD Implement
    }

}
