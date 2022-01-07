#ifndef LIBCACHEMONEY_META_CACHE_HPP
#define LIBCACHEMONEY_META_CACHE_HPP

#include <cstdint>
#include <memory>
#include <cstring>
#include <iostream>
#include "intrinsics.hpp"
#include <vector>
#include "utils.hpp"

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
            generate_mapped_addresses();
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

        inline void fill_set(uint32_t set) {
            for (int block = 0; block < get_assoc(); block++)
                *(uintptr_t *) m_mapped[set + block * get_set_count()] = 0;
//                __builtin_prefetch((void *) m_mapped[set + block * get_set_count()]);
        }


    private:

        void generate_mapped_addresses() {
            auto address = utils::get_aligned_address((uintptr_t) m_buffer, m_buffer_size);

            m_mapped = std::vector<uintptr_t>();
            m_mapped.reserve(get_set_count() * get_assoc());

            for (size_t cache_set = 0; cache_set < get_set_count(); cache_set++)
                for (size_t block = 0; block < get_assoc(); block++)
                    m_mapped[cache_set + block * get_set_count()] = 0;

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

        //Flat 2D array of addresses which map to cache sets
        std::vector<uintptr_t> m_mapped;
        cache_line *m_buffer;
        uintptr_t m_start_address;
        uintptr_t m_end_address;
        size_t m_buffer_size;
    };
}

#endif //LIBCACHEMONEY_META_CACHE_HPP
