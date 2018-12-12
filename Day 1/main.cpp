#include <iostream>
#include <sstream>
#include <unordered_map>
#include <fstream>

int main() {
    std::fstream file{"../input.txt"};
    std::string segment;
    int sum{0};

    getline(file, segment);

    std::stringstream input{segment};
    int number;
    while (input >> number)
        sum = sum + number;

    std::cout << "Solution part 1: " << sum << std::endl;

    std::unordered_map<int, bool> freqs;
    sum = 0;
    freqs[sum] = true;
    while (true) {
        std::stringstream input2{segment};
        while (input2 >> number) {
            sum = sum + number;
            if (freqs[sum]) {
                std::cout << "Solution part 2: " << sum << std::endl;
                return 0;
            }
            freqs[sum] = true;
        }
    }
}