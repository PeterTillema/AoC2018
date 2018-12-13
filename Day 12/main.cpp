#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>

struct changes {
    bool prevPrev{false};
    bool prev{false};
    bool curr{false};
    bool next{false};
    bool nextNext{false};
    bool result{false};
};

int getSum(std::map<int, bool> map) {
    int sum{0};

    for (auto c : map) {
        if (c.second)
            sum += c.first;
    }

    return sum;
}

int main() {
    std::ifstream file{"../input.txt"};
    std::string segment;
    std::vector<int> diffs(20);
    std::map<int, bool> input;
    bool allChanges[2][2][2][2][2]{false};

    file >> segment >> segment >> segment;

    int index{0};
    for (auto c : segment) {
        input[index++] = (c == '#');
    }

    getline(file, segment);
    getline(file, segment);
    while (getline(file, segment)) {
        allChanges[segment.at(0) == '#']
                  [segment.at(1) == '#']
                  [segment.at(2) == '#']
                  [segment.at(3) == '#']
                  [segment.at(4) == '#'] = (segment.at(9) == '#');
    }

    int generation{0};

    while (++generation) {
        std::map<int, bool> newInput;

        for (int b = -4 * generation; b <= 100 + 4 * generation; b++) {
            newInput[b] = allChanges[input[b - 2]]
                                    [input[b - 1]]
                                    [input[b]]
                                    [input[b + 1]]
                                    [input[b + 2]];
        }

        int prevSum{getSum(input)};
        int newSum{getSum(newInput)};
        int diff{newSum - prevSum};

        if (generation == 20) {
            std::cout << "Solution part 1: " << newSum << std::endl;
        }

        diffs.erase(diffs.begin());
        diffs.push_back(diff);
        input = newInput;

        if (std::adjacent_find(diffs.begin(), diffs.end(), std::not_equal_to<>()) == diffs.end()) {
            std::cout << "Solution part 2: " << diff * (50000000000 - generation) + newSum << std::endl;
            break;
        }
    }
}