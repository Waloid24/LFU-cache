#include "ideal_cache.hpp"

int slow_get_page_int(int key) { return key; }

int main(void)
{
    using key_t = int;
    using page_t = key_t;
    using rq_iter = typename std::vector<key_t>::iterator;

    // std::vector<key_t> rq_array = {100, 200, 300, 100, 800, 700, 500, 200, 300, 700};
    std::vector<key_t> rq_array = {1,1,1,1,1,1,1,1};
    ideal_cache::Ideal_Cache<key_t, page_t> my_cache{rq_array.begin(), rq_array.end(), 5, slow_get_page_int};

    int hits = 0;

    std::cout << "Before for in main" << std::endl;
    for (rq_iter rq = rq_array.begin(), rq_end = rq_array.end(); rq != rq_end; ++rq)
    {
        hits += my_cache.lookup_update(*rq);
    }

    std::cout << hits << std::endl;
    return 0;
}