
#include <evsets/eviction_sets.hpp>
#include <array>
#include <random>
#include <algorithm>

//L1/L2    : 43
//LLC      : 111
//RAM      : 250
//THRESHOLD: 203

#include <evsets/eviction_set_l3.hpp>
#include <chrono>
#include <cstring>
#include "linked_list.hpp"
#include <virt_to_phys.hpp>
#include <random>

int main() {

  using namespace std::chrono;
  typedef high_resolution_clock::time_point time_point;

  const static uint64_t SMALL_PAGE_PERIOD = 1 << 12;
  const static uint64_t EVICT_LLC_SIZE = 128*1024*1024;
  const static size_t POOL_SIZE = EVICT_LLC_SIZE/SMALL_PAGE_PERIOD;

  void *buffer = malloc(EVICT_LLC_SIZE);
  memset(buffer, 0xCC, EVICT_LLC_SIZE);

  //Let the start address be one page in
  uintptr_t bufferStart = utils::get_page_start((uintptr_t)buffer) + l3::get_page_size();

  uintptr_t target = bufferStart + l3::get_page_size()*(rand()%10);

  time_point t1 = std::chrono::high_resolution_clock::now();

  auto set = eviction_set_l3::set_create(bufferStart, EVICT_LLC_SIZE - (bufferStart - (uintptr_t)buffer), target);

  print_eviction_set_information(&set);

  time_point t2 = high_resolution_clock::now();
  milliseconds ms = duration_cast<milliseconds>(t2 - t1);
  std::cout << ms.count() << " ms\n";

  return 0;
}