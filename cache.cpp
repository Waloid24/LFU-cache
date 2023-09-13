#include "cache.hpp"

int slow_get_page_int(int key) { return key; }

int main(void)
{
    using key_t = int;
    using page_t = key_t;

    int hits = 0;
    
    int n;
    size_t cache_size;

    std::cout << "Cache size: ";
    std::cin >> cache_size;
    assert(std::cin.good());

    std::cout << "Number of requests: ";
    std::cin >> n;
    assert(std::cin.good());

    caches::LfuCache<key_t, page_t> my_cache{cache_size};

    for (int i = 0; i < n; i++)
    {
        int rq;
        std::cin >> rq;
        assert(std::cin.good());
        if (my_cache.lookup_update(rq, slow_get_page_int))
        {
            hits += 1;
        }
    }

    std::cout << "HITS: " << hits << std::endl;
    return 0;
}