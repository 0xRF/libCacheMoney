#include <meta_cache.hpp>
#include <cstring>
#include <matplot/matplot.h>

using namespace cache_money;

int main() {

    const size_t ASSOC = 8;
    const size_t LINE_SIZE= 64;
    const size_t SIZE = 3*1024;

    typedef meta_cache<LINE_SIZE, ASSOC, SIZE> l1_cache;
    auto l1 = l1_cache();

    auto base = (uintptr_t)malloc(10*1024);
    uintptr_t buff = utils::get_aligned_address((uintptr_t) base, 10*1024);

    size_t set = utils::get_address_set(buff);
    l1.fill_set(set);  std::cout << "Should be quick: " << l1.measure(set) << std::endl;
    l1.flush_set(set); std::cout << "Should be slow: " << l1.measure(set) << std::endl;
    l1.fill_set(set);  std::cout << "Should be quick: " << l1.measure(set) << std::endl;
    l1.flush_set(set); std::cout << "Should be slow: " << l1.measure(set) << std::endl;
    l1.fill_set(set); std::cout << "Should be 0 misses : " << l1.misses(set) << std::endl;
    l1.flush_set(set); std::cout << "Should be 8 misses : " << l1.misses(set) << std::endl;


    return 0;
}