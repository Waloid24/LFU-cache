#ifndef BELADY_CACHE_HPP
#define BELADY_CACHE_HPP

#include <iostream>
#include <functional>
#include <vector>
#include <list>
#include <unordered_map>
#include <map>
#include <deque>

namespace belady_caches {
    template <typename Key_t>class Storage final {

        struct Key_List;

        std::unordered_map<Key_t, std::deque<size_t>> ht_;

        using Rq_iter = typename std::vector<key_t>::iterator;
        using Ht_iter = typename std::unordered_map<Key_t, std::deque<size_t>>::iterator;

        public:

            Storage(Rq_iter start, Rq_iter end) : ht_{} {

                size_t counter = 0;

                for (Rq_iter it = start, ite = end; it != ite; ++it, ++counter)
                {
                    Ht_iter place_in_ht = ht_.find(*it);
                    if (place_in_ht == ht_.end())
                    {
                        emplace_new_elem(*it, counter);
                    }
                    else
                    {
                        place_in_ht->second.emplace_back(counter);
                    }
                }

            }

            void emplace_new_elem(const Key_t &key, size_t counter)
            {
                ht_.emplace(key, 0);
                ht_[key].emplace_back(counter);

            }

            void delete_occ(const Key_t &key)
            {
                Ht_iter place_in_ht = ht_.find(key);

                place_in_ht->second.pop_front();
                if (place_in_ht->second.empty())
                {
                    ht_.erase(place_in_ht);
                }
            }

            size_t check_num_meetings(const Key_t &key)
            {
                Ht_iter place_in_ht = ht_.find(key);
                return place_in_ht->second.size();
            }

            size_t get_first_meeting_place(const Key_t &key)
            {
                Ht_iter place_in_ht = ht_.find(key);
                if (place_in_ht == ht_.end())
                {
                    return 0;
                }
                else
                {
                    return *(place_in_ht->second.begin());
                }
            }
    };

    template <typename Key_t, typename Page_t>class Belady_cache final {

        using Rq_iter       = typename std::vector<Key_t>::iterator;
        using Key_iter      = typename std::list<std::pair<Key_t, Page_t>>::iterator;
        using Ht_iter       = typename std::unordered_map<Key_t, Key_iter>::iterator;
        using Page_getter   = std::function<Page_t (const Key_t&)>;

        std::list<std::pair<Key_t, Page_t>>cache_;
        std::unordered_map<Key_t, Key_iter>ht_;

        Storage<Key_t> storage_;
        size_t capacity_;
        Page_getter slow_get_page_;

        public:

            Belady_cache(Rq_iter start, Rq_iter end, size_t capacity, Page_getter slow_get_page) : storage_{start, end}, capacity_{capacity}, slow_get_page_{slow_get_page} {}

            bool lookup_update(const Key_t &key)
            {
                Ht_iter place_in_ht = ht_.find(key);
                if (place_in_ht == ht_.end())
                {
                    insert_page(key);
                    return false;
                }
                else
                {
                    return true;
                }
            }

            bool full() const
            {
                return capacity_ == ht_.size();
            }

        private:

            void insert_page(const Key_t &key) 
            {
                if (storage_.check_num_meetings(key) > 1)
                {
                    if (full())
                    {
                        Key_iter extra_elem = find_elem_for_pop();
                        Key_t key = extra_elem->first;
                        ht_.erase(ht_.find(key));
                        cache_.erase(extra_elem);  
                    }
                    cache_.emplace_back(key, slow_get_page_(key));
                    ht_.emplace(key, std::prev(cache_.end()));
                    storage_.delete_occ(key);
                }
            }

            Key_iter find_elem_for_pop()
            {
                Key_iter elem_for_pop = cache_.begin();
                size_t meeting_place = 0;
                for (Key_iter it = elem_for_pop, ite = cache_.end(); it != ite; ++it)
                {
                    size_t first_meeting_place = storage_.get_first_meeting_place(it->first);
                    if (first_meeting_place == 0)
                    {
                        return it;
                    }
                    if (first_meeting_place > meeting_place)
                    {
                        elem_for_pop = it;
                        meeting_place = first_meeting_place;
                    }
                }
                return elem_for_pop;
            }
    };
}

#endif