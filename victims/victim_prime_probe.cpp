#include <random>
#include <cstdio>
#include <vector>
#include <iostream>
#include "include/victim_prime_probe.hpp"


namespace victims {

//    prime_probe_weak_alg::prime_probe_weak_alg() {
//        m_sBox = (uint8_t *) malloc(sizeof(cache_line) * 8);
//        m_lines = (cache_line *) m_sBox;
//        std::shuffle(m_secretKey.begin(), m_secretKey.end(), std::mt19937(std::random_device()()));
//        for (int i = 0; i < 8; i++) {
//            m_lines[i].bytes[0] = 7 - i;
//        }
//    }
//
//    void prime_probe_weak_alg::trigger(char array[8]) {
//        for (int i = 0; i < 8; i++)
//            array[i] = array[i] ^ m_lines[m_secretKey[i]].bytes[0];
//    }
}