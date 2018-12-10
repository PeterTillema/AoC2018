#include <iostream>
#include <map>
#include <algorithm>

struct node {
    node* prev;
    node* next;
    unsigned long long data;
};

class Marbles {
private:
    node* curr = nullptr;

public:
    void insert(unsigned long long data) {
        node* n = new node;
        n->data = data;
        if (curr == nullptr) {
            n->prev = n->next = n;
        } else {
            n->prev = curr->prev;
            n->next = curr;
            n->prev->next = n->next->prev = n;
        }
        curr = n;
    }

    unsigned long long remove() {
        unsigned long long data = curr->data;
        if (curr->next == curr) {
            delete std::exchange(curr, nullptr);
        } else {
            curr->prev->next = curr->next;
            curr->next->prev = curr->prev;
            delete std::exchange(curr, curr->next);
        }
        return data;
    }

    void next(int pos = 1) {
        while (pos--) curr = curr->next;
    }

    void prev(int pos = 1) {
        while (pos--) curr = curr->prev;
    }
};

unsigned long long getMaxValue(int amountOfMarbles, int amountOfPlayers) {
    int currentPos{0};
    int currentPlayer{0};

    Marbles marbles;
    std::map<int, unsigned long long> scores;

    marbles.insert(0);
    for (unsigned long long i = 1; i <= amountOfMarbles; i++) {
        currentPlayer = (currentPlayer % amountOfPlayers) + 1;
        if (i % 23) {
            marbles.next(2);
            marbles.insert(i);
        } else {
            marbles.prev(7);
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