//
// Created by rf on 1/10/22.
//

#include <prime_patterns.hpp>
#include <elem.hpp>
#include <cassert>

struct address{
uint32_t set;
uintptr_t add;
};

int main() {
  std::list<int> aa;

  auto a = aa.begin();

  std::cout << sizeof(std::list<std::pair<uint64_t,uint64_t>>) << std::endl;
  std::cout << sizeof(elem<address>) << std::endl;
  auto prime = prime_pattern<"R11_S12_P01S2">();
  auto prime2 = prime_pattern<"R3133_S1332_P01S2">();

  assert(3133==prime2.repeats());
  assert(1332==prime2.strides());

  return 0;
}