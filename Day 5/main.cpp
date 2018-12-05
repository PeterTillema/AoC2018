#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <map>

void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

int fullyReact(std::string inputData) {
    std::string inputData2;
    int length = 0;

    inputData2.assign(inputData);
    while (length != inputData2.length()) {
        length = inputData2.length();
        replaceAll(inputData2, "aA", "");
        replaceAll(inputData2, "bB", "");
        replaceAll(inputData2, "cC", "");
        replaceAll(inputData2, "dD", "");
        replaceAll(inputData2, "eE", "");
        replaceAll(inputData2, "fF", "");
        replaceAll(inputData2, "gG", "");
        replaceAll(inputData2, "hH", "");
        replaceAll(inputData2, "iI", "");
        replaceAll(inputData2, "jJ", "");
        replaceAll(inputData2, "kK", "");
        replaceAll(inputData2, "lL", "");
        replaceAll(inputData2, "mM", "");
        replaceAll(inputData2, "nN", "");
        replaceAll(inputData2, "oO", "");
        replaceAll(inputData2, "pP", "");
        replaceAll(inputData2, "qQ", "");
        replaceAll(inputData2, "rR", "");
        replaceAll(inputData2, "sS", "");
        replaceAll(inputData2, "tT", "");
        replaceAll(inputData2, "uU", "");
        replaceAll(inputData2, "vV", "");
        replaceAll(inputData2, "wW", "");
        replaceAll(inputData2, "xX", "");
        replaceAll(inputData2, "yY", "");
        replaceAll(inputData2, "zZ", "");

        replaceAll(inputData2, "Aa", "");
        replaceAll(inputData2, "Bb", "");
        replaceAll(inputData2, "Cc", "");
        replaceAll(inputData2, "Dd", "");
        replaceAll(inputData2, "Ee", "");
        replaceAll(inputData2, "Ff", "");
        replaceAll(inputData2, "Gg", "");
        replaceAll(inputData2, "Hh", "");
        replaceAll(inputData2, "Ii", "");
        replaceAll(inputData2, "Jj", "");
        replaceAll(inputData2, "Kk", "");
        replaceAll(inputData2, "Ll", "");
        replaceAll(inputData2, "Mm", "");
        replaceAll(inputData2, "Nn", "");
        replaceAll(inputData2, "Oo", "");
        replaceAll(inputData2, "Pp", "");
        replaceAll(inputData2, "Qq", "");
        replaceAll(inputData2, "Rr", "");
        replaceAll(inputData2, "Ss", "");
        replaceAll(inputData2, "Tt", "");
        replaceAll(inputData2, "Uu", "");
        replaceAll(inputData2, "Vv", "");
        replaceAll(inputData2, "Ww", "");
        replaceAll(inputData2, "Xx", "");
        replaceAll(inputData2, "Yy", "");
        replaceAll(inputData2, "Zz", "");
    }

    return length;
}

int main() {
    std::ifstream inputFile{"../input.txt"};
    std::string inputData;
    std::string inputData2;
    int length = 0;

    getline(inputFile, inputData);
    std::cout << "Solution part 1: " << fullyReact(inputData) << std::endl;

    int minLength = 99999;
    for (int i = 0; i < 26; i++) {
        inputData2.assign(inputData);

        inputData2.erase(std::remove(inputData2.begin(), inputData2.end(), char(i + 'a')), inputData2.end());
        inputData2.erase(std::remove(inputData2.begin(), inputData2.end(), char(i + 'A')), inputData2.end());
        int length = fullyReact(inputData2);

        if (length < minLength) {
            minLength = length;
        }
    }

    std::cout << "Solution part 2: " << minLength << std::endl;
}