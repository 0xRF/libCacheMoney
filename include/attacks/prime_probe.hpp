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

#ifndef LIBMONEY_PRIME_PROBE_HPP
#define LIBMONEY_PRIME_PROBE_HPP

#include <cstdint>
#include <cstddef>
#include <array>
#include <vector>


namespace cache_money {
    class prime_probe {
    public:

        static const uint64_t SAMPLES = 50;

        std::array<std::array<uintptr_t, 8>, 64>
        generate_mapped_addresses(uintptr_t address, uintptr_t true_start, size_t array_size);

//        std::array<std::array<std::array<std::pair<uint32_t, uint64_t>, 8>, 64>, SAMPLES> *probe();

//        std::pair<std::vector<uint32_t>, std::vector<uint64_t>> probe(uint64_t epoch = 0);
          std::vector<std::vector<uint32_t>> probe(uint64_t epoch = 0);

    private:
    };
}

#endif //LIBMONEY_PRIME_PROBE_HPP
