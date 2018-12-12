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
    std::fstream file{"../input.txt"};
    std::string segment;
    std::string input2;
    std::vector<changes> allChanges;
    std::vector<int> diffs(30);
    std::map<int, bool> input;

    getline(file, segment);
    std::stringstream sss{segment};
    sss >> segment >> segment >> input2;

    int index{0};
    for (auto c : input2) {
        input[index++] = (c == '#');
    }

    getline(file, segment);
    while (getline(file, segment)) {
        changes changes;

        changes.prevPrev = (segment.at(0) == '#');
        changes.prev = (segment.at(1) == '#');
        changes.curr = (segment.at(2) == '#');
        changes.next = (segment.at(3) == '#');
        changes.nextNext = (segment.at(4) == '#');
        changes.result = (segment.at(9) == '#');
        allChanges.push_back(changes);
    }

    int generation{0};

    while (++generation) {
        std::map<int, bool> newInput;
        std::map<int, bool> input3 = input;

        for (int b = -1000; b <= 1000; b++) {
            for (auto c : allChanges) {
                if (input[b - 2] == c.prevPrev &&
                    input[b - 1] == c.prev &&
                    input[b + 0] == c.curr &&
                    input[b + 1] == c.next &&
                    input[b + 2] == c.nextNext) {
                    newInput[b] = c.result;
                }
            }
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