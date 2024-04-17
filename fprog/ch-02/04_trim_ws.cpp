/* README:
- trim whitespace from stating and ending of a string
*/

#include <iostream>
#include <string>
#include <algorithm>

void ltrim(std::string &str) {
    std::string ws{" \t\n\r\v"};
    str.erase(0, str.find_first_not_of(ws));
}

void rtrim(std::string &str) {
    std::string ws{" \t\n\r\v"};
    str.erase(str.find_last_not_of(ws)+1);
}

void trim(std::string &str) {
    ltrim(str);
    rtrim(str);
}

std::string ltrim2(std::string str) {
    str.erase(
        str.begin(),
        std::find_if(str.begin(), str.end(),
            [](char ch) {return !isspace(ch);})
    );
    return str;
}

std::string rtrim2(std::string str) {
    str.erase(
        std::find_if(str.rbegin(), str.rend(),
            [](char ch) {return !isspace(ch);}).base(),
        str.end()
    );
    return str;
}

std::string trim2(std::string str) {
    return rtrim2(ltrim2(str));
}

int main() {
    std::string str1{"  \t \nString-1 with whitespaces trimmed  \n"};
    trim(str1);
    std::cout << "*" << str1 << "*" << std::endl;

    std::string str2{"  \t \nString-2 with whitespaces trimmed  \n"};
    std::string result = trim2(str2);
    std::cout << "*" << result << "*" << std::endl;

    return 0;
}