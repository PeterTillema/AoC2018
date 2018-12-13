#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>

enum direction {
    UP = 0,
    RIGHT,
    DOWN,
    LEFT
};

struct car {
    int id{0};
    int x{0};
    int y{0};
    int dir{UP};
    int intersectionIndex{0};
    bool dead{false};
};

int main() {
    std::ifstream file{"../input.txt"};
    std::string dataLine;
    std::map<int, std::map<int, uint8_t>> map;
    std::map<int, std::map<int, bool>> carMap;
    std::vector<car> cars;
    int row{0};
    int carId{1};

    while (getline(file, dataLine)) {
        int column{0};

        for (auto c : dataLine) {
            car newCar;

            newCar.id = carId;
            newCar.x = column;
            newCar.y = row;

            if (c == '>') {
                newCar.dir = RIGHT;
                cars.push_back(newCar);
                carMap[column][row] = true;
                carId++;
            } else if (c == '<') {
                newCar.dir = LEFT;
                cars.push_back(newCar);
                carMap[column][row] = true;
                carId++;
            } else if (c == '^') {
                newCar.dir = UP;
                cars.push_back(newCar);
                carMap[column][row] = true;
                carId++;
            } else if (c == 'v') {
                newCar.dir = DOWN;
                cars.push_back(newCar);
                carMap[column][row] = true;
                carId++;
            } else if (c == '/') {
                map[column][row] = 1;
            } else if (c == '\\') {
                map[column][row] = 2;
            } else if (c == '+') {
                map[column][row] = 3;
            }

            column++;
        }
        row++;
    }

    unsigned int amountOfCarsLeft{cars.size()};
    bool hasSolution1{false};

    while (amountOfCarsLeft > 1) {
        std::sort(cars.begin(), cars.end(), [](const car& r1, const car& r2) {
            return (r1.x < r2.x) || (r1.x == r2.x && r1.y < r2.y);
        });

        for (auto& car : cars) {
            if (!car.dead) {
                carMap[car.x][car.y] = false;

                if (car.dir == UP) {
                    car.y--;
                } else if (car.dir == RIGHT) {
                    car.x++;
                } else if (car.dir == DOWN) {
                    car.y++;
                } else {
                    car.x--;
                }

                if (carMap[car.x][car.y]) {
                    if (!hasSolution1) {
                        std::cout << "Solution part 1: " << car.x << "," << car.y << std::endl;
                        hasSolution1 = true;
                    }
                    for (auto& car2 : cars) {
                        if (!car2.dead && car.x == car2.x && car.y == car2.y) {
                            car2.dead = true;
                            amountOfCarsLeft--;
                        }
                    }
                    carMap[car.x][car.y] = false;
                } else {
                    carMap[car.x][car.y] = true;
                }

                uint8_t command{map[car.x][car.y]};
                if (command == 1) {
                    car.dir ^= 1;
                } else if (command == 2) {
                    car.dir ^= 3;
                } else if (command == 3) {
                    car.dir = (car.dir + car.intersectionIndex + 3) % 4;
                    car.intersectionIndex = (car.intersectionIndex + 1) % 3;
                }
            }
        }
    }

    for (auto& car : cars) {
        if (!car.dead) {
            std::cout << "Solution part 2: " << car.x << "," << car.y << std::endl;
        }
    }
}