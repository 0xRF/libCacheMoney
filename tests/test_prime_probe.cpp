//
// Created by rf on 12/20/21.
//
#include <attacks/prime_probe.hpp>
#include <matplot/matplot.h>
#include <utils.hpp>
#include <cstdlib>
#include <l1.hpp>
#include <intrinsics.hpp>
#include <iostream>
#include <fstream>
#include <meta_cache.hpp>

#include <weak_alg.hpp>

using namespace cache_money;
using namespace cache;
using namespace std;

int main(void) {

    const uint64_t SAMPLES = prime_probe::SAMPLES;

    auto pp = prime_probe();

    victims::weak_alg alg;
    char arr[8] = "ABCDEFG";

    //4 in this weak alg, probs not correct but still
//    uint64_t slot = pp.find_initial_slot(4, [pAlg = &alg, arr] { pAlg->trigger((char*)arr); });
//    pp.probe([pAlg = &alg, arr] { pAlg->trigger((char*)arr); }, slot);

    return 0;
}