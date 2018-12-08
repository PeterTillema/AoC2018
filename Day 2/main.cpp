#include <iostream>
#include <sstream>
#include <fstream>
#include <unordered_map>

int main() {
    std::fstream file{"../input.txt"};
    std::string strings[250];
    std::string segment;
    int index{0};
    int c2{0}, c3{0};

    while (getline(file, segment)) {
        std::unordered_map<char, uint8_t> chars;

        for (auto c : segment) {
            chars[c]++;
        }

        bool hasTwo{false};
        bool hasThree{false};
        for (auto count : chars) {
            if (count.second == 2) hasTwo = true;
            if (count.second == 3) hasThree = true;
        }
        if (hasTwo) c2++;
        if (hasThree) c3++;

        strings[index++] = segment;
    }

    std::cout << "Solution part 1: " << c2 * c3 << std::endl;

    for (auto string1 : strings) {
        for (const auto& string2 : strings) {
            const char *string3 = string2.c_str();

            char val1, val2;
            int amountOfDifferences{0};

            std::stringstream output{""};

            for (auto c : string1) {
                if (c != *string3++) {
                    amountOfDifferences++;
                } else {
                    output << c;
                }
            }

            if (amountOfDifferences == 1) {
                std::cout << "Solution part 2: " << output.str() << std::endl;
            }
        }
    }
}