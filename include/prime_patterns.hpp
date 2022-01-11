//
// Created by rf on 1/10/22.
//

#ifndef LIBCACHEMONEY_INCLUDE_PRIME_PATTERNS_HPP
#define LIBCACHEMONEY_INCLUDE_PRIME_PATTERNS_HPP

#include <string_literal.hpp>
#include <array>
#include <iostream>

constexpr int strtoi(const char *s) {
  int sum = 0;
  char ch;
  char sign = *s;
  if (*s=='-' || *s=='+')
	s++;
  while ((ch = *s++) >= '0' && ch <= '9') {
	sum = sum*10 - (ch - '0');
  }
  if (sign!='-') {
	sum = -sum;
  }
  return sum;
}
#include <list>
template<string_literal lit>
class prime_pattern {

public:

  prime_pattern() {
	constexpr std::string_view str = lit.value;
	for (int i = str.find_last_of('_') + 2; i < str.length(); i++)
	{
//	  if(str[i] == 'S')
	}
  }

  static consteval int get_sizes() {
	constexpr std::string_view str = lit.value;
	auto pos = str.find_last_of('_');
	return str.length() - pos;
  }

  static consteval size_t repeats() {
	constexpr std::string_view str = lit.value;
	const auto sst2 = str.substr(1, str.find_first_of('_') - 1);
	return strtoi(sst2.data());
  }
  static constexpr size_t strides() {
	constexpr std::string_view str = lit.value;
	auto pos = str.find_first_of('_') + 2;
	return strtoi(str.substr(pos, str.find_last_of('_') - pos).data());
  }

private:

  std::array<int, get_sizes()> pattern{};

};
#endif //LIBCACHEMONEY_INCLUDE_PRIME_PATTERNS_HPP