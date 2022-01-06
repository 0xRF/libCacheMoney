//
// Created by rf on 12/20/21.
//

#include <attacks/prime_probe.hpp>
#include <functional>
#include <cache.hpp>
#include <l1.hpp>
#include <optional>
#include <utils.hpp>
#include <iostream>
#include <intrinsics.hpp>

using namespace cache_money;
using namespace cache;
using namespace std;

//array<array<uintptr_t, 8>, 64>
//prime_probe::generate_mapped_addresses (uintptr_t address, uintptr_t true_start, size_t array_size)
//{
//  auto cache_sets = array<array<uintptr_t, 8>, 64> ();
//
//  for (size_t set = 0; set < l1::set_count (); set++)
//	{
//	  uintptr_t ptr = address * l1::line_size ();
//	  for (size_t block = 0; block < l1::assoc (); block++)
//		{
//		  cout << utils::get_address_set (ptr + block * l1::) << endl;
//		}
//	}
//}
array<array<uintptr_t, 8>, 64>
prime_probe::generate_mapped_addresses(uintptr_t address, uintptr_t true_start, size_t array_size) {
    auto cache_sets = array<array<uintptr_t, 8>, 64>();

    for (size_t cache_set = 0; cache_set < cache::l1::set_count(); cache_set++) {
        uintptr_t ptr = address;
        auto tags = vector<uint64_t>();
        for (size_t block = 0; block < cache::l1::assoc(); block++) {
            auto tag = utils::get_address_tag(ptr);
            while (utils::get_address_set(ptr) != cache_set ||
                   (find(tags.begin(), tags.end(), tag) != tags.end())) {
                ptr += sizeof(uintptr_t);
                tag = utils::get_address_tag(ptr);

                if (ptr > true_start + array_size)
                    throw;
            }
            cache_sets[cache_set][block] = ptr;
            tags.push_back(tag);
            ptr += sizeof(uintptr_t);
        }
    }
    return cache_sets;
}

//array<array<array<pair<uint32_t, uint64_t>, 8>, 64>, prime_probe::SAMPLES>* prime_probe::probe() {
//
//    size_t array_size = 1024 * 1024;
//    auto buffer = (uintptr_t) malloc(array_size);
//    auto address = utils::get_aligned_address(buffer, array_size);
//    auto mapped = generate_mapped_addresses(address, buffer, array_size);
//    auto times = new array<array<array<pair<uint32_t, uint64_t>, 8>, 64>, SAMPLES>();
//    uint64_t epoch = intrinsics::rdtscp64();
//
//    for (int i = 0; i < SAMPLES; i++) {
//        for (uint64_t set = 0; set < l1::set_count(); set++)
//            for (uint64_t block = 0; block < l1::assoc(); block++)
//                (*times)[i][set][block] = {intrinsics::memaccesstime(mapped[set][block]), intrinsics::rdtscp64() - epoch};
//
//        utils::cycle_wait(10);
//    }
//    return times;
//}
//pair<vector<uint32_t>, vector<uint64_t>> prime_probe::probe(uint64_t epoch) {
//    if (epoch == 0)
//        epoch = intrinsics::rdtscp64();
//
//    size_t array_size = 1024 * 1024;
//    auto buffer = (uintptr_t) malloc(array_size);
//    auto address = utils::get_aligned_address(buffer, array_size);
//    auto mapped = generate_mapped_addresses(address, buffer, array_size);
//
//    auto memtimes = std::vector<uint32_t>(64 * 8 * SAMPLES);
//    auto cycles = std::vector<uint64_t>(64 * 8 * SAMPLES);
////            new array<uint32_t, 64 * 8 * prime_probe::SAMPLES>();
////    auto cycles = new array<uint64_t, 64 * 8 * prime_probe::SAMPLES>();
//
//
//    for (int i = 0; i < SAMPLES; i++) {
//        for (uint64_t set = 0; set < l1::set_count(); set++)
//            for (uint64_t block = 0; block < l1::assoc(); block++) {
//                (memtimes)[i + block * l1::assoc() * l1::set_count() +
//                           set * l1::set_count()] = intrinsics::memaccesstime(mapped[set][block]);
//                (cycles)[i + block * l1::assoc() * l1::set_count() + set * l1::set_count()] =
//                        intrinsics::rdtscp64() - epoch;
//            }
//
//        utils::cycle_wait(10);
//    }
//    return pair<vector<uint32_t>, vector<uint64_t>>(memtimes, cycles);
//}
vector<vector<uint32_t>> prime_probe::probe(uint64_t epoch) {
    if (epoch == 0)
        epoch = intrinsics::rdtscp64();

    size_t array_size = 1024 * 1024;
    auto buffer = (uintptr_t) malloc(array_size);
    auto address = utils::get_aligned_address(buffer, array_size);
    auto mapped = generate_mapped_addresses(address, buffer, array_size);
    auto timings = vector<vector<uint32_t>>(l1::set_count() * l1::assoc() * SAMPLES);

    for (int i = 0; i < SAMPLES; i++) {
        for (uint64_t set = 0; set < l1::set_count(); set++)
            for (uint64_t block = 0; block < l1::assoc(); block++) {
                timings[i][set * l1::set_count() + block] = intrinsics::memaccesstime(mapped[set][block]);
            }

        utils::cycle_wait(10);
    }
    return timings;
//    return pair<vector<uint32_t>, vector<uint64_t>>(memtimes, cycles);
}
