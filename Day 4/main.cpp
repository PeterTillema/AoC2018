#include <iostream>
#include <sstream>
#include <chrono>
#include <map>
#include <algorithm>
#include <fstream>

int main() {
    std::fstream file{"../input.txt"};
    std::string segment;
    std::map<int, int> shifts;
    std::map<int, int> guards;

    while (getline(file, segment)) {
        int month, day, hour, minute, guard;
        char c;
        std::string command;
        std::stringstream sss{segment};

        sss >> c >> month >> c >> month >> c >> day >> c >> hour >> c >> minute >> c >> command;
        int timestamp = (((month * 31) + day) * 24 + hour) * 60 + minute;

        if (command == "Guard") {
            sss >> c >> guard;
            shifts[timestamp] = guard;
        } else if (command == "falls") {
            shifts[timestamp] = -2;
        } else if (command == "wakes") {
            shifts[timestamp] = -1;
        }
    }

    int guard{0};
    int timestart{0};

    for (auto const& value : shifts) {
        if (value.second == -1) {
            guards[guard] += value.first - timestart;
        } else if (value.second == -2) {
            timestart = value.first;
        } else {
            guard = value.second;
        }
    }

    auto x = std::max_element(guards.begin(), guards.end(),
                              [](const std::pair<int, int>& p1, const std::pair<int, int>& p2) {
                                  return p1.second < p2.second; });

    bool isRightGuard{false};
    guard = 0;
    timestart = 0;
    std::map<uint8_t, int> minutes;
    std::map<int, std::map<int, int>> minutes_guards;
    for (auto const& value : shifts) {
        if (value.second == -2) {
            timestart = value.first;
        } else if (value.second == -1) {
            int minute1 = timestart % 60;
            int minute2 = value.first % 60;

            if (minute1 > minute2) {
                for (int a = minute1; a < 60; a++) {
                    minutes_guards[a][guard]++;
                }
                for (int a = 0; a < minute2; a++) {
                    minutes_guards[a][guard]++;
                }
            } else {
                for (int a = minute1; a < minute2; a++) {
                    minutes_guards[a][guard]++;
                }
            }

            if (guard == x->first) {
                if (minute1 > minute2) {
                    for (int a = minute1; a < 60; a++) {
                        minutes[a]++;
                    }
                    for (int a = 0; a < minute2; a++) {
                        minutes[a]++;
                    }
                } else {
                    for (int a = minute1; a < minute2; a++) {
                        minutes[a]++;
                    }
                }
            }
        } else {
            guard = value.second;
        }
    }

    auto y = std::max_element(minutes.begin(), minutes.end(),
                              [](const std::pair<int, int>& p1, const std::pair<int, int>& p2) {
                                  return p1.second < p2.second; });

    std::cout << "Solution part 1: " << x->first * y->first << std::endl;

    int maxMinute{0};
    int totalAsleep{0};
    guard = 0;
    for (const auto& z : minutes_guards) {
        for (const auto& abc : z.second) {
            if (abc.second > totalAsleep) {
                totalAsleep = abc.second;
                maxMinute = z.first;
                guard = abc.first;
            }
        }
    }

    std::cout << "Solution part 2: " << guard * maxMinute << std::endl;
}