#ifndef LIBCACHEMONEY_WEAK_ALG_HPP
#define LIBCACHEMONEY_WEAK_ALG_HPP

#include <cstdint>
#include <tuple>

namespace victims {


    class weak_alg {
    public:
        weak_alg();

        void trigger(char array[8]);
    private:
        uint64_t *m_sBox = nullptr;
        std::array<int, 8> m_secretKey = {0, 1, 2, 3, 4, 5, 6, 7};
    };
}


#endif //LIBCACHEMONEY_WEAK_ALG_HPP
