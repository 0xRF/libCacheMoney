#include <cstdint>
#include <iostream>

#include <utils.hpp>
#include "l3.hpp"

using namespace cache;
int main() {

  uintptr_t arr[16] = {
      0x7f80c9fd4240, 0x7f80ca43a240, 0x7f80ca89a240, 0x7f80ca8b5240,
      0x7f80ca8e5240, 0x7f80cab14240, 0x7f80cab50240, 0x7f80c4c5e240,
      0x7f80c4c7b240, 0x7f80cafde240, 0x7f80c4732240, 0x7f80c478a240,
      0x7f80c4cfb240, 0x7f80c56fd240, 0x7f80c57e0240, 0x7f80c618e240};

  for (int i = 0; i < 16; i++) {
    uintptr_t physicalAdress2 = utils::get_physical_address((uintptr_t)arr[i]);
    std::cout << " SET: " << cache::l3::get_physical_cache_set(physicalAdress2);
    std::cout << " SLICE: " << cache::l3::get_physical_slice(physicalAdress2) << std::endl;
  }
  return 0;
}
