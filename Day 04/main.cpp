#include <iostream>
#include <sstream>
#include <map>
#include <algorithm>
#include <fstream>
#include <vector>

class guard {
private:
    int minuteAsleep{0};
    int totalAsleep{0};
    std::map<int, int> minutesAsleep;

public:
    void sleep(int minute) {
        this->minuteAsleep = minute;
    }

    void awake(int minute) {
        this->totalAsleep += minute - minuteAsleep;
        for (int a = minuteAsleep; a < minute; a++)
            this->minutesAsleep[a]++;
    }

    int getTotalSleepMinutes() const {
        return this->totalAsleep;
    }

    int getMaxMinute() {
        auto x = std::max_element(this->minutesAsleep.begin(), this->minutesAsleep.end(),
                                  [](const std::pair<int, int>& p1, const std::pair<int, int>& p2) {
                                      return p1.second < p2.second; });

        return x->first;
    }

    int getAmountOfAsleepPerMinute(int minute) {
        return this->minutesAsleep[minute];
    }
};

int main() {
    std::fstream file{"../input.txt"};
    std::string segment;

    std::vector<std::string> inputLines;
    while (getline(file, segment))
        inputLines.push_back(segment);

    std::sort(inputLines.begin(), inputLines.end());

    std::map<int, guard> guards;
    for (auto& i : inputLines) {
        std::stringstream sss{i};
        std::string command;
        int minute, guardId;
        char c;

        sss >> segment >> minute >> c >> minute >> c >> command;
        if (command == "Guard") {
            sss >> c >> guardId;
        } else if (command == "falls") {
            guards[guardId].sleep(minute);
        } else {
            guards[guardId].awake(minute);
        }
    }

    auto x = std::max_element(guards.begin(), guards.end(),
                              [](const std::pair<int, guard>& p1, const std::pair<int, guard>& p2) {
                                  return p1.second.getTotalSleepMinutes() < p2.second.getTotalSleepMinutes();
    });

    std::cout << "Solution part 1: " << x->first * x->second.getMaxMinute() << std::endl;

    int laziestGuard{0};
    int laziestMinute{0};
    int amountOfAsleepTimes{0};
    for (int i = 0; i < 59; i++) {
        for (auto& guard : guards) {
            int y = guard.second.getAmountOfAsleepPerMinute(i);
            if (y > amountOfAsleepTimes) {
                laziestGuard = guard.first;
                laziestMinute = i;
                amountOfAsleepTimes = y;
            }
        }
    }

    std::cout << "Solution part 2: " << laziestGuard * laziestMinute << std::endl;
}