#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>

struct star {
    int x{0};
    int y{0};
    int dX{0};
    int dY{0};
};

using skyType = std::map<int, std::map<int, bool>>;

int getMinY(skyType map) {
    int minY{INT_MAX};

    for (auto i : map) for (auto j : i.second) minY = std::min(minY, j.first);
    return minY;
}

int getMaxY(skyType map) {
    int maxY{0};

    for (auto i : map) for (auto j : i.second) maxY = std::max(maxY, j.first);
    return maxY;
}

int getMinX(skyType map) {
    int minX{INT_MAX};

    for (auto i : map) minX = std::min(minX, i.first);
    return minX;
}

int getMaxX(skyType map) {
    int maxX{0};

    for (auto i : map) maxX = std::max(maxX, i.first);
    return maxX;
}

int main() {
    std::fstream file{"../input.txt"};
    std::string segment;
    std::vector<star> stars;

    while (getline(file, segment)) {
        std::stringstream sss{segment};
        star star;
        char c;

        getline(sss, segment, '<');
        sss >> star.x >> c >> star.y;
        getline(sss, segment, '<');
        sss >> star.dX >> c >> star.dY;

        stars.push_back(star);
    }

    skyType sky, skyPrev;
    int seconds{0}, prevRange{INT_MAX};

    /*
     * The algorithm is to get the sky with the smallest Y range as possible, then you're 99% sure the stars
     * form the word we're looking for; this is pretty easy to do, we fill the sky (map nested in a map),
     * fill it with the coordinates of the stars, and get the Y range. If this is MORE than the previous sky,
     * the previous sky contains the word we're looking for. Thus, we have to save the previous sky as well,
     * in order to draw it and get the word. If this is done, stop the program cuz we only want 1 word ;)
     */
    while (++seconds) {
        // Fill the light with blinking, twinkling stars..
        for (star i : stars) {
            sky[i.x + seconds * i.dX][i.y + seconds * i.dY] = true;
        }

        // Get the Y range
        int range{getMaxY(sky) - getMinY(sky)};

        // Check if it's more than the previous Y range
        if (range > prevRange) {
            // Draw the word
            std::cout << "Solution part 1:" << std::endl;
            for (int i = getMinY(skyPrev); i <= getMaxY(skyPrev); i++) {
                for (int j = getMinX(skyPrev); j <= getMaxX(skyPrev); j++) {
                    if (skyPrev[j][i]) std::cout << "#";
                    else std::cout << " ";
                }
                std::cout << std::endl;
            }

            std::cout << "Solution part 2: " << seconds - 1 << std::endl;
            break;
        }

        // Set the previous sky to be the current one, clear the sky and set the previous Y range
        skyPrev = sky;
        sky.clear();
        prevRange = range;
    }
}