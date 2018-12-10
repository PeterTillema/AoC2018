#include <iostream>
#include <map>
#include <algorithm>

struct node {
    node* _prev;
    node* _next;
    unsigned long data;
};

class marbles {
private:
    node* _curr = nullptr;

public:
    void insert(unsigned long data) {
        node* _node = new node;

        _node->data = data;
        if (_curr == nullptr) {
            _node->_prev = _node->_next = _node;
        } else {
            _node->_prev = _curr->_prev;
            _node->_next = _curr;
            _node->_prev->_next = _node->_next->_prev = _node;
        }
        _curr = _node;
    }

    unsigned long remove() {
        unsigned long data = _curr->data;

        _curr->_prev->_next = _curr->_next;
        _curr->_next->_prev = _curr->_prev;
        delete std::exchange(_curr, _curr->_next);

        return data;
    }

    void advance() {
        _curr = _curr->_next->_next;
    }

    void back() {
        int pos{7};
        while (pos--)
            _curr = _curr->_prev;
    }
};

unsigned long getMaxValue(int amountOfMarbles, int amountOfPlayers) {
    int currentPos{0}, currentPlayer{0};
    marbles marbles;
    std::map<int, unsigned long> scores;

    marbles.insert(0);
    for (unsigned long i = 1; i <= amountOfMarbles; i++) {
        currentPlayer = (currentPlayer % amountOfPlayers) + 1;
        if (i % 23) {
            marbles.advance();
            marbles.insert(i);
        } else {
            marbles.back();
            scores[currentPlayer] += i + marbles.remove();
        }
    }

    auto y = std::max_element(scores.begin(), scores.end(),
                              [](const std::pair<int, int>& p1, const std::pair<int, int>& p2) {
                                  return p1.second < p2.second; });

    return y->second;
}

int main() {
    int amountOfPlayers{458};
    int amountOfMarbles{72019};

    std::cout << "Solution part 1: " << getMaxValue(amountOfMarbles, amountOfPlayers) << std::endl;
    std::cout << "Solution part 2: " << getMaxValue(amountOfMarbles * 100, amountOfPlayers) << std::endl;
}