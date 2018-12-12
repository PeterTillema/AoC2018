#include <iostream>
#include <map>

int main() {
    int serialInput{5235};
    int square[301][301]{0};

    for (int x = 1; x <= 300; x++) {
        for (int y = 1; y <= 300; y++) {
            int rackId{x+10};

            square[x][y] = (rackId * y + serialInput) * rackId / 100 % 10 - 5;
        }
    }

    int topLeftX{1};
    int topLeftY{1};
    int maxFuel{0};

    for (int x = 1; x <= 300 - 2; x++) {
        for (int y = 1; y <= 300 - 2; y++) {
            int fuel{square[x][y] + square[x][y+1] + square[x][y+2] +
                     square[x+1][y] + square[x+1][y+1] + square[x+1][y+2] +
                     square[x+2][y] + square[x+2][y+1] + square[x+2][y+2]};

            if (fuel > maxFuel) {
                maxFuel = fuel;
                topLeftX = x;
                topLeftY = y;
            }
        }
    }

    std::cout << "Solution part 1: " << topLeftX << "," << topLeftY << std::endl;

    maxFuel = 0;
    int topSize{0};

    for (int x = 1; x <= 300; x++) {
        for (int y = 1; y <= 300; y++) {
            int fuel{0};

            for (int size = 1; size <= 301 - std::max(x, y); size++) {
                for (int a = x; a < x + size; a++) {
                    fuel += square[a][y + size - 1];
                }
                for (int a = y; a < y + size - 1; a++) {
                    fuel += square[x + size - 1][a];
                }

                if (fuel > maxFuel) {
                    maxFuel = fuel;
                    topLeftX = x;
                    topLeftY = y;
                    topSize = size;
                }
            }
        }
    }

    std::cout << "Solution part 2: " << topLeftX << "," << topLeftY << "," << topSize << std::endl;
}