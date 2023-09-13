#ifndef CACHE_HPP
#define CACHE_HPP

#include <iostream>
#include <unordered_map>
#include <cassert>
#include <list>
#include <algorithm>

namespace caches {

    template<typename Key_t, typename Page_t>class LfuCache final {

        size_t size_;
        struct frqList;
        using freq_list_iter = typename std::list<frqList>::iterator;

        struct valList {
            Key_t key_;
            Page_t page_;
            freq_list_iter frql_it_;    
        };

        struct frqList {
            int freq_;
            std::list<valList> vall_;

            explicit frqList(int freq) : freq_(freq){} // vall_ соберётся стандартным конструктором
        };

        public:

            LfuCache(size_t size) : size_(std::max(size_t{1}, size)){}

        private:

        using val_list_iter = typename std::list<valList>::iterator;

        std::list<frqList> frql_;
        std::unordered_map<Key_t, val_list_iter> ht_;     

            bool full() const { 
                return (ht_.size() == size_);
            }

            bool empty() const {
                return (ht_.size() == 0);
            }

        public:

            template <typename F> bool lookup_update(Key_t key, F slow_get_page) {

                auto hit = ht_.find(key);
                if (hit == ht_.end())
                {
                    std::cout << "Cache doesn't contain key [" << key << "]" << std::endl;

                    freq_list_iter itOfListFreqAtBegin = frql_.begin();     //итератор на начальный элемент в списке частот
                    
                    if (itOfListFreqAtBegin == frql_.end())         // хэш-таблица пустая
                    {
                        std::cout << "Hashtable is empty" << std::endl;
                        frqList &emplacedElem = frql_.emplace_back(1); 
                        emplacedElem.vall_.emplace_back(key, slow_get_page(key), itOfListFreqAtBegin);
                        ht_.emplace(key, emplacedElem.vall_.begin());
                        return false;
                    }

                    auto &val_list_with_low_freq = itOfListFreqAtBegin->vall_;

                    if (full())
                    {
                        std::cout << "Cache is full" << std::endl;

                        ht_.erase(ht_.find(val_list_with_low_freq.begin()->key_));
                        val_list_with_low_freq.pop_front();
                    }

                    val_list_with_low_freq.emplace_front(key, slow_get_page(key), itOfListFreqAtBegin);
                    ht_.try_emplace(key, val_list_with_low_freq.begin());

                    return false;
                }
                else
                {
                    std::cout << "Cache contains key [" << key << "]" << std::endl;

                    val_list_iter val_list_elem_iter    = hit->second; // итератор на элемент списка ValList
                    int freq_this_val_list              = hit->first;
                    freq_list_iter this_freq_list_iter  = val_list_elem_iter->frql_it_;
                    freq_list_iter next_freq_list_iter  = std::next(this_freq_list_iter);
                    
                    if (next_freq_list_iter == frql_.end() || next_freq_list_iter->freq_ != freq_this_val_list + 1)
                    {
                        if (next_freq_list_iter == frql_.end())
                            std::cout << "Next freq list item is NULL" << std::endl;
                        else
                            std::cout << "Next freq != this freq + 1" << std::endl;

                        frql_.emplace(next_freq_list_iter, freq_this_val_list+1);
                        next_freq_list_iter = std::next(this_freq_list_iter);
                    }

                    auto &next_val_list = next_freq_list_iter->vall_;

                    next_val_list.splice(next_val_list.end(), this_freq_list_iter->vall_, val_list_elem_iter);
                    val_list_elem_iter->frql_it_ = next_freq_list_iter;

                    if (this_freq_list_iter->vall_.empty())
                    {
                        frql_.erase(this_freq_list_iter);
                    }

                    return true;
                }

                
            }
            
    };

}

#endif