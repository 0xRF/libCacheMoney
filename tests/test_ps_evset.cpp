#include <evsets/ps_evset.hpp>
#include <random>
#include <algorithm>
#include <chrono>
#include <cstring>
#include <linked_list.hpp>
#include <virt_to_phys.hpp>
#include <random>
#include <l3.hpp>

using namespace cache;

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
  uint64_t bufferSize = EVICT_LLC_SIZE - (bufferStart - (uintptr_t)buffer);

  linked_list guessPool = {};

  for (size_t i = 0; i < bufferSize/l3::get_page_size() - l3::get_page_size()*2; i++)
	list_push_back(&guessPool, (node *)(bufferStart + i*l3::get_page_size()));

  time_point t1 = std::chrono::high_resolution_clock::now();

  auto set = brute_force_set(guessPool, target);
  intrinsics::maccess::double_fenced(target);
  intrinsics::maccess::double_fenced(target);
  printf("Got the set is it valid %d\n", set_valid(set, target));
  print_eviction_set_information(&set);

  time_point t2 = high_resolution_clock::now();
  milliseconds ms = duration_cast<milliseconds>(t2 - t1);
  printf("time: %ld ms\n", ms.count());

  return 0;
}
