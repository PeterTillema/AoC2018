#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <unordered_set>

struct instruction {
    int opcode{0};
    int a{0};
    int b{0};
    int c{0};
};

int main() {
    std::ifstream file{"../input.txt"};
    std::string segment;
    int ipReg;
    int regs[6] = {0};
    int index{0};
    instruction instructions[50];
    std::vector<std::string> opcodes = {
            "addr", "addi", "mulr", "muli", "banr", "bani", "borr", "bori",
            "setr", "seti", "gtir", "gtri", "gtrr", "eqir", "eqri", "eqrr"};

    getline(file, segment);
    std::stringstream sss{segment};

    sss >> segment >> ipReg;

    while (getline(file, segment)) {
        std::stringstream ss{segment};
        instruction instruction;

        ss >> segment >> instruction.a >> instruction.b >> instruction.c;
        instruction.opcode = std::find(opcodes.begin(), opcodes.end(), segment) - opcodes.begin();
        instructions[index++] = instruction;
    }

    int lastAnswer{INT_MAX};
    std::unordered_set<int> outcomes;
    while (regs[ipReg] < index) {
        instruction instruction = instructions[regs[ipReg]];
        int opcode{instruction.opcode}, a{instruction.a}, b{instruction.b}, c{instruction.c};

        if (opcode == 0) regs[c] = regs[a] + regs[b];
        if (opcode == 1) regs[c] = regs[a] + b;
        if (opcode == 2) regs[c] = regs[a] * regs[b];
        if (opcode == 3) regs[c] = regs[a] * b;
        if (opcode == 4) regs[c] = (regs[a] & regs[b]);
        if (opcode == 5) regs[c] = (regs[a] & b);
        if (opcode == 6) regs[c] = (regs[a] | regs[b]);
        if (opcode == 7) regs[c] = (regs[a] | b);
        if (opcode == 8) regs[c] = regs[a];
        if (opcode == 9) regs[c] = a;
        if (opcode == 10) regs[c] = (a > regs[b]);
        if (opcode == 11) regs[c] = (regs[a] > b);
        if (opcode == 12) regs[c] = (regs[a] > regs[b]);
        if (opcode == 13) regs[c] = (a == regs[b]);
        if (opcode == 14) regs[c] = (regs[a] == b);
        if (opcode == 15) {
            // eqrr, at this point we need to check the registers
            regs[c] = (regs[a] == regs[b]);
            int val{a ? regs[a] : regs[b]};

            if (lastAnswer == INT_MAX) {
                std::cout << "Solution part 1: " << val << std::endl;
            }
            if (outcomes.find(val) != outcomes.end()) {
                std::cout << "Solution part 2: " << lastAnswer << std::endl;
                break;
            }

            lastAnswer = val;
            outcomes.insert(val);
        }
        regs[ipReg]++;
    }
}