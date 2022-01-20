//
// Created by rf on 1/17/22.
//

#ifndef LIBCACHEMONEY_INCLUDE_EVSETS_EVICTION_SET_L3_HPP
#define LIBCACHEMONEY_INCLUDE_EVSETS_EVICTION_SET_L3_HPP

#include "../linked_list.hpp"
#include <vector>

namespace eviction_set_l3 {

#define L1_L2 42
#define LLC 111
#define RAM 250
#define THRESHOLD 203
#define MAX_RETRIES 128

linked_list set_create(uintptr_t bufferStart, size_t bufferSize, uintptr_t victim);
void set_add(linked_list &set, linked_list& guessPool, uintptr_t victim);
bool set_valid(linked_list &evset, uintptr_t victim);
void set_reduce(linked_list &set, linked_list& guessPool, uintptr_t victim);
};

#endif //LIBCACHEMONEY_INCLUDE_EVSETS_EVICTION_SET_L3_HPP
