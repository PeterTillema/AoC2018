#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cmath>

struct instruction {
    int opcode{0};
    int a{0};
    int b{0};
    int c{0};
};

int getSum(int n) {
    // Sum of divisors
    int result{0};

    for (int i = 2; i <= sqrt(n); i++) {
        if (!(n % i)) {
            // If both divisors are the same, only add it once
            if (i == (n / i)) result += i;
            else result += (i + n/i);
        }
    }

    // Of course add n and 1 as well :)
    return result + n + 1;
}

int main() {
    std::ifstream file{"../input.txt"};
    std::string segment;
    int ipReg;
    int regs[6]{0};
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

    for (int i = 0; i < 2; i++) {
        regs[0] = i;
        regs[1] = regs[2] = regs[3] = regs[4] = regs[5] = 0;

        // This technically is a valid solution, except it's extremely slow.
        // I've converted the program to C code, and it seems to find the sum
        // of the factors of some input. I manually found the input number
        // and used Wolfram Alpha to find the correct solution. This program
        // still works though, though I cheated a bit. ;)
        //
        // C code:
        // r4 = (part 1 ? 958 : 10551358);
        // for (r5 = 0; r5 <= r4; r5++) {
        //   for (r3 = 0; r3 <= r4; r3++) {
        //     if (r3 * r5 == r4) {
        //       r0 += r5;
        //     }
        //   }
        // }
        //
        // return r0;
        //
        // ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
        // ░░░░░░░░░░░░░▄▄▄▄▄▄▄░░░░░░░░░
        // ░░░░░░░░░▄▀▀▀░░░░░░░▀▄░░░░░░░
        // ░░░░░░░▄▀░░░░░░░░░░░░▀▄░░░░░░
        // ░░░░░░▄▀░░░░░░░░░░▄▀▀▄▀▄░░░░░
        // ░░░░▄▀░░░░░░░░░░▄▀░░██▄▀▄░░░░
        // ░░░▄▀░░▄▀▀▀▄░░░░█░░░▀▀░█▀▄░░░
        // ░░░█░░█▄▄░░░█░░░▀▄░░░░░▐░█░░░
        // ░░▐▌░░█▀▀░░▄▀░░░░░▀▄▄▄▄▀░░█░░
        // ░░▐▌░░█░░░▄▀░░░░░░░░░░░░░░█░░
        // ░░▐▌░░░▀▀▀░░░░░░░░░░░░░░░░▐▌░
        // ░░▐▌░░░░░░░░░░░░░░░▄░░░░░░▐▌░
        // ░░▐▌░░░░░░░░░▄░░░░░█░░░░░░▐▌░
        // ░░░█░░░░░░░░░▀█▄░░▄█░░░░░░▐▌░
        // ░░░▐▌░░░░░░░░░░▀▀▀▀░░░░░░░▐▌░
        // ░░░░█░░░░░░░░░░░░░░░░░░░░░█░░
        // ░░░░▐▌▀▄░░░░░░░░░░░░░░░░░▐▌░░
        // ░░░░░█░░▀░░░░░░░░░░░░░░░░▀░░░
        // ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
        //

        while (regs[ipReg] < index) {
            instruction instruction = instructions[regs[ipReg]];
            int opcode{instruction.opcode}, a{instruction.a}, b{instruction.b}, c{instruction.c};

            if (regs[ipReg] == 1) {
                regs[0] = getSum(regs[4]);
                break;
            }

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
            if (opcode == 15) regs[c] = (regs[a] == regs[b]);
            regs[ipReg]++;
        }

        std::cout << "Solution part " << i + 1 << ": " << regs[0] << std::endl;
    }
}