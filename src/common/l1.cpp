//
// Created by rf on 12/14/21.
//

#include <intrinsics.hpp>
#include <l1.hpp>
#include <utils.hpp>
#include <vector>

namespace cache::l1 {

    uint64_t speed(uint64_t iterations) {
        auto times = std::vector<uint64_t>();
        auto pTest = new uintptr_t;
        uintptr_t page_start = utils::get_page_start((uintptr_t) pTest);

        for (uint64_t i = 0; i < iterations; i++) {
            uint64_t time = intrinsics::memaccesstime(page_start);
            times.push_back(time);
        }
        delete pTest;
        return *std::min_element(times.begin(), times.end());
    }

    size_t size() { return 32 * 1024; }
    size_t line_size() { return 64; } // always 64 bytes wide
    size_t assoc() { return 8; }
    size_t set_size() { return assoc() * line_size(); }
    size_t set_count() { return size() / set_size(); }

    bool is_start_of_cache_line(uintptr_t phy_address) {
        return (phy_address & 0xFF) % line_size() == 0;
    }


} // namespace cache::l1
