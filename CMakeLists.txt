# usefull link: https://google.github.io/googletest/quickstart-cmake.html

cmake_minimum_required(VERSION 3.24)
project(Lfu_Cache_Testing)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include/)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall") # use CMAKE_CXX_STANDARD

message(STATUS "CMAKE_BINARY_DIR = ${CMAKE_BINARY_DIR}")

# if(TESTING_LFU)
    add_subdirectory(
        "tests"
    )
    add_subdirectory(
        "/usr/src/gtest" ${CMAKE_BINARY_DIR}/gtest
    )
    enable_testing()
    include_directories(${GTEST_INCLUDE_DIR})    
    add_executable(
        lfu_cache_testing
        ${SOURCE_LIST}
    )
    target_link_libraries(
        lfu_cache_testing gtest gtest_main
    )
    add_test(AllTests lfu_cache_testing)

    include(GoogleTest)
    gtest_discover_tests(lfu_cache_testing)

    target_include_directories(lfu_cache_testing PUBLIC ${INCLUDE_DIR})
# else()
    # message(STATUS "------------NOT TESTS-----------------")

    if(USE_IDEAL_CACHE)
        message(STATUS "----------Ideal cache------------")
        add_executable(
            lfu_cache_ideal
            ideal_cache.cpp
        )
        target_include_directories(lfu_cache_ideal PUBLIC ${INCLUDE_DIR})
    else()
        message(STATUS "--------Regular Cache----------")
        add_executable(
            lfu_cache
            cache.cpp
        )
        target_include_directories(lfu_cache PUBLIC ${INCLUDE_DIR})
    endif()

# endif()



