#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

struct rectangle {
    int x{0};
    int y{0};
    int w{0};
    int h{0};
};

int main() {
    std::fstream file{"../input.txt"};
    std::vector<rectangle> rectangles;
    std::string segment, str;
    uint8_t claims[1001][1001]{0};
    char c;
    int count{0};
    rectangle rect;

    while (getline(file, segment)) {
        std::stringstream sss{segment};

        sss >> str >> c >> rect.x >> c >> rect.y >> c >> rect.w >> c >> rect.h;
        for (int a = rect.x; a < rect.x + rect.w; a++) {
            for (int b = rect.y; b < rect.y + rect.h; b++) {
                if (claims[a][b] != 2) {
                    claims[a][b]++;
                    if (claims[a][b] == 2) {
                        count++;
                    }
                }
            }
        }

        rectangles.push_back(rect);
    }

    std::cout << "Solution part 1: " << count << std::endl;

    int id{1};
    for (auto rect2 : rectangles) {
        bool hasFound{true};

        for (int a = rect2.x; a < rect2.x + rect2.w; a++) {
            for (int b = rect2.y; b < rect2.y + rect2.h; b++) {
                if (claims[a][b] == 2) {
                    hasFound = false;
                }
            }
        }

        if (hasFound) {
            std::cout << "Solution part 2: " << id << std::endl;
        }

        id++;
    }
}