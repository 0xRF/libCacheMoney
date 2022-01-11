//
// Created by rf on 1/11/22.
//

#ifndef LIBCACHEMONEY_INCLUDE_LINKED_LIST_HPP
#define LIBCACHEMONEY_INCLUDE_LINKED_LIST_HPP

#include <cstdint>

template<typename T>
struct alignas(64) elem {
  elem *prev;
  elem *next;
  T value;
};

#endif //LIBCACHEMONEY_INCLUDE_LINKED_LIST_HPP
