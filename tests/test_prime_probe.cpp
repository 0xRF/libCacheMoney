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

#include <victim_prime_probe.hpp>

using namespace cache_money;
using namespace cache;
using namespace std;

int main(void) {

    const uint64_t SAMPLES = prime_probe::SAMPLES;

    prime_probe pp;
    uint64_t epoch = intrinsics::rdtscp64();

    cout << "Starting Probe" << endl;
    auto memtimes = pp.probe(epoch);

    std::vector<std::vector<double>> mapped{};
    for (auto times: memtimes) {
        auto doubs = std::vector<double>{};
        for (auto time: times) {
            doubs.push_back((double) time);
        }
        mapped.push_back(doubs);
    }

    uint64_t fin = intrinsics::rdtscp64();
    cout << "Finished Probe" << endl;
    uint64_t speed = l1::speed();

    victims::prime_probe_weak_alg alg;
    char arr[8] = "ABCDEFG";
    alg.trigger(arr);

//    matplot::hold(matplot::on);
//    matplot::scatter(mapped);
    matplot::ylim({0, 64});
    matplot::xlim({0, static_cast<double>(fin - epoch)});


    matplot::show();


    return 0;
}