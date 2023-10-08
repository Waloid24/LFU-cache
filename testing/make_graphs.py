import matplotlib.pyplot as plt
import numpy as np
import sys

def build_bar_chart (n_tests: int):
    
    # cat_par = [f"{i + 1}" for i in range(n_tests)]

    belady_cache = []
    lfu_cache = []
    for test_i in range (n_tests):

        with open(f"result/belady_cache/belady_cache_results_{test_i}.txt", "r") as belady_cache_file:
            belady_cache.append(int(belady_cache_file.readline ()))

        with open(f"result/lfu_cache/lfu_cache_results_{test_i}.txt", "r") as lfu_cache_file:
            lfu_cache.append(int(lfu_cache_file.readline ()))

    x = np.arange(n_tests)

    plt.figure (figsize = (16,9), dpi = 80)

    width = 0.3
    plt.bar(x - width/2, belady_cache, width, label = "belady")
    plt.bar(x + width/2, lfu_cache  , width, label = "lfu")

    plt.title("Comparison of LFU and Belady's cache replacement algorithms")
    plt.legend()

    plt.xlabel ("Test number",    fontsize = 16)
    plt.ylabel ("Number of hits", fontsize = 16)

    plt.xticks(x)

    plt.savefig ("../images/algorithm_comparison.png")

def main ():

    n_tests: int = int (sys.argv[1])
    build_bar_chart (n_tests)

main ()