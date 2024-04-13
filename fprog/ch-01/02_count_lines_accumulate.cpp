/* README: 
- count number of lines in each file using std::accumulate()
*/

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <iterator>
#include <numeric>

int counter(int count, char ch) {
    return (ch != '\n') ? count : count+1; 
}

int count_lines(const std::string &file) {
    int count=0;
    std::ifstream ifs(file);

    count = std::accumulate(
        std::istream_iterator<char>(ifs >> std::noskipws),
        std::istream_iterator<char>(),
        0,
        counter
    );

    /* This case is to count last line that doesn't end with '\n'.
     * We check if the stream's fail state is set (ifs.fail()), if it is,
     * it means the end-of-file was reached before encountering a newline character.
     * In this case, we increment the newline count to include the last line.
     */
    if (ifs.fail()) {
        // If the fail state is set, increment the count to include the last line
        count++;
    }

    return count;

}

std::vector<int>
count_lines_in_files(const std::vector<std::string> &files) {
    std::vector<int> result;

    for (const auto &file: files) {
        result.push_back(count_lines(file));
    }

    return result;
}

int main() {
    auto results = count_lines_in_files({"01_count_lines.cpp", "02_count_lines.cpp"});
    for (auto result: results) {
        std::cout << result << " line(s)" << std::endl;
    }

    return 0;
}