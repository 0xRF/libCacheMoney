//
// Created by rf on 1/20/22.
//

#ifndef LIBCACHEMONEY_INCLUDE_EVSETS_PS_EVSET_HPP
#define LIBCACHEMONEY_INCLUDE_EVSETS_PS_EVSET_HPP

#include "../linked_list.hpp"
#include <vector>

#define THRESHOLD 203
#define MAX_RETRIES 32

linked_list set_create(linked_list& guessPool, uintptr_t victim);
bool set_add(linked_list &set, linked_list& guessPool, uintptr_t victim);
bool set_valid(linked_list &set, uintptr_t victim);
bool set_reduce(linked_list &set, linked_list& guessPool, uintptr_t victim);

linked_list brute_force_set(linked_list& guessPool, uintptr_t victim);

#endif //LIBCACHEMONEY_INCLUDE_EVSETS_PS_EVSET_HPP
