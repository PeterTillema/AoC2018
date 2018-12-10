#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

int metadata{0};

int parseNode(std::ifstream& file) {
    int _nodeValue{0}, amountOfChildren, _metaLength;
    file >> amountOfChildren >> _metaLength;

    std::vector<int> _children;
    for (int a = 0; a < amountOfChildren; a++)
        _children.push_back(parseNode(file));

    int metadataPrevious = metadata;
    for (int a = 0; a < _metaLength; a++) {
        unsigned int _metaValue;

        file >> _metaValue;
        metadata += _metaValue;
        if (_metaValue <= _children.size())
            _nodeValue += _children.at(_metaValue - 1);
    }
    if (!amountOfChildren)
        _nodeValue = metadata - metadataPrevious;

    return _nodeValue;
}

int main() {
    auto file = std::ifstream{"../input.txt"};

    int _nodeValue = parseNode(file);
    std::cout << "Solution part 1: " << metadata << std::endl;
    std::cout << "Solution part 2: " << _nodeValue << std::endl;
}