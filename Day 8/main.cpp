#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

unsigned int pos{0};
int metadata{0};

int parseNode(std::ifstream& file) {
    int val{0}, amountOfChildren, metadataLength;
    file >> amountOfChildren >> metadataLength;

    std::vector<int> childrenNodeValues;
    for (int a = 0; a < amountOfChildren; a++) {
        childrenNodeValues.push_back(parseNode(file));
    }

    int metadataPrevious = metadata;
    for (int a = 0; a < metadataLength; a++) {
        int metadataValue;

        file >> metadataValue;
        metadata += metadataValue;
        if (metadataValue <= childrenNodeValues.size()) {
            val += childrenNodeValues.at(metadataValue - 1);
        }
    }
    if (!amountOfChildren) {
        val = metadata - metadataPrevious;
    }

    return val;
}

int main() {
    auto file = std::ifstream{"../input.txt"};

    int solutionPart2 = parseNode(file);
    std::cout << "Solution part 1: " << metadata << std::endl;
    std::cout << "Solution part 2: " << solutionPart2 << std::endl;
}