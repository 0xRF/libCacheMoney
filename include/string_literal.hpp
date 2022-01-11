//
// Created by rf on 1/10/22.
//

#ifndef LIBCACHEMONEY_INCLUDE_STRING_LITERAL_HPP
#define LIBCACHEMONEY_INCLUDE_STRING_LITERAL_HPP
#include <algorithm>

//https://ctrpeach.io/posts/cpp20-string-literal-template-parameters/
template<std::size_t N>
struct string_literal {
  constexpr string_literal(const char (&str)[N]) {
	std::copy_n(str, N, value);
  }
  char value[N];
};

#endif //LIBCACHEMONEY_INCLUDE_STRING_LITERAL_HPP
