# LFU-cache

## About
Here is my LFU-cache implementation with complexity O(1).
Below is the structure of my cache.

![my LFU-cache structure](./images/lfu_cache.jpg)

The class is wrapped in the 'caches' namespace.

### Methods
The LFU-cache constructor takes **2** arguments: 'size_t size' and 'std::function<Page_t(const Key_t&)> slow_get_page'. The function 'slow_get_page()' returns the *page* (type **Page_t**) based on the *key* (type **Key_t**), you need to dereminated it yourself. The argument 'size' specifies the cache capacity. For example:
```c++
int slow_get_page_int(int key) { return key; }

int main()
{
    ...
    caches::Lfu_Cache<int, int> my_cache{cache_size, slow_get_page_int};
    ...
}
```
In this example, *page* = *key* and both are int type.

There are three methods, available to the user:
1. bool lookup_update(const Key_t &key);
2. bool full() const;
3. bool empty() const;

The function `lookup_update()` allows you to save new elements to the cache. If the 'key' is already used in the cache, the function returns 'true', otherwize 'false'.
The function `full()` checks if the cache is full.
The function `empty()` checks if the cache is empty.

## Build

For compile this project, type:
```makefile
git clone https://github.com/Waloid24/LFU-cache.git
cd LFU-cache
mkdir build
cd build
cmake ..
make
```

For run, type:
```makefile
./cache
```





