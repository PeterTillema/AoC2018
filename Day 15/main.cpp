#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
#include <set>
#include <deque>
#include <algorithm>

#define SIZE 32

enum type {
    WALL,
    EMPTY,
    ELF,
    GLOBIN
};

struct mapEntry {
    type type{WALL};
    int hp{200};
    int x{0};
    int y{0};
    bool hasMoved{false};

    bool attack(int power) {
        hp -= power;
        if (hp <= 0) {
            type = EMPTY;
            return true;
        }
        return false;
    }
};

using location = std::pair<int, int>;
using path = std::vector<location>;
using mapType = mapEntry[SIZE][SIZE];

bool findLocation(location loc, std::vector<location> vec) {
    for (auto c : vec) {
        if (loc == c) {
            return true;
        }
    }

    return false;
}

void printPath(path path) {
    for (auto c : path) {
        std::cout << "(" << c.first << "," << c.second << ") - ";
    }
    std::cout << std::endl;
}

void printMap(mapType map) {
    for (int row = 0; row < SIZE; row++) {
        std::stringstream general;

        for (int column = 0; column < SIZE; column++) {
            type z{map[row][column].type};

            if (z == WALL) {
                std::cout << "#";
            } else if (z == EMPTY) {
                std::cout << ".";
            } else if (z == ELF) {
                std::cout << "E";
                general << " E" << map[row][column].hp;
            } else {
                std::cout << "G";
                general << " G" << map[row][column].hp;
            }
        }
        std::cout << general.str() << "\n";
    }
    std::cout << std::endl;
}

bool move_attack(type enemyType, mapType map, int row, int column, int attackPower) {
    if (map[row - 1][column].type != enemyType &&
        map[row][column - 1].type != enemyType &&
        map[row][column + 1].type != enemyType &&
        map[row + 1][column].type != enemyType) {
        // Generate da paths and move
        std::vector<location> targets;
        auto curr = map[row][column];

        for (int a = 0; a < SIZE; a++) {
            for (int b = 0; b < SIZE; b++) {
                if (map[a][b].type == enemyType) {
                    targets.emplace_back(std::make_pair(a, b));
                }
            }
        }

        std::set<location> seen;
        std::deque<std::pair<path, location>> queue;
        std::deque<std::pair<path, location>> solutions;
        int solDepth{0};
        bool foundSolution{false};

        location startPoint = std::make_pair(row, column);
        path tempPath;

        tempPath.push_back(startPoint);
        queue.emplace_back(std::make_pair(tempPath, startPoint));

        while (!queue.empty()) {
            // Get first entry
            auto c = queue.front();
            auto curPath = c.first;
            auto curLoc = c.second;

            queue.pop_front();

            if (foundSolution && curPath.size() >= solDepth) {
                break;
            }

            // Get all the directions
            // curLoc.first = row
            // curLoc.second = column
            location up = std::make_pair(curLoc.first - 1, curLoc.second);
            location left = std::make_pair(curLoc.first, curLoc.second - 1);
            location right = std::make_pair(curLoc.first, curLoc.second + 1);
            location down = std::make_pair(curLoc.first + 1, curLoc.second);

            for (auto dir : {up, left, right, down}) {
                auto tempPath2 = curPath;
                tempPath2.push_back(dir);

                if (findLocation(dir, targets)) {
                    solutions.emplace_back(std::make_pair(tempPath2, dir));
                    solDepth = tempPath2.size();
                    foundSolution = true;
                } else {
                    if (map[dir.first][dir.second].type == EMPTY && !seen.count(dir)) {
                        auto tempPath2 = curPath;
                        tempPath2.push_back(dir);
                        seen.insert(dir);
                        queue.emplace_back(std::make_pair(tempPath2, dir));
                    }
                }
            }
        }

        if (foundSolution) {
            // Sort solutions by target, and grab the first in reading order
            auto x = std::min_element(solutions.begin(), solutions.end(),
                                      [](const std::pair<path, location>& a, const std::pair<path, location>& b){
                                          return a.second.first < b.second.first ||
                                                 (a.second.first == b.second.first && a.second.second < b.second.second);
                                      });

            map[row][column].type = EMPTY;

            // The new location is the second entry of the path
            row = x->first.at(1).first;
            column = x->first.at(1).second;
            map[row][column] = curr;
        }
    }

    // Attack!!
    int minHp{INT_MAX};

    auto up = &map[row - 1][column];
    auto left = &map[row][column - 1];
    auto right = &map[row][column + 1];
    auto down = &map[row + 1][column];
    auto dirRight = &map[row][column];

    for (auto dir : {up, left, right, down}) {
        if (dir->type == enemyType) {
            int hp{dir->hp};

            if (hp < minHp) {
                minHp = hp;
                dirRight = dir;
            }
        }
    }

    if (minHp != INT_MAX) {
        if (dirRight->attack(attackPower)) {
            return true;
        }
    }

    return false;
}

int main() {
    std::ifstream file{"../input.txt"};
    std::string segment;
    mapType map;
    mapType prev_map;

    uint8_t row2{0};
    int amountOfElfes{0};
    int amountOfGloblins{0};
    while (getline(file, segment)) {
        uint8_t column{0};

        for (auto c : segment) {
            mapEntry entry;

            entry.x = column;
            entry.y = row2;

            if (c == '.') {
                entry.type = EMPTY;
            } else if (c == '#') {
                entry.type = WALL;
            } else if (c == 'E') {
                entry.type = ELF;
                amountOfElfes++;
            } else {
                entry.type = GLOBIN;
                amountOfGloblins++;
            }
            prev_map[row2][column] = entry;
            column++;
        }
        row2++;
    }

    int amountOfElfesTemp{amountOfElfes};
    int amountOfGloblinsTemp{amountOfGloblins};

    for (int attackPower = 3; attackPower < 100; attackPower++) {
        bool hasSolutionPart2{true};
        bool needToDecrease{true};
        int rounds{0};

        for (int a = 0; a < SIZE; a++) {
            for (int b = 0; b < SIZE; b++) {
                map[a][b] = prev_map[a][b];
            }
        }
        amountOfElfes = amountOfElfesTemp;
        amountOfGloblins = amountOfGloblinsTemp;

        while (amountOfElfes && amountOfGloblins) {
            rounds++;
            for (int row = 0; row < SIZE; row++) {
                for (int column = 0; column < SIZE; column++) {
                    map[row][column].hasMoved = false;
                }
            }
            int index{0};
            for (int row = 0; row < SIZE; row++) {
                for (int column = 0; column < SIZE; column++) {
                    type z{map[row][column].type};

                    if (!map[row][column].hasMoved) {
                        map[row][column].hasMoved = true;
                        if (z == ELF) {
                            needToDecrease = true;
                            if (move_attack(GLOBIN, map, row, column, attackPower)) {
                                amountOfGloblins--;
                                needToDecrease = amountOfGloblins || 0;
                            }
                        } else if (z == GLOBIN) {
                            needToDecrease = true;
                            if (move_attack(ELF, map, row, column, 3)) {
                                amountOfElfes--;
                                needToDecrease = amountOfElfes || 0;
                                hasSolutionPart2 = false;
                            }
                        }
                    }
                }
            }
        }

        int totalHp{0};
        for (int row = 0; row < SIZE; row++) {
            for (int column = 0; column < SIZE; column++) {
                if (map[row][column].type > EMPTY) {
                    totalHp += map[row][column].hp;
                }
            }
        }

        if (attackPower == 3) {
            std::cout << "Solution part 1: " << (rounds - needToDecrease) * totalHp << std::endl;
        }

        if (hasSolutionPart2) {
            std::cout << "Solution part 2: " << (rounds - needToDecrease) * totalHp << std::endl;
            break;
        }
    }
}