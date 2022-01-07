//
// Created by rf on 12/20/21.
//
#include <attacks/prime_probe.hpp>
#include <utils.hpp>
#include <cstdlib>
#include <l1.hpp>
#include <intrinsics.hpp>
#include <iostream>
#include <fstream>

using namespace cache_money;
using namespace cache;

int main(void) {
//
//    size_t array_size = 1024 * 1024;
//    auto buffer = (uintptr_t) malloc(array_size);
//    auto address = utils::get_aligned_address(buffer, array_size);
//    prime_probe pp;
//    auto mapped = pp.generate_mapped_addresses(address, buffer, array_size);
//
//    const uint64_t SAMPLES = 10000;
//    auto times = new std::array<std::array<std::array<uint32_t, 8>, 64>, SAMPLES>();
//
//    for (int i = 0; i < SAMPLES; i++) {
//        for (uint64_t set = 0; set < l1::set_count(); set++)
//            for (uint64_t block = 0; block < l1::assoc(); block++)
//                (*times)[i][set][block] = intrinsics::memaccesstime(mapped[set][block]);
//
//        utils::cycle_wait(10);
//    }
//
//    for (int i = 0; i < SAMPLES; i++) {
//        std::fstream file;
//        file.open("samples/sample_" + std::to_string(i) + ".dat", std::ios_base::out);
//        for (uint64_t block = 0; block < l1::assoc(); block++) {
//            for (uint64_t set = 0; set < l1::set_count(); set++) {
//                file << (*times)[i][set][block] << (set < l1::set_count() - 1 ? " " : "");
//            }
//            file << std::endl;
//        }
//        file.close();
//    }
    return 0;
}