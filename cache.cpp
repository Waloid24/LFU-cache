#include "cache.hpp"

int main(void)
{
    using key_t = int;
    using page_t = key_t;

    std::size_t cache_size;
    std::cin >> cache_size;

    LfuCache<key_t, page_t> my_cache{cache_size};

    std::cout << "Hello!" << std::endl;

    return 0;
}