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

#include <attacks/flush_reload.hpp>
#include <intrinsics.hpp>
#include <utils.hpp>

namespace cache_money {

    flush_reload::flush_reload(uintptr_t address)
            : m_address(address), m_results() {}

    std::vector<uint64_t> flush_reload::get_results() { return m_results; }

    bool flush_reload::attack(uint64_t offset, std::optional<uint64_t> baseline,
                              std::function<void()> trigger,
                              std::optional<int64_t> maxIterations) {
        m_results.clear();
        int index = 0;
        if (!baseline.has_value())
            baseline = utils::get_cache_baseline_speed();

        while (index < maxIterations || maxIterations == std::nullopt) {
            intrinsics::clflush(m_address);
            if (trigger)trigger();
            auto time = intrinsics::memaccesstime(m_address);
            m_results.push_back(time);
            if (time - offset < *baseline) {
                return true;
            }
            index++;
        }
        return false;
    }
} // namespace cache_money
