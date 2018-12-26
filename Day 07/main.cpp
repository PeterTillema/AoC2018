#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

int main() {
    std::fstream file{"../input.txt"};
    std::string dataLine;

    std::map<char, std::map<char, bool>> dependencies;
    std::map<char, bool> finished;

    while (getline(file, dataLine)) {
        std::stringstream data{dataLine};
        std::string temp;
        char dependency;
        char target;

        data >> temp >> dependency >> temp >> temp >> temp >> temp >> temp >> target;
        dependencies[target][dependency] = true;
    }

    std::cout << "Solution part 1: ";
    std::map<char, bool> hasFinished;
    std::map<char, bool> beingWorkedOn;

    for (uint8_t i = 'A'; i <= 'Z'; i++) {
        hasFinished[i] = false;
    }

    bool foundMatch{true};
    int workerTime[5]{0};
    int workerStep[5];
    int time{0};
    while (foundMatch) {
        foundMatch = false;

        for (uint8_t i = 'A'; i <= 'Z' && !foundMatch; i++) {
            if (!hasFinished[i]) {
                if (dependencies.count(i) > 0) {
                    for (auto x : dependencies[i]) {
                        if (!hasFinished[x.first]) {
                            goto notFinished;
                        }
                    }
                    std::cout << i;
                    hasFinished[i] = true;
                    foundMatch = true;
notFinished:;
                } else {
                    std::cout << i;
                    hasFinished[i] = true;
                    foundMatch = true;
                }
            }
        }
    }

    std::cout << std::endl;


    for (int i = 'A'; i <= 'Z'; i++) {
        hasFinished[i] = false;
        beingWorkedOn[i] = false;
    }

    int index = 0;
    while (index < 26 || workerTime[0] || workerTime[1] || workerTime[2] || workerTime[3] || workerTime[4]) {
        for (int i = 0; i < 5; i++) {
            if (workerTime[i] == 1) {
                hasFinished[workerStep[i]] = true;
            }
            if (workerTime[i]) {
                workerTime[i]--;
            }
        }

        for (int i = 'A'; i <= 'Z'; i++) {
            if (!hasFinished[i] && !beingWorkedOn[i]) {
                if (dependencies.count(i) > 0) {
                    for (auto x : dependencies[i]) {
                        if (!hasFinished[x.first]) {
                            goto notFinished2;
                        }
                    }
                    for (int j = 0; j < 5; j++) {
                        if (!workerTime[j]) {
                            workerTime[j] = 61 + i - 'A';
                            workerStep[j] = i;
                            beingWorkedOn[i] = true;
                            index++;
                            break;
                        }
                    }
notFinished2:;
                } else {
                    for (int j = 0; j < 5; j++) {
                        if (!workerTime[j]) {
                            workerTime[j] = 61 + i - 'A';
                            workerStep[j] = i;
                            beingWorkedOn[i] = true;
                            index++;
                            break;
                        }
                    }
                }
            }
        }
        time++;
    }

    std::cout << "Solution part 2: " << time - 1 << std::endl;
}