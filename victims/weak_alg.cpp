#include <random>
#include <cstdio>
#include <vector>
#include <iostream>
#include "include/weak_alg.hpp"
#include "../include/utils.hpp"


namespace victims {

    weak_alg::weak_alg() {
        m_sBox = (uint64_t *) malloc(64 * 8);
        std::shuffle(m_secretKey.begin(), m_secretKey.end(), std::mt19937(std::random_device()()));
        for (int i = 0; i < 8; i++) {
            m_sBox[i] = 3 << (7 - i) << i;
            std::cout << "s_box " << i << " Set: " << utils::get_address_set((uintptr_t)m_sBox+i*sizeof(uintptr_t)) << std::endl;
        }
        std::cout << "Secret Key Set: " << utils::get_address_set((uintptr_t)m_secretKey.data()) << std::endl;
    }

    void weak_alg::trigger(char array[8]) {
        for (int i = 0; i < 8; i++)
            array[i] = array[i] ^ m_sBox[m_secretKey[i]];
    }
}