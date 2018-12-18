#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

enum tileType {
    EMPTY = 0,
    TREE,
    LUMBERYARD
};

int main() {
    std::ifstream file{"../input.txt"};
    std::string segment;
    uint8_t map[52][52];
    uint8_t map2[52][52];

    for (int a = 0; a < 52; a++) {
        for (int b = 0; b < 52; b++) {
            map[a][b] = EMPTY;
            map2[a][b] = EMPTY;
        }
    }

    int row{1};
    while (getline(file, segment)) {
        int column{1};

        for (auto c : segment) {
            if (c == '#') {
                map[row][column] = LUMBERYARD;
                map2[row][column] = LUMBERYARD;
            } else if (c == '|') {
                map[row][column] = TREE;
                map2[row][column] = TREE;
            } else {
                map[row][column] = EMPTY;
                map2[row][column] = EMPTY;
            }
            column++;
        }

        row++;
    }

    int totalWood{0};
    int totalLumberyard{0};
    int counting{0};
    bool startCounting{false};
    std::vector<std::pair<int, int>> pattern;

    int i{0};
    while (true) {
        uint8_t new_map[52][52];

        for (int a = 0; a < 52; a++) {
            for (int b = 0; b < 52; b++) {
                new_map[a][b] = EMPTY;
            }
        }

        totalWood = totalLumberyard = 0;

        for (int row = 1; row < 51; row++) {
            for (int column = 1; column < 51; column++) {
                uint8_t type{map[row][column]};

                uint8_t leftup{map[row - 1][column - 1]};
                uint8_t up{map[row - 1][column]};
                uint8_t rightup{map[row - 1][column + 1]};
                uint8_t down{map[row + 1][column]};
                uint8_t left{map[row][column - 1]};
                uint8_t right{map[row][column + 1]};
                uint8_t leftdown{map[row + 1][column - 1]};
                uint8_t rightdown{map[row + 1][column + 1]};

                int amountOfNbTrees{0};
                int amountOfNbLumberyards{0};

                for (auto dir : {up, down, left, right, leftup, rightup, leftdown, rightdown}) {
                    if (dir == TREE) {
                        amountOfNbTrees++;
                    } else if (dir == LUMBERYARD) {
                        amountOfNbLumberyards++;
                    }
                }

                if (type == EMPTY && amountOfNbTrees >= 3) {
                    type = TREE;
                } else if (type == TREE && amountOfNbLumberyards >= 3) {
                    type = LUMBERYARD;
                } else if (type == LUMBERYARD) {
                    if (!amountOfNbLumberyards || !amountOfNbTrees) {
                        type = EMPTY;
                    }
                }

                if (type == TREE) {
                    totalWood++;
                } else if (type == LUMBERYARD) {
                    totalLumberyard++;
                } else {
                }

                new_map[row][column] = type;
            }
        }

        for (int row = 0; row < 52; row++) {
            for (int column = 0; column < 52; column++) {
                map[row][column] = new_map[row][column];
            }
        }

        std::pair<int, int> item = std::make_pair(totalWood, totalLumberyard);

        /**
         * I'm kinda cheating here, but I'm looking for a pattern after X
         * iterations. After that, I assume everything stays the same, and
         * I can grab the result directly.
         */

        if (i == 1500) {
            startCounting = true;
        }

        if (i == 9) {
            std::cout << "Solution part 1: " << totalLumberyard * totalWood << std::endl;
        }

        if (startCounting && std::find(pattern.begin(), pattern.end(), item) != pattern.end()) {
            item = pattern.at((1000000000 - i - 1) % counting);

            std::cout << "Solution part 2: " << item.first * item.second << std::endl;
            break;
        } else {
            if (startCounting) {
                counting++;
                pattern.emplace_back(item);
            }
        }

        i++;
    }
}