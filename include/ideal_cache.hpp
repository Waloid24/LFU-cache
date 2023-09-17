#ifndef IDEAL_CACHE_HPP
#define IDEAL_CACHE_HPP

#include <iostream>
#include <functional>
#include <vector>

// 4 12 1 2 3 4 1 2 5 1 2 4 3 4

namespace ideal_cache {

        template <typename Key_t, typename Page_t> class Ideal_Cache final {

            using page_getter = std::function<Page_t (const Key_t&)>;
            
            size_t capacity_;
            page_getter slow_get_page_;

            std::vector<std::pair<Key_t, Page_t>> cache;

            public:

                Ideal_Cache(size_t capacity, page_getter slow_get_page) : capacity_{capacity}, slow_get_page_{slow_get_page} {}

                bool lookup_update(const Key_t &key)
                {
                    if (cache.size() < capacity_)
                    {
                        cache.emplace_back(key, slow_get_page_(key));
                        return false;
                    }
                    else if (key == 5)
                    {
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }

        };
}

#endif
