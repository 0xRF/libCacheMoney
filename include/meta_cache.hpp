#ifndef LIBCACHEMONEY_META_CACHE_HPP
#define LIBCACHEMONEY_META_CACHE_HPP

#include <cstdint>
#include <memory>
#include <cstring>

namespace cache_money {

    template<size_t SET_COUNT>
    struct CacheLine_t {
    private:
        uint8_t bytes[SET_COUNT];
    } __attribute__((aligned));

    //A class to represent a cache structure and provide useful properities
    // for mapping addresses
    // represents the cache but has no true 'relation to it hence meta
    template<std::size_t LINE_SIZE, std::size_t ASSOC, std::size_t SIZE>
    class meta_cache {
    public:
        meta_cache() {
        }

        typedef CacheLine_t<LINE_SIZE> CacheLine;

        const std::array<CacheLine, ASSOC> &get_set(uint32_t set) {
            //
        }

        const CacheLine &get_line(uint32_t lineIndex);

        static constexpr size_t get_size() { return SIZE; }

        static constexpr size_t get_assoc() { return ASSOC; }

        static constexpr size_t get_line_size() { return LINE_SIZE; }

        static constexpr size_t get_set_count() { return get_size() / get_set_size(); }

        static constexpr size_t get_set_size() { return get_assoc() * get_line_size(); }

        void load_line(uint64_t lineIndex);

        void load_address_line(uintptr_t address);

    private:
        CacheLine  m_mapped[get_set_count()*get_assoc()];
    };
}

#endif //LIBCACHEMONEY_META_CACHE_HPP
