#include <meta_cache.hpp>
#include <cstring>

using namespace cache_money;

int main() {

    typedef meta_cache<64, 8, 32 * 1024> l1_cache;

    auto l1 = l1_cache();

    uintptr_t base = (uintptr_t)malloc(10*1024);
    uintptr_t buff = utils::get_aligned_address((uintptr_t) base, 10*1024);
    if(buff > base + 10*1024)
        throw std::runtime_error("Fuck");

    size_t set = utils::get_address_set(buff);
    std::cout << l1.measure(set) << std::endl;
    l1.fill_set(set);
    std::cout << l1.measure(set) << std::endl;
    __builtin_prefetch((void*)buff);
    memset((void*)buff, 0xCC, 10);
    std::cout << l1.measure(set) << std::endl;



    return 0;
}