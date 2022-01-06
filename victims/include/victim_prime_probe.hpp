#ifndef LIBCACHEMONEY_VICTIM_PRIME_PROBE_HPP
#define LIBCACHEMONEY_VICTIM_PRIME_PROBE_HPP

#include <cstdint>
#include <tuple>

namespace victims {


    class prime_probe_weak_alg {
    public:
        prime_probe_weak_alg();

        void trigger(char array[8]);
    private:
        uint8_t *m_sBox = nullptr;
//        CacheLine *m_lines = nullptr;
        std::array<int, 8> m_secretKey = {0, 1, 2, 3, 4, 5, 6, 7};
    };
}


#endif //LIBCACHEMONEY_VICTIM_PRIME_PROBE_HPP
