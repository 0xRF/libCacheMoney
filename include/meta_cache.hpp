#ifndef LIBCACHEMONEY_META_CACHE_HPP
#define LIBCACHEMONEY_META_CACHE_HPP

#include <cstdint>
#include <memory>
#include <cstring>
#include <iostream>
#include "intrinsics.hpp"
#include <vector>
#include "utils.hpp"
#include <limits>

namespace cache_money {

    template<size_t SET_COUNT>
    struct cache_line_t {
    private:
        uint8_t bytes[SET_COUNT];
    } __attribute__((aligned));

    //A class to represent a cache structure and provide useful properities
    // for mapping addresses
    // represents the cache but has no true 'relation to it hence meta
    template<std::size_t LINE_SIZE, std::size_t ASSOC, std::size_t SIZE>
    class meta_cache {
    public:

        typedef cache_line_t<LINE_SIZE> cache_line;

        meta_cache() {
            m_buffer_size = sizeof(cache_line) * get_set_count() * get_assoc() * 30;
            m_buffer = (cache_line *) malloc(m_buffer_size);
            m_baseline = 0;
            generate_mapped_addresses();
            generate_baseline_speed();
        }

        static constexpr size_t get_size() { return SIZE; }
        static constexpr size_t get_assoc() { return ASSOC; }
        static constexpr size_t get_line_size() { return LINE_SIZE; }
        static constexpr size_t get_set_count() { return get_size() / get_set_size(); }
        static constexpr size_t get_set_size() { return get_assoc() * get_line_size(); }
        inline uint64_t measure(uint32_t set) {
            uint64_t max = 0;
            for (int block = 0; block < get_assoc(); block++) {
                uint64_t time = intrinsics::memaccesstime(m_mapped[set + block * get_set_count()]);
                if (time > max)
                    max = time;
            }
            return max;
        }

        std::vector<std::vector<uint64_t>> get_cache_timings(uint64_t cycleSpan = 10000000000) {
            uint64_t epoch = intrinsics::rdtscp64();
            std::vector<std::vector<uint64_t>> mapped{};
            uint64_t time = 0;

            while (time < cycleSpan) {
                auto iteration = std::vector<uint64_t>();
                for (size_t set = 0; set < get_set_count(); set++) {
                    iteration[set] = measure(set);
                }
                time = intrinsics::rdtscp64() - epoch;
                mapped.emplace_back(iteration);
            }
            return mapped;
        }

        inline std::uint32_t misses(uint32_t set, uint64_t baseline = 0) {
            if (baseline == 0)
                baseline = m_baseline;

            uint64_t count = 0;
            for (int block = 0; block < get_assoc(); block++) {
                uint64_t time = intrinsics::memaccesstime(m_mapped[set + m_sbox[block] * get_set_count()]);
                if (time > baseline + 40)
                    count++;
                else
                {

                }
            }
            return count;
        }
        void flush_cache() {
            for (int set = 0; set < get_set_count(); set++)
                flush_set(set);
        }
        void fill_cache() {
            for (int set = 0; set < get_set_count(); set++)
                fill_set(set);
        }
        inline void flush_set(uint32_t set) {
            for (int block = 0; block < get_assoc(); block++)
                intrinsics::clflush(m_mapped[set + m_sbox[block] * get_set_count()]);
        }
        inline void fill_set(uint32_t set) {
            for (int block = 0; block < get_assoc(); block++)
                *(uintptr_t *) m_mapped[set + m_sbox[block] * get_set_count()] = 0;
        }

    private:
        //TODO cleanup this code
        void generate_mapped_addresses() {
            auto address = utils::get_aligned_address((uintptr_t) m_buffer, m_buffer_size);

            m_mapped = std::vector<uintptr_t>(get_set_count() * get_assoc());

            for (size_t cache_set = 0; cache_set < get_set_count(); cache_set++) {
                uintptr_t ptr = address;
                auto tags = std::vector<uint64_t>();
                for (size_t block = 0; block < get_assoc(); block++) {
                    auto tag = utils::get_address_tag(ptr);
                    while (utils::get_address_set(ptr) != cache_set ||
                           (find(tags.begin(), tags.end(), tag) != tags.end())) {
                        ptr += sizeof(uintptr_t);
                        tag = utils::get_address_tag(ptr);
                        if (ptr > (uintptr_t) m_buffer + m_buffer_size)
                            throw std::runtime_error("Out of bounds");
                    }
                    m_mapped[cache_set + block * get_set_count()] = ptr;
                    tags.push_back(tag);
                    ptr += sizeof(uintptr_t);
                }
            }
            if (std::count(m_mapped.begin(), m_mapped.end(), 0) != 0)
                throw std::runtime_error("Could find all the required addresses");
        }
        void generate_baseline_speed(uint64_t iterations = 10000) {
            m_baseline = std::numeric_limits<uint64_t>::max();
            for (size_t i = 0; i < iterations; i++) {
                for (size_t cache_set = 0; cache_set < get_set_count(); cache_set++)
                    for (int block = 0; block < get_assoc(); block++) {
                        uint64_t time = intrinsics::memaccesstime(m_mapped[cache_set + block * get_set_count()]);
                        if (time < m_baseline)
                            m_baseline = time;
                    }
            }
        }

        //Flat 2D array of addresses which map to cache sets
        std::vector<uintptr_t> m_mapped;
        std::array<size_t, 8> m_sbox = {7, 1, 6, 3, 0, 5, 2, 4};
        cache_line *m_buffer;
        size_t m_buffer_size;
        uint64_t m_baseline;
    };
}

#endif //LIBCACHEMONEY_META_CACHE_HPP


/// Get the l1 cache m_baseline speed per line and store it in a vector
/// \param iterations
/// \return
//        std::vector<std::vector<double>> get_baseline_speed(uint64_t iterations = 10000) {
//            auto cache = std::vector<std::vector<double>>(get_set_count(),
//                                                          std::vector<double>(get_assoc(),
//                                                                              std::numeric_limits<double>::max()));
//            for (size_t i = 0; i < iterations; i++) {
//                for (size_t cache_set = 0; cache_set < get_set_count(); cache_set++)
//                    for (int block = 0; block < get_assoc(); block++) {
//                        uint64_t time = intrinsics::memaccesstime(m_mapped[cache_set + block * get_set_count()]);
//                        if ((double) time < cache[cache_set][block])
//                            cache[cache_set][block] = (double) time;
//                    }
//            }
//            return cache;
//        }
