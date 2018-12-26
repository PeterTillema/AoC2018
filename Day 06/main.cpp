#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>

int main() {
    int locations[50][2] = {{174, 356},
                            {350, 245},
                            {149, 291},
                            {243, 328},
                            {312, 70},
                            {327, 317},
                            {46, 189},
                            {56, 209},
                            {84, 60},
                            {308, 202},
                            {289, 331},
                            {201, 139},
                            {354, 201},
                            {283, 130},
                            {173, 144},
                            {110, 280},
                            {242, 250},
                            {196, 163},
                            {217, 300},
                            {346, 188},
                            {329, 225},
                            {112, 275},
                            {180, 190},
                            {255, 151},
                            {107, 123},
                            {86, 304},
                            {236, 88},
                            {313, 124},
                            {297, 187},
                            {203, 289},
                            {104, 71},
                            {100, 151},
                            {227, 47},
                            {318, 293},
                            {268, 225},
                            {116, 49},
                            {222, 125},
                            {261, 146},
                            {47, 117},
                            {119, 214},
                            {183, 242},
                            {136, 210},
                            {91, 300},
                            {326, 237},
                            {144, 273},
                            {300, 249},
                            {200, 312},
                            {305, 50},
                            {235, 265},
                            {322, 291}};
    int area[50]{0};
    int totalArea{0};

    for (int x = -1500; x < 1900; x++) {
        for (int y = -1500; y < 1900; y++) {
            int minDistance{INT_MAX};
            int minNum{0};
            bool twoAreEqual{false};
            int totalDistance = 0;

            for (int num = 0; num < 50; num++) {
                int distance = abs(locations[num][0] - x) + abs(locations[num][1] - y);
                totalDistance += distance;

                if (distance < minDistance) {
                    minDistance = distance;
                    minNum = num;
                    twoAreEqual = false;
                } else if (distance == minDistance) {
                    twoAreEqual = true;
                }
            }

            if (totalDistance < 10000)
                totalArea++;

            if (!twoAreEqual)
                area[minNum]++;
        }
    }

    int maxArea{0};

    for (auto x : area) {
        if (x < 10000 && x > maxArea)
            maxArea = x;
    }

    std::cout << "Solution part 1: " << maxArea << std::endl;
    std::cout << "Solution part 2: " << totalArea << std::endl;
}