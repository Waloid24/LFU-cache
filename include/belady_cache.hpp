#ifndef BELADY_CACHE_HPP
#define BELADY_CACHE_HPP

#include <iostream>
#include <functional>
#include <vector>
#include <list>
#include <unordered_map>
#include <map>

namespace belady_caches {
    template <typename Key_t>class Storage final {

        struct Key_List;

        using Key_iter      = typename std::list<Key_List>::iterator;
        using Occl_iter     = typename std::list<size_t>::iterator;
        using Ht_iter       = typename std::unordered_map<Key_t, Key_iter>::iterator;
        using Rq_iter       = typename std::vector<key_t>::iterator;

        struct Key_List {
            Key_t key_;
            std::list<size_t> occl_;

            Key_List(const Key_t &key) : key_(key){}
        };

        std::unordered_map<Key_t, Key_iter> ht_;
        std::list<Key_List> keyl_;
        Rq_iter start_;
        Rq_iter end_;

        public:

            Storage(Rq_iter start, Rq_iter end) : start_{start}, end_{end} {}

            void fill_storage()
            {
                size_t counter = 0;

                for (Rq_iter it = start_, ite = end_; it != ite; ++it, ++counter)
                {
                    Ht_iter place_in_ht = ht_.find(*it);
                    if (place_in_ht == ht_.end())
                    {
                        emplace_new_elem(*it, counter);
                    }
                    else
                    {
                        place_in_ht->second->occl_.emplace_back(counter);
                    }
                }
            }

            void emplace_new_elem(const Key_t &key, size_t counter)
            {
                keyl_.emplace_back(key);
                Key_iter emplaced_elem = std::prev(keyl_.end());
                emplaced_elem->occl_.emplace_back(counter);
                ht_.emplace(key, emplaced_elem);
            }

            void delete_occ(const Key_t &key)
            {
                Ht_iter place_in_ht = ht_.find(key);

                place_in_ht->second->occl_.pop_front();
                if (place_in_ht->second->occl_.empty())
                {
                    keyl_.erase(place_in_ht->second);
                    ht_.erase(place_in_ht);
                }
            }

            size_t check_num_meetings(const Key_t &key)
            {
                Ht_iter place_in_ht = ht_.find(key);
                size_t siiize = place_in_ht->second->occl_.size();
                return siiize;
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
                    return *(place_in_ht->second->occl_.begin());
                }
            }
    };

    template <typename Key_t, typename Page_t>class Belady_cache final {

        using Rq_iter       = typename std::vector<Key_t>::iterator;
        using Key_iter      = typename std::vector<std::pair<Key_t, Page_t>>::iterator;
        using Ht_iter       = typename std::unordered_map<Key_t, Key_iter>::iterator;
        using Page_getter   = std::function<Page_t (const Key_t&)>;

        std::vector<std::pair<Key_t, Page_t>>cache_;
        std::unordered_map<Key_t, Key_iter>ht_;
        Storage<Key_t> storage_;
        size_t capacity_;
        Page_getter slow_get_page_;

        public:

            Belady_cache(Rq_iter start, Rq_iter end, size_t capacity, Page_getter slow_get_page) : storage_{start, end}, capacity_{capacity}, slow_get_page_{slow_get_page}
            {
                storage_.fill_storage();
            }

            bool lookup_update(const Key_t &key)
            {
                Ht_iter place_in_ht = ht_.find(key);
                if (place_in_ht == ht_.end())
                {
                    emplace(key);
                    return false;
                }
                else
                {
                    return true;
                }
            }

            bool full()
            {
                return capacity_ == ht_.size();
            }

        private:

            void emplace(const Key_t &key) 
            {
                if (full() && (storage_.check_num_meetings(key) > 1))
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

            Key_iter find_elem_for_pop()
            {
                Key_iter elem_for_pop = cache_.begin();
                size_t meeting_place = 0;
                for (Key_iter it = cache_.begin(), ite = cache_.end(); it != ite; ++it)
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