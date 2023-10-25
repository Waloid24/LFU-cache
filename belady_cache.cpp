#include <iostream>
#include <stdexcept>
#include <vector>
#include "include/belady_cache.hpp"

int slow_get_page_int(int key) { return key; }

int main()
{
    using key_t = int;
    using page_t = key_t;
    using rq_iter = typename std::vector<key_t>::iterator;

    std::vector<key_t> rq_array;
    
    size_t cache_size;
    size_t n;

    std::cin >> cache_size;
    if (!std::cin.good())
    {
        std::abort();
    }
    std::cin >> n;
    if (!std::cin.good())
    {
        std::abort();
    }

    for (size_t i = 0; i < n; ++i)
    {
        key_t rq;
        std::cin >> rq;
        if (!std::cin.good())
        {
            std::abort();
        }

        rq_array.emplace_back(rq);
    }

    belady_caches::Belady_cache<key_t, page_t> my_cache{rq_array.begin(), rq_array.end(), cache_size, slow_get_page_int};

    int hits = 0;

    for (auto i : rq_array)
    {
        hits += my_cache.lookup_update(i);
    }

    std::cout << hits << std::endl;
    return 0;
}