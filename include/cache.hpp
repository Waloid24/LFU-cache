#ifndef CACHE_HPP
#define CACHE_HPP

#include <iostream>
#include <unordered_map>

template<typename Key_t, typename Page_t>class LfuCache final {
    std::unordered_map<Key_t, Page_t> valueMap{};
    std::unordered_map<Key_t, size_t> freqMap{};

    size_t size_;

    public:

        LfuCache(std::size_t size) : size_(size){}

        std::size_t get(Key_t key)
        {
            if (valueMap.contains(key) == false)
            {
                std::cout << "valueMap doesn't contain element with _" << key << "_ key" << std::endl;
                return -1;
            }
            freqMap.insert({key, freqMap.at(key) + 1});
            return valueMap.at(key);
        }

        void put(Key_t key, Page_t value)
        {
            if (valueMap.contains(key) == false)
            {
                valueMap.insert({key, value});
                freqMap.insert({key, 1});
            }
            else
            {
                valueMap.insert({key, value});
                freqMap.insert({key, freqMap.at(key) + 1});
            }
        }
};

#endif