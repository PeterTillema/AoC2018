#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

std::vector<int> data;
unsigned int pos{0};
int metadata{0};

int parseNode() {
    int nodeValue{0};
    int amountOfChildren = data.at(pos++);
    int metadataLength = data.at(pos++);

    std::vector<int> childrenNodeValues;

    for (int a = 0; a < amountOfChildren; a++) {
        childrenNodeValues.push_back(parseNode());
    }

    int metadataPrevious = metadata;
    for (int a = 0; a < metadataLength; a++) {
        int metadataValue = data.at(pos++);

        metadata += metadataValue;
        if (metadataValue <= childrenNodeValues.size()) {
            nodeValue += childrenNodeValues.at(metadataValue - 1);
        }
    }
    if (!amountOfChildren) {
        nodeValue = metadata - metadataPrevious;
    }

    return nodeValue;
}

int main() {
    std::fstream file{"../input.txt"};
    std::string segment;

    getline(file, segment);
    std::stringstream line{segment};

    int number;
    while (line >> number) {
        data.push_back(number);
    }

    int solutionPart2 = parseNode();
    std::cout << "Solution part 1: " << metadata << std::endl;
    std::cout << "Solution part 2: " << solutionPart2 << std::endl;
}