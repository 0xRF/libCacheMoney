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

#include <victim_prime_probe.hpp>

using namespace cache_money;
using namespace cache;
using namespace std;

int main(void) {

    const uint64_t SAMPLES = prime_probe::SAMPLES;

    prime_probe *pp = new prime_probe;
    pp->prime();
    uint64_t epoch = intrinsics::rdtscp64();
//    for(register int i asm("r12") = 0; i< 1000; i++) {
    auto primed = pp->prime();

    auto memed = pp->probe(primed, 10, 10);

    for (uint64_t set = 0; set < l1::set_count(); set++)
        for (uint64_t block = 0; block < l1::assoc(); block++)
            if (memed[set + block * l1::set_count()])
                std::cout << "Memed " << block << " " << set << std::endl;

    std::cout << "Cache Lines Memed " << std::count(memed.begin(), memed.end(), true) << std::endl;

//    victims::prime_probe_weak_alg alg;
    char arr[8] = "ABCDEFG";
//    alg.trigger(arr);

//    matplot::hold(matplot::on);
//    matplot::scatter(mapped);
//    matplot::ylim({0, 64});
//    matplot::xlim({0, static_cast<double>(fin - epoch)});

//    matplot::show();

    delete pp;

    return 0;
}