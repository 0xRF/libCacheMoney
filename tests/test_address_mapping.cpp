//A test case for the virtual address to physical address

#include <virt_to_phys.hpp>
#include <l3.hpp>
#include <utils.hpp>
#include <cassert>
#include <limits>
#include <iostream>

int main() {
  int meme = 0;
  uintptr_t virtualAddress = (uintptr_t)&meme;

  uintptr_t physicalAddress_A = utils::get_physical_address(virtualAddress);
  uintptr_t physicalAddress_B = vtop(getpid(), virtualAddress);

  printf("A:  0x%lx - B: 0x%lx\n", physicalAddress_A, physicalAddress_B);



//  assert(physicalAddress_A==physicalAddress_B);

  return 0;
}
