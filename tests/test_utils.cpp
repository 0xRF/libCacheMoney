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
#include <cstdlib>

uintptr_t driver_get_page_of_address(uintptr_t address) {
    return utils::get_page_start(address); //TODO IMPLEMENT
}

int main(int argc, char **argv) {
    int value = 0;
    return driver_get_page_of_address((uintptr_t)&value) != utils::get_page_start((uintptr_t )&value);
}
