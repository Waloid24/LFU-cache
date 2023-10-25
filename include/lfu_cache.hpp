#ifndef LFU_CACHE_HPP
#define LFU_CACHE_HPP
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <list>
#include <functional>
namespace caches {
    template<typename Key_t, typename Page_t>class Lfu_Cache final {
        struct Frq_List;
        struct Val_List;

        using frq_list_iter = typename std::list<Frq_List>::iterator;
        using val_list_iter = typename std::list<Val_List>::iterator;
        using ht_iter       = typename std::unordered_map<Key_t, val_list_iter>::iterator;
        using page_getter   = std::function<Page_t (const Key_t&)>;
        
        struct Val_List {
            Key_t key_;
            Page_t page_;
            frq_list_iter frql_it_;
            Val_List(Key_t key, Page_t page, frq_list_iter frql_it) : key_{key}, page_{page}, frql_it_{frql_it}{};
        };
        struct Frq_List {
            int frq_;
            std::list<Val_List> vall_;
            Frq_List(int frq) : frq_(frq){}
        };
        size_t size_;
        page_getter slow_get_page_;
        
        std::list<Frq_List> frql_;
        std::unordered_map<Key_t, val_list_iter> ht_; 
        public:
            Lfu_Cache(size_t size, page_getter slow_get_page) : size_{std::max(size_t{1}, size)}, slow_get_page_{slow_get_page} {}
            bool full() const { 
                return (ht_.size() == size_);
            }
            bool lookup_update(const Key_t &key) {
                auto hit = ht_.find(key);
                if (hit == ht_.end())
                {
                    insert_page(key);
                    return false;
                }
                else
                {
                    move_page(hit);
                    return true;
                }
            }
            
        private:
            void insert_page(const Key_t &key)
            {
                frq_list_iter frst_frql_elem_it = frql_.begin();
                
                if (frst_frql_elem_it == frql_.end())
                {
                    Frq_List &new_frql_elem = frql_.emplace_back(1); 
                    new_frql_elem.vall_.emplace_back(key, slow_get_page_(key), frql_.begin());
                    ht_.emplace(key, new_frql_elem.vall_.begin());
                    return;
                }
                auto &lfu = frst_frql_elem_it->vall_;
                if (full())
                {
                    ht_.erase(ht_.find(lfu.begin()->key_));
                    lfu.pop_front();
                }
                lfu.emplace_front(key, slow_get_page_(key), frst_frql_elem_it);
                ht_.emplace(key, lfu.begin());
                #if 0
                #endif
            }
            void move_page(ht_iter hit)
            {
                val_list_iter   val_list_elem_it    = hit->second;
                auto            this_frq            = hit->first;
                frq_list_iter   this_frql_it        = val_list_elem_it->frql_it_;
                frq_list_iter   next_frql_iter  = std::next(this_frql_it);
                
                if (next_frql_iter == frql_.end() || next_frql_iter->frq_ != this_frq + 1)
                {
                    frql_.emplace(next_frql_iter, this_frq+1);
                    next_frql_iter = std::next(this_frql_it);
                }
                auto &next_val_list = next_frql_iter->vall_;
                next_val_list.splice(next_val_list.end(), this_frql_it->vall_, val_list_elem_it);
                val_list_elem_it->frql_it_ = next_frql_iter;
                if (this_frql_it->vall_.empty())
                {
                    frql_.erase(this_frql_it);
                }
            }
    };
}
#endif
