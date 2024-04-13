/* README: 
- count number of lines in each file using std::count().
*/

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iterator>
#include <algorithm>

int count_lines(const std::string &file_name) {
    std::ifstream ifs(file_name);
    ifs.unsetf(std::ios_base::skipws);

    int count = std::count(
        std::istream_iterator<char>(ifs),
        std::istream_iterator<char>(),
        '\n'
        );

    /* This case is to count last line that doesn't end with '\n'.
     * We check if the stream's fail state is set (ifs.fail()), if it is,
     * it means the end-of-file was reached before encountering a newline character.
     * In this case, we increment the newline count to include the last line.
     */
    if (ifs.fail()) {
        // If the fail state is set, increment the count to include the last line
        ++count;
    }

    return count;

}

std::vector<int>
count_lines_in_files(const std::vector<std::string> &files) {
    std::vector<int> results;

    for (const auto &file: files) {
        results.push_back(count_lines(file));
    }

    return results;
}

int main() {

    //std::vector<std::string> files{"01_count_lines.cpp", "02_count_lines.cpp"};
    //auto result = count_lines_in_files(files);

    auto results = count_lines_in_files({"01_count_lines.cpp", "02_count_lines.cpp"});

    for (auto result: results) {
        std::cout << result << " line(s)" << std::endl;
    }

    return 0;
}