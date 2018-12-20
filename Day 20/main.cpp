#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include <deque>

struct loc {
    int row{0};
    int col{0};
};

int main() {
    std::ifstream file{"../input.txt"};
    std::string input;
    std::map<int, std::map<int, int>> grid;
    std::deque<loc> stack;
    std::map<char, std::pair<int, int>> dirs = {
            {'N', {-1, 0}},
            {'E', {0, 1}},
            {'S', {1, 0}},
            {'W', {0, -1}},
    };
    int r{0}, c{0}, pr{0}, pc{0};

    getline(file, input);
    for (auto z : input) {
        if (z == 'N' || z == 'E' || z == 'S' || z == 'W') {
            r += dirs[z].first;
            c += dirs[z].second;
            grid[r][c] = (grid[r][c] ? std::min(grid[r][c], grid[pr][pc] + 1) : grid[pr][pc] + 1);
        } else if (z == '|') {
            auto x = stack.back();
            r = x.row;
            c = x.col;
        } else if (z == '(') {
            stack.push_back({r, c});
        } else if (z == ')') {
            auto x = stack.back();
            stack.pop_back();
            r = x.row;
            c = x.col;
        }

        pr = r;
        pc = c;
    }

    int max{0};
    int count{0};
    for (auto a : grid) {
        for (auto b : a.second) {
            max = std::max(max, b.second);
            if (b.second >= 1000)
                count++;
        }
    }

    std::cout << "Solution part 1: " << max << std::endl;
    std::cout << "Solution part 2: " << count << std::endl;
}