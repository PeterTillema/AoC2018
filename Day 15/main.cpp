#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
#include <set>
#include <deque>

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
using mapType = std::map<int, std::map<int, mapEntry>>;

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
    for (auto x : map) {
        std::stringstream general;

        for (auto y : x.second) {
            type z{y.second.type};

            if (z == WALL) {
                std::cout << "#";
            } else if (z == EMPTY) {
                std::cout << ".";
            } else if (z == ELF) {
                std::cout << "E";
                general << "E(" << y.second.hp << "), ";
            } else {
                std::cout << "G";
                general << "G(" << y.second.hp << "), ";
            }
        }
        std::cout << "    " << general.str() << "\n";
    }
    std::cout << std::endl;
}

bool move_attack(type enemyType, mapType& map, int row, int column, int attackPower) {
    if (map[row - 1][column].type != enemyType &&
        map[row][column - 1].type != enemyType &&
        map[row][column + 1].type != enemyType &&
        map[row + 1][column].type != enemyType) {
        // Generate da paths and move
        std::vector<location> targets;
        auto curr = map[row][column];

        for (auto a : map) {
            for (auto b : a.second) {
                if (b.second.type == enemyType) {
                    targets.emplace_back(std::make_pair(a.first, b.first));
                }
            }
        }

        std::set<location> seen;
        std::deque<std::pair<path, location>> queue;

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

            // Get all the directions
            // curLoc.first = row
            // curLoc.second = column
            location up = std::make_pair(curLoc.first - 1, curLoc.second);
            location left = std::make_pair(curLoc.first, curLoc.second - 1);
            location right = std::make_pair(curLoc.first, curLoc.second + 1);
            location down = std::make_pair(curLoc.first + 1, curLoc.second);

            for (auto dir : {up, left, right, down}) {
                if (findLocation(dir, targets)) {
                    map[row][column].type = EMPTY;
                    if (curPath.size() < 2) {
                        std::cout << "Error!" << std::endl;
                    } else {
                        auto d = curPath.at(1);
                        map[d.first][d.second] = curr;
                        row = d.first;
                        column = d.second;
                    }
                    goto stopsearching;
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
stopsearching:;
    }

    // Attack!!
    int minHp{INT_MAX};
    uint8_t enemy{0};

    if (map[row - 1][column].type == enemyType) {
        int hp{map[row - 1][column].hp};
        if (hp < minHp) {
            minHp = hp;
        }
    }
    if (map[row][column - 1].type == enemyType) {
        int hp{map[row][column - 1].hp};
        if (hp < minHp) {
            minHp = hp;
            enemy = 1;
        }
    }
    if (map[row][column + 1].type == enemyType) {
        int hp{map[row][column + 1].hp};
        if (hp < minHp) {
            minHp = hp;
            enemy = 2;
        }
    }
    if (map[row + 1][column].type == enemyType) {
        int hp{map[row + 1][column].hp};
        if (hp < minHp) {
            minHp = hp;
            enemy = 3;
        }
    }

    if (minHp != INT_MAX) {
        if (!enemy) {
            if (map[row - 1][column].attack(attackPower)) {
                return true;
            }
        } else if (enemy == 1) {
            if (map[row][column - 1].attack(attackPower)) {
                return true;
            }
        } else if (enemy == 2) {
            if (map[row][column + 1].attack(attackPower)) {
                return true;
            }
        } else {
            if (map[row + 1][column].attack(attackPower)) {
                return true;
            }
        }
    }

    return false;
}

int main() {
    std::ifstream file{"../input.txt"};
    std::string segment;
    mapType map;

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
            map[row2][column] = entry;
            column++;
        }
        row2++;
    }

    mapType prev_map = map;
    int amountOfElfesTemp{amountOfElfes};
    int amountOfGloblinsTemp{amountOfGloblins};

    for (int attackPower = 3; attackPower < 400; attackPower++) {
        bool hasSolutionPart2{true};
        bool needToDecrease{true};
        int rounds{0};

        map = prev_map;
        amountOfElfes = amountOfElfesTemp;
        amountOfGloblins = amountOfGloblinsTemp;

        while (amountOfElfes && amountOfGloblins) {
            rounds++;
            for (auto &x : map) {
                for (auto &y : x.second) {
                    y.second.hasMoved = false;
                }
            }
            int index{0};
            for (auto &row : map) {//y
                for (auto &column : row.second) {
                    type z{column.second.type};

                    if (!column.second.hasMoved) {
                        if (z == ELF) {
                            needToDecrease = true;
                            column.second.hasMoved = true;
                            if (move_attack(GLOBIN, map, row.first, column.first, attackPower)) {
                                amountOfGloblins--;
                                needToDecrease = amountOfGloblins || 0;
                            }
                        } else if (z == GLOBIN) {
                            needToDecrease = true;
                            column.second.hasMoved = true;
                            if (move_attack(ELF, map, row.first, column.first, 3)) {
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
        for (auto x : map) {
            for (auto y : x.second) {
                if (y.second.type > EMPTY) {
                    totalHp += y.second.hp;
                }
            }
        }

        if (attackPower == 3) {
            std::cout << "rounds: " << rounds - needToDecrease << " - total hp: " << totalHp << std::endl;
        }

        if (hasSolutionPart2) {
            std::cout << "rounds: " << rounds - needToDecrease << " - total hp: " << totalHp << " - attack power: " << attackPower << std::endl;
            break;
        }
    }
}