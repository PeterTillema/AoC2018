#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>

enum tileType {
    EMPTY = 0,
    FLUID_WATER,
    CLAY,
    SOLID_WATER,
    DROP
};

using mapType = std::map<int, std::map<int, tileType>>;

int getMinY(mapType map) {
    int a{INT_MAX};
    for (auto c : map) {
        if (c.first < a) {
            a = c.first;
        }
    }
    return a;
}

int getMaxY(mapType map) {
    int a{0};
    for (auto c : map) {
        if (c.first > a) {
            a = c.first;
        }
    }
    return a;
}

int maxY{0};

std::map<int, std::map<int, bool>> done;

std::pair<int, tileType> lookup(int row, int column, int direction, mapType map) {
    std::pair<int, tileType> output;
    int leftX{0}, rightX{0};
    while (true) {
        column += direction;
        output.first = column;

        if (map[row + 1][column] != CLAY && map[row + 1][column] != SOLID_WATER) {
            output.second = DROP;
            return output;
        } else if (map[row][column] == CLAY) {
            output.second = CLAY;
            return output;
        }
    }
}

void flowWater(int row, int column, mapType& map) {
    int rowStart{row};
    int columnStart{column};

    if (done[row][column]) {
        return;
    }
    done[row][column] = true;

    bool foundBottom{false};
    int obstacle{0};
    for (int i = row; i <= maxY; i++) {
        if (map[i][column] == CLAY || map[i][column] == SOLID_WATER) {
            foundBottom = true;
            obstacle = i;
            break;
        }
    }

    if (!foundBottom) {
        for (int i = rowStart; i <= maxY; i++) {
            map[i][column] = FLUID_WATER;
        }
        return;
    }

    for (int i = rowStart; i < obstacle; i++) {
        map[i][column] = FLUID_WATER;
    }

    row = obstacle - 1;
    while (true) {
        std::pair<int, tileType> left = lookup(row, column, -1, map);
        int leftX{left.first};
        tileType leftType{left.second};

        std::pair<int, tileType> right = lookup(row, column, 1, map);
        int rightX{right.first};
        tileType rightType{right.second};

        if (leftType == CLAY && rightType == CLAY) {
            for (int i = leftX + 1; i < rightX; i++) {
                map[row][i] = SOLID_WATER;
            }
        } else {
            for (int i = leftX + 1; i < rightX; i++) {
                map[row][i] = FLUID_WATER;
            }
            if (leftType == DROP) {
                flowWater(row, leftX, map);
            }
            if (rightType == DROP) {
                flowWater(row, rightX, map);
            }
            break;
        }
        row--;
    }
}

int main() {
    std::ifstream file{"../input.txt"};
    std::string segment;
    mapType map;

    while (getline(file, segment)) {
        char a, b, c;
        int value1, value2, value3;

        std::stringstream sss{segment};
        sss >> a >> c >> value1 >> c >> b >> c >> value2 >> c >> c >> value3;

        if (a == 'x') {
            for (int i = value2; i <= value3; i++) {
                map[i][value1] = CLAY;
            }
        } else {
            for (int i = value2; i <= value3; i++) {
                map[value1][i] = CLAY;
            }
        }
    }

    maxY = getMaxY(map);
    int minY{getMinY(map)};

    flowWater(0, 500, map);

    int solidCount{0};
    int fluidCount{0};
    for (auto c : map) {
        for (auto d : c.second) {
            if (c.first >= minY) {
                if (d.second == FLUID_WATER) {
                    solidCount++;
                }
                if (d.second == SOLID_WATER) {
                    fluidCount++;
                    solidCount++;
                }
            }
        }
    }

    std::cout << "Solution part 1: " << solidCount << std::endl;
    std::cout << "Solution part 2: " << fluidCount << std::endl;
}