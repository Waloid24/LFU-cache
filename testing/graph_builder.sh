#!/usr/bin/bash

function mk_dir
{
    rm -r $1
    mkdir -p $1
}  

function run_tests
{
    local ideal_cache="../build/lfu_cache_ideal"
    local lfu_cache="../build/lfu_cache"

    local lfu_result_dir="result/lfu_cache"
    local ideal_result_dir="result/ideal_cache"

    mk_dir ${lfu_result_dir}
    mk_dir ${ideal_result_dir}

    echo "The ideal cache is working..."
    for ((i=0; i<$n_tests; i++))
    do
        $ideal_cache < tests/test_${i}.txt > ${ideal_result_dir}/ideal_cache_results_${i}.txt
    done
    echo -en "\n"

    echo "The LFU-cache is working..."
    for ((i=0; i<$n_tests; i++))
    do
        $lfu_cache < tests/test_${i}.txt > ${lfu_result_dir}/lfu_cache_results_${i}.txt
    done
    echo -en "\n"
}

function build_graphs
{
    local lfu_result_dir="result/lfu_cache"
    local ideal_result_dir="result/ideal_cache"

    python3 make_graphs.py ${n_tests} ${lfu_result_dir} ${ideal_result_dir}
}

n_tests=$1

if [[ $# -ne 1 ]]
then
    echo "The number of arguments for testing script should be equal to 1"
else
    if [[ n_tests -le 0 ]]
    then 
        echo "The number of tests must be greater than zero"
    else
        mk_dir tests

        echo "Generating tests..."
        
        python3 generating_test.py $n_tests tests
        echo -en "\n"

        echo "Building cache..."
        cmake .. -B ../build
        cmake --build ../build
        echo -en "\n"

        run_tests

        build_graphs
    fi
fi
