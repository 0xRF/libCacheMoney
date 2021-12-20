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

#include <vector>
#include <cstdint>
#include <functional>
#include <cache.hpp>
#include <l1.hpp>
#include <optional>

namespace cache_money {
    class prime_probe {
    public:
        static inline auto generate_mapped_addresses(uintptr_t address, uintptr_t true_start, size_t array_size) {
            auto cache_sets = std::array<std::array<uintptr_t, 8>, 64>();

            for (size_t cache_set = 0; cache_set < cache::l1::set_count(); cache_set++) {
                uintptr_t ptr = address;
                auto tags = std::vector<uint64_t>();
                for (size_t block = 0; block < cache::l1::assoc(); block++) {
                    auto tag = utils::get_address_tag(ptr);
                    while (utils::get_address_set(ptr) != cache_set ||
                           (std::find(tags.begin(), tags.end(), tag) != tags.end())) {
                        ptr += sizeof(uintptr_t);
                        tag = utils::get_address_tag(ptr);

                        if (ptr > true_start + array_size)
                            throw;
                    }
                    cache_sets[cache_set][block] = ptr;
                    tags.push_back(tag);
                    ptr += sizeof(uintptr_t);
                }
            }
            return cache_sets;
        }

    private:
    };
}

#endif //LIBMONEY_PRIME_PROBE_HPP
