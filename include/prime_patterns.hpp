//
// Created by rf on 1/10/22.
//

#ifndef LIBCACHEMONEY_INCLUDE_PRIME_PATTERNS_HPP
#define LIBCACHEMONEY_INCLUDE_PRIME_PATTERNS_HPP

#include <string_literal.hpp>
#include <array>
#include <iostream>
#include <list>
#include <utils.hpp>
#include <eviction_set_builder.hpp>
#include <intrinsics.hpp>
template<string_literal lit>
class prime_pattern {

public:

  prime_pattern() {
//	constexpr std::string_view str = lit.value;
//	for (int i = str.find_last_of('_') + 2; i < str.length(); i++)
//	{
//	  if(str[i] == 'S')
//	}
  }

  //TODO figure out how to automatically and efficiepntly compile this to the correct prime pattern, but ngl seems like a pain
  // Implements the 6700k prime pattern ,R3_S4_P01SS2SS301230123
  static inline void acsess_prime_pattern(elem<eviction_set_element>* S , uint32_t length){
//	for(int r = 0; r < repeats(); r++)
//	  for(int i = 0; i < length; i++){
//		intrinsics::maccess(S);
//		intrinsics::maccess(S->next);
//
//
//	  }
  }


  static consteval int get_sizes() {
	constexpr std::string_view str = lit.value;
	auto pos = str.find_last_of('_');
	return str.length() - pos;
  }

  static consteval size_t repeats() {
	constexpr std::string_view str = lit.value;
	const auto sst2 = str.substr(1, str.find_first_of('_') - 1);
	return utils::strtoi(sst2.data());
  }
  static constexpr size_t strides() {
	constexpr std::string_view str = lit.value;
	auto pos = str.find_first_of('_') + 2;
	return utils::strtoi(str.substr(pos, str.find_last_of('_') - pos).data());
  }

private:

  std::array<int, get_sizes()> pattern{};

};
#endif //LIBCACHEMONEY_INCLUDE_PRIME_PATTERNS_HPP
