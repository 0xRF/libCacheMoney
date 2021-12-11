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

#ifndef LIBMONEY_FLUSH_RELOAD_HPP
#define LIBMONEY_FLUSH_RELOAD_HPP

#include <vector>
#include <cstdint>
#include <functional>
#include <optional>

namespace cache_money {
    class flush_reload {
    public:
        flush_reload(uintptr_t address);
        bool attack(uint64_t offset, std::optional<uint64_t> baseline = {}, std::function<void()> trigger = nullptr, std::optional<int64_t> iterations = {});
        std::vector<uint64_t> get_results();
        uint64_t get_baseline(uint64_t iterations = 10000);
    private:
        std::vector<uint64_t> m_results;
        uintptr_t m_address;
    };
}

#endif //LIBMONEY_FLUSH_RELOAD_HPP
