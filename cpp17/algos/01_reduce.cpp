/* README:

- std::reduce (C++17) is a generalized reduction algorithm, a counterpart
  to the left-fold std::accumulate.
- std::reduce can only operate with an associative and commutative
  operation as the reduction is not evaluated in strict left-to-right order.
- The algorithm also supports parallel execution through std::execution.

- Since C++17, many STL algorithms let you specify that their execution
  should be parallelized.
- The algorithms that can be parallelized accept the execution policy
  as an additional argument.
- If you want the algorithm execution to be parallelized, you need to pass
  it the std::execution::par policy.

- std::mt19937(since C++11) class is a very efficient pseudo-random number
  generator and is defined in a random header file.
- It produces 32-bit pseudo-random numbers using the well-known and popular
  algorithm named Mersenne twister algorithm. std::mt19937 class.
- mt19937 stands for mersenne twister with a long period of (2^19937 – 1) which
  means mt19937 produces a sequence of 32-bit integers that only repeats
  itself after (2^19937 – 1) number have been generated.
- The std::mt19937 does two things –
  -> When an std::mt19937 object is instantiated, it takes an argument which
      is used to generate seed value(like srand()).
  -> By using operator(), it generates a random number (like rand()).
*/

#include <vector>
#include <algorithm>
#include <execution>
#include <random>

double average_score(const std::vector<int>& scores)
{
    return std::reduce(
        std::execution::par,
        scores.cbegin(), scores.cend(),
        0) / (double) scores.size();
}

int main() {
    std::vector<int> data{1, 2, 3, 4, 5};
    std::cout << "average score: " << average_score(data) << std::endl;

    // Basic reduce with int{} (0) as starting value 
    // and std::plus<int>{} as the reduction operation
    int sum = std::reduce(data.begin(), data.end());
    std::cout << "sum: " << sum << std::endl;


    // Reduce with custom init value and reduction operation
    int product = std::reduce(data.begin(), data.end(),
                        1, std::multiplies<>{});
    std::cout << "product: " << product << std::endl;


    std::mt19937 gen(0);
    std::vector<unsigned> large_data;
    std::generate_n(std::back_inserter(large_data), 10,
                std::ref(gen));

    for (auto data: large_data) {
         std::cout << data << " ";
    }
    std::cout << std::endl;

    // Automatically parallelized sum
    int64_t big_sum = std::reduce(std::execution::par,
                              large_data.cbegin(), large_data.cend(),
                              0z);
    std::cout << "big sum: " << big_sum << std::endl;

    return 0;
}

/* Output:
average score: 3
sum: 15
product: 120
2357136044 2546248239 3071714933 3626093760 2588848963 3684848379 2340255427 3638918503 1819583497 2678185683 
big sum: 28351833428
*/