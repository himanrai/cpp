#include <iostream>
#include <vector>
#include <array>
#include <numeric> //std::accumulate

template <typename T>
double calculate_average(const T &seq) {
    return std::accumulate(seq.cbegin(), seq.cend(),
        0) / (double) seq.size();
}

int calculate_product(const std::vector<int> &vec) {
    return std::accumulate(vec.cbegin(), vec.cend(),
        1, std::multiplies<int>());
}

int main() {
    std::vector<int> vec{10, 20, 30, 40, 50};
    std::array<int, 6> arr{1, 2, 3, 4, 5, 6};

    std::cout << "average: " << calculate_average(vec) << std::endl;
    std::cout << "average: " << calculate_average(arr) << std::endl;

    std::cout << "product: " << calculate_product({1,2,3,4,5}) << std::endl;

    return 0;
}