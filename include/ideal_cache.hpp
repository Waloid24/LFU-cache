// i.e. I know all the keys in input sequence at once
// 4 12 1 2 3 4 1 2 5 1 2 4 3 4

// <your_path>/LFU-cache/build$ cmake -DUSE_IDEAL_CACHE=ON ..
// <your_path>/LFU-cache/build$ make
// <your_path>/LFU-cache/build$ ./cache

#ifndef IDEAL_CACHE_HPP
#define IDEAL_CACHE_HPP

#include <iostream>
#include <functional>
#include <vector>
#include <list>
#include <unordered_map>
#include <map>

namespace ideal_cache {

        template <typename Key_t> class Key_Distr final {
            
            struct Val_List;
            struct Occ_List;    //occurrence

            using rq_iter   = typename std::vector<Key_t>::iterator;
            using vall_iter = typename std::list<Val_List>::iterator;
            using occl_iter = typename std::map<size_t, std::list<Val_List>>::iterator;
            using ht_iter   = typename std::unordered_map<Key_t, vall_iter>::iterator;

            rq_iter begin_;
            rq_iter end_;

            struct Val_List {
                Key_t key_;
                occl_iter occl_it_;
            };

            std::map<size_t, std::list<Val_List>> occl_;
            std::unordered_map<Key_t, vall_iter> ht_;

            public:

                Key_Distr(rq_iter begin, rq_iter end) : begin_{begin}, end_{end} {}

                size_t get_occurence(Key_t key)
                {
                    return ht_.find(key)->second->occl_it_->first;
                }

                void fill_storage()
                {
                    for (rq_iter it = begin_, ite = end_; it != ite; ++it)
                    {
                        Key_t key = *it;
                        std::cout << "Number of the query: " << key << std::endl;

                        ht_iter ht_iter_new_elem = ht_.find(key);
                        if (ht_iter_new_elem == ht_.end())
                        {
                            std::cout << "---------There ISN't this element in HASHTABLE--------" << std::endl;

                            occl_iter first_elem_in_occl;
                            if (occl_.begin()->first == 1)
                            {
                                first_elem_in_occl = occl_.begin();
                                std::cout << "| There IS an element with occurence = 1 in the OCCL |" << std::endl;
                            }
                            else
                            {
                                first_elem_in_occl = occl_.emplace(1, 0).first;
                                std::cout << "| There ISN'T an elem with occurence = 1 in the OCCL |" << std::endl;
                            }
                            
                            std::list<Val_List> &new_vall = first_elem_in_occl->second;
                            new_vall.emplace_back(key, first_elem_in_occl);
                            ht_.emplace(key, std::prev(new_vall.end()));

                            std::cout << "| The occl state:                                    |"   << std::endl;
                            std::cout << "| size = " << occl_.size() << "                               |" << std::endl;
                            std::cout << "|   The first element in occl:                       |"   << std::endl;
                            std::cout << "|   size = " << new_vall.size() << "                               |" << std::endl;
                            std::cout << "|   last element:                                    |"   << std::endl;
                            std::cout << "|     key = " << std::prev(new_vall.end())->key_ << "                               |" << std::endl;
                            std::cout << "| The hashtable state:                               |" << std::endl;
                            std::cout << "| size = " << ht_.size() << "                               |" << std::endl;
                            std::cout << "| elements:                                          |" << std::endl;
                            
                            for (ht_iter it = ht_.begin(), ite = ht_.end(); it != ite; ++it)
                            {
                                std::cout << "|   key = " << it->first << "                               |" << std::endl;
                                std::cout << "|   this element occurence = " << it->second->occl_it_->first << "                   |" << std::endl;
                            }

                            std::cout << "------------------------------------------------------" << std::endl;
                        }
                        else
                        {
                            std::cout << "----------There IS this element in HASHTABLE----------" << std::endl;
                            
                            vall_iter this_elem_in_vall = ht_iter_new_elem->second;
                            occl_iter this_elem_in_occl = this_elem_in_vall->occl_it_;
                            size_t    this_occ          = this_elem_in_occl->first;
                            occl_iter next_elem_in_occl = std::next(this_elem_in_occl);

                            std::cout << "| BEFORE MOVING:                                     |" << std::endl;
                            std::cout << "|   The occl state:                                  |" << std::endl;
                            std::cout << "|     size = " << this_elem_in_occl->first << std::endl;
                            std::cout << "|     occurence = " << this_occ << std::endl;
                            std::cout << "|     The vall state:                                |" << std::endl;
                            std::cout << "|       size = " << this_elem_in_occl->second.size() << std::endl;
                            std::cout << "|       elements:                                    |" << std::endl;
                            
                            for (vall_iter it = this_elem_in_occl->second.begin(), ite = this_elem_in_occl->second.end(); it!=ite; ++it)
                            {
                                std::cout << "|         key = " << it->key_ << std::endl;
                                std::cout << "|         this element occurence = " << it->occl_it_->first << std::endl;
                            }

                            std::cout << "| The hashtable state:                               |" << std::endl;
                            std::cout << "|   size = " << ht_.size() << "                               |" << std::endl;
                            std::cout << "|   elements:                                          |" << std::endl;
                            
                            for (ht_iter it = ht_.begin(), ite = ht_.end(); it != ite; ++it)
                            {
                                std::cout << "|     key = " << it->first << std::endl;
                                std::cout << "|     this element occurence = " << it->second->occl_it_->first << std::endl;
                            }

                            std::cout << "| MOVING:                                             |" << std::endl;

                            if (next_elem_in_occl == occl_.end() || next_elem_in_occl->first != this_occ + 1)
                            {
                                std::cout << "$$ should add next occurence: ";
                                if (next_elem_in_occl == occl_.end())
                                {
                                    std::cout << "next_elem_in_occl == occl_.end() $$" << std::endl;
                                }
                                else
                                {
                                    std::cout << "next_elem_in_occl->first != this_occ + 1 $$" << std::endl;
                                }
                                occl_.emplace(this_occ+1, 0);
                                next_elem_in_occl = std::next(next_elem_in_occl);

                                std::cout << "| The occl state:                                     "   << std::endl;
                                std::cout << "|   size = " << occl_.size()                              << std::endl;
                                std::cout << "|   elements:                                         "   << std::endl; 

                                for (occl_iter it = occl_.begin(), ite = occl_.end(); it!=ite; ++it)
                                {
                                    std::cout << "|     occurence = " << it->first << std::endl;
                                    std::cout << "|     The vall state:                                |" << std::endl;
                                    std::cout << "|       size = " << it->second.size() << std::endl;
                                }
                                

                                std::cout << "$$ new occurence of the occl next element " << next_elem_in_occl->first << " $$" << std::endl;
                            }


                            std::cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << std::endl;
                            std::cout << "| The hashtable state:                               |" << std::endl;
                            std::cout << "|   size = " << ht_.size() << std::endl;
                            std::cout << "|   elements:                                          |" << std::endl;
                            
                            for (ht_iter it = ht_.begin(), ite = ht_.end(); it != ite; ++it)
                            {
                                std::cout << "|     key = " << it->first << std::endl;
                                std::cout << "|     this element occurence = " << it->second->occl_it_->first << std::endl;
                            }
                            std::cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << std::endl;


                            next_elem_in_occl->second.splice(next_elem_in_occl->second.end(), this_elem_in_occl->second, this_elem_in_vall);


                            std::cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << std::endl;
                            std::cout << "| The hashtable state:                               |" << std::endl;
                            std::cout << "|   size = " << ht_.size() << std::endl;
                            std::cout << "|   elements:                                          |" << std::endl;
                            
                            for (ht_iter it = ht_.begin(), ite = ht_.end(); it != ite; ++it)
                            {
                                std::cout << "|     key = " << it->first << std::endl;
                                std::cout << "|     this element occurence = " << it->second->occl_it_->first << std::endl;
                            }
                            std::cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << std::endl;

                            ht_.find(key)->second->occl_it_ = next_elem_in_occl;
                            std::cout << "$$ ht_iter_new_elem->second->occl_it_ = " << ht_iter_new_elem->second->occl_it_->first << " $$" << std::endl;

                            std::cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << std::endl;
                            std::cout << "| The hashtable state:                               |" << std::endl;
                            std::cout << "|   size = " << ht_.size() << std::endl;
                            std::cout << "|   elements:                                          |" << std::endl;
                            
                            for (ht_iter it = ht_.begin(), ite = ht_.end(); it != ite; ++it)
                            {
                                std::cout << "|     key = " << it->first << std::endl;
                                std::cout << "|     this element occurence = " << it->second->occl_it_->first << std::endl;
                            }
                            std::cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << std::endl;

                            if (this_elem_in_occl->second.empty())
                            {
                                std::cout << "$$ erase old occerency $$" << std::endl;
                                occl_.erase(this_elem_in_occl);
                            }

                            std::cout << "| END OF MOVING                                      |" << std::endl;
                            std::cout << "| The occl state:                                    |"   << std::endl;
                            std::cout << "|   size = " << occl_.size()                              << std::endl;
                            std::cout << "|   elements:                                        |"   << std::endl; 

                            for (occl_iter it = occl_.begin(), ite = occl_.end(); it!=ite; ++it)
                            {
                                std::cout << "|     occurence = " << it->first << std::endl;
                                std::cout << "|     The vall state:                                |" << std::endl;
                                std::cout << "|       size = " << it->second.size() << std::endl;
                                std::cout << "|       elements:                                    |" << std::endl;
                                
                                for (vall_iter it_vall = it->second.begin(), ite_vall = it->second.end(); it_vall!=ite_vall; ++it_vall)
                                {
                                    std::cout << "|         key = " << it_vall->key_ << std::endl;
                                    std::cout << "|         this element occurence = " << it_vall->occl_it_->first << std::endl;
                                }
                            }
                            std::cout << "| The hashtable state:                               |" << std::endl;
                            std::cout << "|   size = " << ht_.size() << "                               |" << std::endl;
                            std::cout << "|   elements:                                          |" << std::endl;
                            
                            for (ht_iter it = ht_.begin(), ite = ht_.end(); it != ite; ++it)
                            {
                                std::cout << "|     key = " << it->first << std::endl;
                                std::cout << "|     this element occurence = " << it->second->occl_it_->first << std::endl;
                            }

                            std::cout << "------------------------------------------------------" << std::endl;
                        }
                    }
                    std::cout << "End of for in fill_storage()" << std::endl;
                }

                void reduce_occurence(Key_t key)
                {
                    ht_iter     this_elem_in_ht     = ht_.find(key);
                    vall_iter   this_elem_in_vall   = this_elem_in_ht->second;
                    occl_iter   this_elem_in_occl   = this_elem_in_vall->occl_it_;
                    occl_iter   prev_elem_in_occl   = std::prev(this_elem_in_occl);
                    size_t      this_occ            = this_elem_in_occl->first;

                    if (prev_elem_in_occl->first != this_occ-1)
                    {
                        occl_.emplace(this_occ - 1, 0);
                        prev_elem_in_occl = std::prev(this_elem_in_occl);
                    }
                    prev_elem_in_occl->second.splice(prev_elem_in_occl->second.end(), this_elem_in_occl->second, this_elem_in_vall);
                    this_elem_in_vall->occl_it_ = prev_elem_in_occl;

                    if (this_elem_in_occl->second.empty())
                    {
                        occl_.erase(this_elem_in_occl);
                    }
                }
        };

        template <typename Key_t, typename Page_t> class Ideal_Cache final {

            struct Occ_List;
            struct Val_List;

            using page_getter   = std::function<Page_t (const Key_t&)>;
            using vall_iter     = typename std::list<Val_List>::iterator;
            using ht_iter       = typename std::unordered_map<Key_t, vall_iter>::iterator;
            using occl_iter     = typename std::map<size_t, std::list<Val_List>>::iterator;
            
            using rq_iter   = typename std::vector<Key_t>::iterator;

            struct Val_List {
                Key_t key_;
                Page_t page_;
                occl_iter occl_it_;
            };

            Key_Distr<Key_t> k_distr_;
            std::map<size_t, std::list<Val_List>> occl_;                   // maybe you should add a comparator
            std::unordered_map<Key_t, vall_iter> ht_;
            size_t capacity_;
            page_getter slow_get_page_;

            public: 

                Ideal_Cache(rq_iter begin, rq_iter end, size_t capacity, page_getter slow_get_page) : 
                                            k_distr_{begin, end}, capacity_{capacity}, slow_get_page_{slow_get_page} {
                    std::cout << "Before fill_storage()" << std::endl;
                    k_distr_.fill_storage();
                }

                bool full()
                {
                    return ht_.size() == capacity_;
                }

                bool lookup_update(const Key_t &key)
                {
                    ht_iter this_elem_in_ht = ht_.find(key);
                    if (this_elem_in_ht == ht_.end())
                    {
                        size_t this_elem_occ_in_distr = k_distr_.get_occurence(key);
                        if (this_elem_occ_in_distr > 1 && this_elem_occ_in_distr > occl_.begin()->first)
                        {
                            if (full())
                            {
                                remove_from_storage();
                            }
                            k_distr_.reduce_occurence(key);
                            insert_new_elem(this_elem_occ_in_distr-1, key);
                        }
                        return false;
                    }
                    else
                    {
                        k_distr_.reduce_occurence(key);
                        reduce_occurence(this_elem_in_ht);
                        return true;
                    }
                }

            private:

                void remove_from_storage()
                {
                    vall_iter least_encountered = occl_.begin()->second.begin();
                    ht_.erase(ht_.find(least_encountered->key_));
                    occl_.begin()->second.pop_front();   
                    if (occl_.begin()->second.empty())
                    {
                        occl_.erase(occl_.begin());
                    }
                }

                void insert_new_elem(size_t occurence, Key_t key)
                {
                    occl_iter new_elem_in_occl = occl_.emplace(occurence, 0).first;
                    new_elem_in_occl->second.emplace_back(key, slow_get_page_(key), new_elem_in_occl); // It this iterator on occl?
                    ht_.emplace(key, std::prev(new_elem_in_occl->second.end()));
                }

                void reduce_occurence(ht_iter this_elem_in_ht)
                {
                    vall_iter   this_elem_in_vall   = this_elem_in_ht->second;
                    occl_iter   this_elem_in_occl   = this_elem_in_vall->occl_it_;
                    occl_iter   prev_elem_in_occl   = std::prev(this_elem_in_occl);
                    size_t      this_elem_occurence = this_elem_in_occl->first;

                    if (prev_elem_in_occl->first != this_elem_occurence-1)
                    {
                        occl_.emplace(this_elem_occurence-1, 0);
                        prev_elem_in_occl = std::prev(this_elem_in_occl);
                    }
                    prev_elem_in_occl->second.splice(prev_elem_in_occl->second.end(), this_elem_in_occl->second, this_elem_in_vall);
                    this_elem_in_vall->occl_it_ = prev_elem_in_occl;

                    if (this_elem_in_occl->second.empty())
                    {
                        occl_.erase(this_elem_in_occl);
                    }
                }
        };
}

#endif
