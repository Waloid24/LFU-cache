#include <gtest/gtest.h>
#include "lfu_cache.hpp"

int slow_get_page_int(int key) { return key; }

TEST(TestGroupName, Subtest_1) 
{
	#if 1
	using key_t = int;
    using page_t = key_t;

	int n = 6;
	size_t size = 4;
    caches::Lfu_Cache<key_t, page_t> my_cache{size, slow_get_page_int};
	
	int requests[n] = {1,1,1,1,1,1};
	int hits = 0;
    for (int i = 0; i < n; i++)
    {
        int rq = requests[i];
        hits += my_cache.lookup_update(rq);
    }
	#endif

	ASSERT_TRUE(hits == 5);
}

TEST(TestGroupName, Subtest_2) 
{
	#if 1
	using key_t = int;
    using page_t = key_t;

	int n = 6;
	size_t size = 4;
    caches::Lfu_Cache<key_t, page_t> my_cache{size, slow_get_page_int};
	
	int requests[n] = {1,2,3,4,5,6};
	int hits = 0;
    for (int i = 0; i < n; i++)
    {
        int rq = requests[i];
        hits += my_cache.lookup_update(rq);
    }
	#endif

	ASSERT_TRUE(hits == 0);
}

TEST(TestGroupName, Subtest_3) 
{
	#if 1
	using key_t = int;
    using page_t = key_t;

	int n = 2;
	size_t size = 4;
    caches::Lfu_Cache<key_t, page_t> my_cache{size, slow_get_page_int};
	
	int requests[n] = {1,1};
	int hits = 0;
    for (int i = 0; i < n; i++)
    {
        int rq = requests[i];
        hits += my_cache.lookup_update(rq);
    }
	#endif

	ASSERT_TRUE(hits == 1);
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}