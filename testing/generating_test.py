import random
import numpy as np
import sys

def generate_tests (n_tests: int, output_dir):

    for test_i in range (n_tests):

        cache_size: int = np.random.randint(100, 1000)
        n_keys: int = np.random.randint(1000, 10000)

        laplace     = np.random.laplace(0, 20, n_keys)
        binomial    = np.random.binomial(50, 0.1, n_keys)
        exponential = np.random.exponential(200, n_keys)
        normal      = np.random.normal(0, 15, n_keys)
        gamma       = np.random.gamma(1, 50, n_keys)

        with open(f"./{output_dir}/test_{test_i}.txt", "w") as test_file:

            test_file.write(str(cache_size) + "\n" + str(n_keys * 5) + "\n")

            for l, b, e, n, g in zip (laplace, binomial, exponential, normal, gamma):
                test_file.write(f"{int(l)} {int(b)} {int(e)} {int(b)} {int(g)} ")
            test_file.write("\n")


n_tests: int = int(sys.argv[1])
output_dir = str(sys.argv[2])
generate_tests(n_tests, output_dir)