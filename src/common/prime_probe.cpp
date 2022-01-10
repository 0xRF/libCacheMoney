//
// Created by rf on 12/20/21.
//

#include <attacks/prime_probe.hpp>
#include <functional>
#include <cache.hpp>
#include <l1.hpp>
#include <optional>
#include <utils.hpp>
#include <iostream>
#include <intrinsics.hpp>
#include <numeric>
#include <algorithm>
#include <cstring>
#include <malloc.h>

using namespace cache_money;
using namespace cache;
using namespace std;

prime_probe::prime_probe() : m_cache() {}

std::vector<uint32_t>
prime_probe::probe(const std::function<void> &trigger, uint64_t slotInitial, uint64_t slot, uint64_t epoch) {

    auto victimSets = std::vector<uint32_t>();
    utils::cycle_wait(slotInitial);

    return victimSets;
}

vector<uint64_t> prime_probe::prime(uint64_t samples) {

    auto timings = std::vector<uint64_t>(cache::set_count());

    m_cache.fill_cache();

    for (size_t set = 0; set < cache::set_count(); set++)
        timings[set] = m_cache.measure(set);

    return std::move(timings);
}

prime_probe::~prime_probe() {
}

//This can be improved upon by taking a sample of the averages
uint64_t prime_probe::find_initial_slot(int targetCacheSets, const function<void(void)> &trigger, uint32_t iterations,
                                        uint32_t stepSize) {

    uint64_t slot = 0;
    uint64_t bestDistance = 0;
    uint64_t bestSlot = 0;

    for (int i = 0; i < iterations; i++) {
        int currMisses = 0;
        m_cache.fill_cache();
        utils::cycle_wait(slot);
        trigger();
        for (size_t set = 0; set < cache::set_count(); set++)
            currMisses += m_cache.misses(set);

        uint64_t distance = std::abs(targetCacheSets - currMisses);

        if (bestDistance > distance) {
            bestDistance = distance;
            bestSlot = slot;
        }
        slot += stepSize;
    }
    return bestSlot;
}

