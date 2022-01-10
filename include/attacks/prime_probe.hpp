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
#include <functional>
#include "meta_cache.hpp"


namespace cache_money {
    class prime_probe {
    public:

        prime_probe();
        ~prime_probe();

        static const uint64_t SAMPLES = 100;

        /// Fills the entire cache with malicous data  and Returns the smallest speed of each cache set over N iterations
        /// \return
        std::vector<uint64_t> prime(uint64_t samples = 1000);

        /// Returns the amount of which each cache set has a misses
        /// \param trigger
        /// \param slotInitial
        /// \param slot
        /// \param epoch
        /// \return
        std::vector<uint32_t> probe(const std::function<void>& trigger, uint64_t slotInitial = 0, uint64_t slot = 0, uint64_t epoch = 0);

        /// Figure out the initial slot time through trial and error
        /// \param trigger
        /// \return
        [[nodiscard]] uint64_t find_initial_slot(int targetCacheSets, const std::function<void(void)>& trigger, uint32_t iterations=10000, uint32_t stepSize = 10);


    private:
        typedef meta_cache<64, 8, 3*1024> cache;
        cache m_cache;
    };
}

#endif //LIBMONEY_PRIME_PROBE_HPP
