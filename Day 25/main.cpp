#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

struct nanobot {
    int w{0};
    int x{0};
    int y{0};
    int z{0};
};

int dist(nanobot bot1, nanobot bot2) {
    return abs(bot1.w - bot2.w) + abs(bot1.x - bot2.x) + abs(bot1.y - bot2.y) + abs(bot1.z - bot2.z);
}

void print(std::vector<std::vector<nanobot>> cons) {
    for (auto a : cons) {
        for (auto b : a) {
            std::cout << "(" << b.w << "," << b.x << "," << b.y << "," << b.z << "), ";
        }
        std::cout << "\n";
    }
    std::cout << std::endl;
}

int main() {
    std::ifstream file{"../input.txt"};
    std::string segment;
    std::vector<std::vector<nanobot>> cons;

    while (getline(file, segment)) {
        std::stringstream sss{segment};
        std::vector<int> joined;
        nanobot bot;
        char c;

        sss >> bot.w >> c >> bot.x >> c >> bot.y >> c >> bot.z;
        for (int a = 0; a < cons.size(); a++) {
            for (auto b : cons[a]) {
                if (dist(bot, b) <= 3) {
                    cons[a].push_back(bot);
                    joined.push_back(a);
                    break;
                }
            }
        }

        if (joined.empty()) {
            // No close constellations
            cons.push_back({bot});
        } else {
            // Merge and delete all the constellations into a single one
            std::vector<nanobot> result;
            int index{0};
            int counter{0};

            for (auto it = cons.begin(); it != cons.end();) {
                if (index == joined[counter]) {
                    result.insert(result.end(), (*it).begin(), (*it).end());
                    it = cons.erase(it);
                    if (counter + 1 < joined.size()) {
                        counter++;
                    }
                } else {
                    ++it;
                }
                index++;
            }
            cons.push_back(result);
        }
    }

    std::cout << "Solution part 1: " << cons.size() << std::endl;
}