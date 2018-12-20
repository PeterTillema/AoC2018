#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
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

struct location {
    int row{0};
    int column{0};
};

using path = std::vector<location>;
using mapType = mapEntry[SIZE][SIZE];

bool findLocation(location loc, std::vector<location> vec) {
    for (auto c : vec) {
        if (c.row == loc.row && c.column == loc.column) {
            return true;
        }
    }

    return false;
}

void printPath(path path) {
    for (auto c : path) {
        std::cout << "(" << c.row << "," << c.column << ") - ";
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
                    location loc = {a, b};
                    targets.push_back(loc);
                }
            }
        }

        std::vector<location> seen;
        std::deque<std::pair<path, location>> queue;
        std::deque<std::pair<path, location>> solutions;
        int solDepth{0};
        bool foundSolution{false};

        location startPoint = {row, column};
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
            location up = {curLoc.row - 1, curLoc.column};
            location left = {curLoc.row, curLoc.column - 1};
            location right = {curLoc.row, curLoc.column + 1};
            location down = {curLoc.row + 1, curLoc.column};

            for (auto dir : {up, left, right, down}) {
                auto tempPath2 = curPath;
                tempPath2.push_back(dir);

                if (findLocation(dir, targets)) {
                    solutions.emplace_back(std::make_pair(tempPath2, dir));
                    solDepth = tempPath2.size();
                    foundSolution = true;
                } else {
                    if (map[dir.row][dir.column].type == EMPTY && !findLocation(dir, seen)) {
                        seen.push_back(dir);
                        queue.emplace_back(std::make_pair(tempPath2, dir));
                    }
                }
            }
        }

        if (foundSolution) {
            // Sort solutions by target, and grab the first in reading order
            auto x = std::min_element(solutions.begin(), solutions.end(),
                                      [](const std::pair<path, location>& a, const std::pair<path, location>& b){
                                          return a.second.row < b.second.row ||
                                                 (a.second.row == b.second.row && a.second.column < b.second.column);
                                      });

            map[row][column].type = EMPTY;

            // The new location is the second entry of the path
            row = x->first.at(1).row;
            column = x->first.at(1).column;
            map[row][column] = curr;
        }
    }

    // Attack!!
    int minHp{INT_MAX};

    auto up = &map[row - 1][column];
    auto left = &map[row][column - 1];
    auto right = &map[row][column + 1];
    auto down = &map[row + 1][column];
    auto dirRight = &map[0][0];

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