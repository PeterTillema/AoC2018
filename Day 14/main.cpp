#include <iostream>
#include <vector>

int main() {
    uint8_t digits[6] = {2, 0, 9, 2, 3, 1};
    int digitIndex{0};
    int amountOfRecipes{209231};
    std::vector<int> recipes;
    bool hasSolutionPart2{false};

    recipes.push_back(3);
    recipes.push_back(7);

    unsigned int elf1index{0};
    unsigned int elf2index{1};

    while (recipes.size() < amountOfRecipes + 10 + 1 || !hasSolutionPart2) {
        int recipe1{recipes.at(elf1index)};
        int recipe2{recipes.at(elf2index)};
        int sum{recipe1 + recipe2};

        if (sum > 9) {
            recipes.push_back(sum / 10);

            if (sum / 10 == digits[digitIndex]) {
                if (digitIndex == 5) {
                    std::cout << "Solution part 2: " << recipes.size() - 6 << std::endl;
                    hasSolutionPart2 = true;
                }
                digitIndex++;
            } else {
                digitIndex = 0;
            }
        }
        recipes.push_back(sum % 10);
        if (sum % 10 == digits[digitIndex]) {
            if (digitIndex == 5) {
                std::cout << "Solution part 2: " << recipes.size() - 6 << std::endl;
                hasSolutionPart2 = true;
            }
            digitIndex++;
        } else {
            digitIndex = 0;
        }

        elf1index = (elf1index + 1 + recipe1) % recipes.size();
        elf2index = (elf2index + 1 + recipe2) % recipes.size();
    }

    std::cout << "Solution part 1: ";
    for (int i = amountOfRecipes; i < amountOfRecipes + 10; i++) {
        std::cout << recipes.at(i);
    }
    std::cout << std::endl;
}