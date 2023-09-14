#include "cache.hpp"
#include <cassert>

int slow_get_page_int(int key) { return key; }

int main(void)
{
    using key_t = int;
    using page_t = key_t;
    
    int n;
    size_t cache_size;

    std::cin >> cache_size;
    assert(std::cin.good());

    std::cin >> n;
    assert(std::cin.good());

    caches::Lfu_Cache<key_t, page_t> my_cache{cache_size, slow_get_page_int};

    int hits = 0;
    for (int i = 0; i < n; i++)
    {
        int rq;
        std::cin >> rq;
        assert(std::cin.good());
        hits += my_cache.lookup_update(rq);
    }

    std::cout << hits << std::endl;
    return 0;
}
