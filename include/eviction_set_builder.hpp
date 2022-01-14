//
// Created by rf on 1/11/22.
//

#ifndef LIBCACHEMONEY_INCLUDE_EVICTION_SET_BUILDER_HPP
#define LIBCACHEMONEY_INCLUDE_EVICTION_SET_BUILDER_HPP

#include <vector>
#include <cstdint>
#include <elem.hpp>

struct eviction_set_element {
  uint32_t set;
  uint64_t time;
};

class eviction_set_builder {

public:
  /// Implementation of "Theory and Practice of Finding Eviciton Sets" proposed Test 3
  /// \param S the eviction set S
  /// \param a the amount of
  /// \param threshold the minimum time
  /// \return true if S is an eviction set, false otherewise
  bool test(elem<eviction_set_element>*, size_t a, uint64_t threshold);


private:

};

#endif //LIBCACHEMONEY_INCLUDE_EVICTION_SET_BUILDER_HPP
