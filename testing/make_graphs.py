import matplotlib.pyplot as plt
import numpy as np
import sys

def build_bar_chart (n_tests: int):
    
    # cat_par = [f"{i + 1}" for i in range(n_tests)]

    ideal_cache = []
    lfu_cache = []
    for test_i in range (n_tests):

        with open(f"result/ideal_cache/ideal_cache_results_{test_i}.txt", "r") as ideal_cache_file:
            ideal_cache.append(int(ideal_cache_file.readline ()))

        with open(f"result/lfu_cache/lfu_cache_results_{test_i}.txt", "r") as lfu_cache_file:
            lfu_cache.append(int(lfu_cache_file.readline ()))

    x = np.arange(n_tests)

    plt.figure (figsize = (16,9), dpi = 80)

    width = 0.3
    plt.bar(x - width/2, ideal_cache, width, label = "ideal")
    plt.bar(x + width/2, lfu_cache  , width, label = "lfu")

    plt.title("Comparison of LFU and ideal's cache replacement algorithms")
    plt.legend()

    plt.xlabel ("Test number",    fontsize = 16)
    plt.ylabel ("Number of hits", fontsize = 16)

    plt.xticks(x)

    plt.savefig ("algorithm_comparison.png")

def main ():

    n_tests: int = int (sys.argv[1])
    build_bar_chart (n_tests)

main ()