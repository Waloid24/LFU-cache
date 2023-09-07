#include "cache.hpp"

int main(void)
{
    using key_t = int;
    using page_t = key_t;

    int hits = 0;
    
    int n;
    std::size_t cache_size;

    std::cout << "Cache size: ";
    std::cin >> cache_size;
    assert(std::cin.good());

    std::cout << "Number of requests: ";
    std::cin >> n;
    assert(std::cin.good());

    caches::LfuCache<key_t, page_t> my_cache{cache_size};

    std::cout << hits << std::endl;
    return 0;
}