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

        prime_probe();
        ~prime_probe();

        static const uint64_t SAMPLES = 100;
//        std::array<std::array<std::array<std::pair<uint32_t, uint64_t>, 8>, 64>, SAMPLES> *probe();
//        std::pair<std::vector<uint32_t>, std::vector<uint64_t>> probe(uint64_t epoch = 0);
        std::vector<double> prime();
        std::vector<bool> probe(const std::vector<double> &minTimes, uint64_t slotInitial = 0, uint64_t slot = 0, uint64_t epoch = 0);

    private:
        void generate_mapped_addresses();

        std::vector<std::vector<uintptr_t>> m_mapped;
        uintptr_t m_buffer;
        size_t m_buffer_size;
    };
}

#endif //LIBMONEY_PRIME_PROBE_HPP
