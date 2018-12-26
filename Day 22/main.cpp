#include <iostream>
#include <map>
#include <vector>
#include <queue>

#define SIZE 900

using location = std::pair<int, int>;
using entry = std::pair<location, int>;
using queueEntry = std::pair<int, entry>;

int level[SIZE][SIZE];

int main() {
    int depth{11991};
    int targetX{6};
    int targetY{797};

    for (int x = 0; x < SIZE; x++) {
        for (int y = 0; y < SIZE; y++) {
            int eIndex{0};

            if (x == targetX && y == targetY) {
                eIndex = 0;
            } else if (!x) {
                eIndex = y * 48271;
            } else if (!y) {
                eIndex = x * 16807;
            } else {
                eIndex = level[x - 1][y] * level[x][y - 1];
            }
            level[x][y] = (eIndex + depth) % 20183;
        }
    }

    int sum{0};
    for (int x = 0; x <= targetX; x++) {
        for (int y = 0; y <= targetY; y++) {
            sum += level[x][y] % 3;
        }
    }
    std::cout << "Solution part 1: " << sum << std::endl;

    std::map<entry, int> dist;
    std::priority_queue<queueEntry, std::vector<queueEntry>, std::greater<>> queue;

    queue.push({0, {{0, 0}, 0}});
    dist[{{0, 0}, 0}] = 0;

    while (!queue.empty()) {
        auto curr{queue.top()};
        int currDist{curr.first};
        int x{curr.second.first.first};
        int y{curr.second.first.second};
        int equip{curr.second.second};

        queue.pop();

        if (currDist > dist[curr.second])
            continue;

        if (curr.second == make_pair(location(targetX, targetY), 0)) {
            std::cout << "Solution part 2: " << dist[curr.second] << std::endl;
            break;
        }

        location up{x, y - 1};
        location left{x - 1, y};
        location right{x + 1, y};
        location down{x, y + 1};

        for (location dir : {up, left, right, down}) {
            int newX{dir.first};
            int newY{dir.second};

            if (newX >= 0 && newY >= 0 && level[newX][newY] % 3 != (equip + 1) % 3) {
                int newCosts{dist[curr.second] + 1};
                std::pair<location, int> next = {dir, equip};

                if (!dist.count(next) || dist[next] > newCosts) {
                    dist[next] = newCosts;
                    queue.push({newCosts, next});
                }
            }
        }
        for (int i = 0; i < 3; i++) {
            if (i != equip && i != ((level[x][y] % 3) + 2) % 3) {
                int newCosts{dist[curr.second] + 7};
                std::pair<location, int> next = {{x, y}, i};

                if (!dist.count(next) || dist[next] > newCosts) {
                    dist[next] = newCosts;
                    queue.push({newCosts, next});
                }
            }
        }
    }
}