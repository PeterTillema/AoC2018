#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <map>

int strip(std::string data, int remove = 0) {
    int length = 0;

    if (remove) {
        data.erase(std::remove(data.begin(), data.end(), remove), data.end());
        data.erase(std::remove(data.begin(), data.end(), remove + 'A' - 'a'), data.end());
    }

    bool found{true};
    while (found) {
        found = false;

        for (unsigned int a{0}; a < data.length() - 1; a++) {
            char cur = data[a];
            char next = data[a + 1];

            if (cur == next + 'A' - 'a' || cur == next - 'A' + 'a') {
                data.erase(a, 2);
                found = true;

            }
        }
    }

    return data.length();
}

int main() {
    std::ifstream inputFile{"../input.txt"};
    std::string data;
    int length = 0;

    getline(inputFile, data);
    std::cout << "Solution part 1: " << strip(data) << std::endl;

    std::map<char, int> lengths;
    for (int i = 'a'; i <= 'z'; i++) {
        lengths[i] = strip(data, i);
    }
    auto x = std::min_element(lengths.begin(), lengths.end(),
            [](decltype(lengths)::value_type& l, decltype(lengths)::value_type& r) -> bool { return l.second < r.second; });

    std::cout << "Solution part 2: " << x->second << std::endl;
}