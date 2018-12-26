#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <queue>

struct bot {
    int x{0};
    int y{0};
    int z{0};
    int r{0};
};

std::vector<bot> bots;

int main() {
    std::ifstream file{"../input.txt"};
    std::string segment;
    bot largestBot;
    auto cmp = [](std::pair<int, int> left, std::pair<int, int> right) { return left.first > right.first;};
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, decltype(cmp)> queue(cmp);

    while (getline(file, segment)) {
        std::stringstream sss{segment};
        bot bot;
        char c;

        sss >> c >> c >> c >> c >> c >> bot.x >> c >> bot.y >> c >> bot.z >> c >> c >> c >> c >> bot.r;
        bots.push_back(bot);
        if (bot.r > largestBot.r) {
            largestBot = bot;
        }
        int d{abs(bot.x) + abs(bot.y) + abs(bot.z)};
        queue.push(std::make_pair(std::max(0, d - bot.r), 1));      // Closest distance of bot to origin
        queue.push(std::make_pair(d + bot.r + 1, -1));              // Most far distance of bot to origin
    }

    // Find the amount of bots within the range of the largest bot
    int sum{0};
    for (auto a : bots) {
        // Check if the bot is near the largest bot
        if (abs(largestBot.x - a.x) + abs(largestBot.y - a.y) + abs(largestBot.z - a.z) <= largestBot.r) {
            sum++;
        }
    }
    std::cout << "Solution part 1: " << sum << std::endl;

    // We translated all bots to a 1-dimensional space, where each start is the
    // minimum distance from the bot to the origin, counting as 1, and the end the
    // maximum distance from the bot to the origin, counting as -1. As we sort
    // automatically, we can find the distance with the largest count, i.e. the
    // the most bots that started but not finished yet. This only works if the
    // closest distance is on the edge of a bot though. In fact, all solutions
    // in between the start and the end of the line part with the most amount
    // of bots are 'valid'. It's a bit cheating, and it doesn't work always,
    // but who cares? :P
    int count{0};
    int maxCount{0};
    int best{0};
    while (!queue.empty()) {
        auto a = queue.top();
        queue.pop();

        count += a.second;
        if (count > maxCount) {
            best = a.first;
            maxCount = count;
        }
    }

    std::cout << "Solution part 2: " << best << std::endl;
}