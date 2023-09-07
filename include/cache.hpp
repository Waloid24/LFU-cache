#ifndef CACHE_HPP
#define CACHE_HPP

#include <iostream>
#include <unordered_map>
#include <cassert>
#include <list>

namespace caches {

template<typename Key_t, typename Page_t>class LfuCache final {

    using ValListIt = typename std::list<struct valList>::iterator;
    using FrqListIt = typename std::list<struct frqList>::iterator;

    struct frqList {
        int freq;
        std::list<struct valList> vall;
    };
    struct valList {
        Key_t key;
        Page_t val;
        FrqListIt frql_ptr;
    };

    // std::list<struct valList> vall_;
    std::list<struct frqList> frql_;
    std::unordered_map<Key_t, struct valList *> ht_{};
    size_t size_;

    public:

        LfuCache(size_t size) : size_(size){}

    private:

        bool full() const { 
            return (ht_.size() == size_);
        }

    public:

        template <typename F> bool lookup_update(Key_t key, F slow_get_page) {
            if (ht_.contains(key) == false)
            {
                auto listFreqOfOne = (*(frql_.begin())).vall;                   //TODO: most likely, you need a pointer
                if (full())
                {
                    ht_.erase(ht_.find((*(listFreqOfOne.begin())).key));        //removed from the list of values and from the hash table the first one for the list of element values
                    listFreqOfOne.erase(listFreqOfOne.begin());
                }

                struct valList newElem {
                    key,
                    slow_get_page(key),
                    frql_.begin(),
                };

                listFreqOfOne.push_front(newElem);                              //TODO: isn't there a dead parrot here?
                ht_.insert(newElem);                                            //TODO: and here...
            }
            
        }
        
};

}

#endif


// size_t get(Key_t key)
//         {
//             if (valueMap.contains(key) == false)
//             {
//                 std::cout << "valueMap doesn't contain element with _" << key << "_ key" << std::endl;
//                 return -1;
//             }
//             freqMap.insert({key, freqMap.at(key) + 1});
//             return valueMap.at(key);
//         }

//         void put(Key_t key, Page_t value)
//         {
//             if (valueMap.contains(key) == false)
//             {
//                 valueMap.insert({key, value});
//                 freqMap.insert({key, 1});
//             }
//             else
//             {
//                 valueMap.insert({key, value});
//                 freqMap.insert({key, freqMap.at(key) + 1});
//             }
//         }

//         bool lookup_update(Key_t key, Page_t value)
//         {
//             if (get(key) == (size_t)-1)
//             {
                
//             }
//         }