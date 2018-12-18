#include <iostream>
#include <fstream>
#include <sstream>

int main() {
    std::ifstream file{"../input1.txt"};
    std::string segment;

    bool opcode_numbers[16][16];

    for (int a = 0; a < 16; a++) {
        for (int b = 0; b < 16; b++) {
            opcode_numbers[a][b] = true;
        }
    }

    int total{0};
    while (getline(file, segment)) {
        int registers[4]{0};
        int output[4]{0};
        int opcode, a, b, c;
        char d;

        std::stringstream sss{segment};
        sss >> segment >> d >> registers[0] >> d >> registers[1] >> d >> registers[2] >> d >> registers[3];

        getline(file, segment);
        std::stringstream sss2{segment};
        sss2 >> opcode >> a >> b >> c;

        getline(file, segment);
        std::stringstream sss3{segment};
        sss3 >> segment >> d >> output[0] >> d >> output[1] >> d >> output[2] >> d >> output[3];

        getline(file, segment);

        int amount{16};
        // addr
        if (output[c] != (registers[a] + registers[b])) {
            amount--;
            opcode_numbers[opcode][0] = false;
        }
        // addi
        if (output[c] != (registers[a] + b)) {
            amount--;
            opcode_numbers[opcode][1] = false;
        }
        // mulr
        if (output[c] != (registers[a] * registers[b])) {
            amount--;
            opcode_numbers[opcode][2] = false;
        }
        // muli
        if (output[c] != (registers[a] * b)) {
            amount--;
            opcode_numbers[opcode][3] = false;
        }
        // banr
        if (output[c] != (registers[a] & registers[b])) {
            amount--;
            opcode_numbers[opcode][4] = false;
        }
        // bani
        if (output[c] != (registers[a] & b)) {
            amount--;
            opcode_numbers[opcode][5] = false;
        }
        // borr
        if (output[c] != (registers[a] | registers[b])) {
            amount--;
            opcode_numbers[opcode][6] = false;
        }
        // bori
        if (output[c] != (registers[a] | b)) {
            amount--;
            opcode_numbers[opcode][7] = false;
        }
        // setr
        if (output[c] != registers[a]) {
            amount--;
            opcode_numbers[opcode][8] = false;
        }
        // seti
        if (output[c] != a) {
            amount--;
            opcode_numbers[opcode][9] = false;
        }
        // gtir
        if (output[c] != (a > registers[b])) {
            amount--;
            opcode_numbers[opcode][10] = false;
        }
        // gtri
        if (output[c] != (registers[a] > b)) {
            amount--;
            opcode_numbers[opcode][11] = false;
        }
        // gtrr
        if (output[c] != (registers[a] > registers[b])) {
            amount--;
            opcode_numbers[opcode][12] = false;
        }
        // eqir
        if (output[c] != (a == registers[b])) {
            amount--;
            opcode_numbers[opcode][13] = false;
        }
        // eqri
        if (output[c] != (registers[a] == b)) {
            amount--;
            opcode_numbers[opcode][14] = false;
        }
        // eqrr
        if (output[c] != (registers[a] == registers[b])) {
            amount--;
            opcode_numbers[opcode][15] = false;
        }

        if (amount > 2) {
            total++;
        }
    }

    std::cout << "Solution part 1: " << total << std::endl;

    // Start assigning variables
    bool assigned{true};
    while (assigned) {
        assigned = false;
        for (int i = 0; i < 16; i++) {
            int amount{0};
            int foundRow{0};
            int foundColumn{0};
            for (int j = 0; j < 16; j++) {
                if (opcode_numbers[i][j]) {
                    amount++;
                    foundRow = i;
                    foundColumn = j;
                }
            }
            if (amount == 1) {
                for (int k = 0; k < 16; k++) {
                    if (k != foundRow) {
                        if (opcode_numbers[k][foundColumn]) {
                            assigned = true;
                            opcode_numbers[k][foundColumn] = false;
                        }
                    }
                }
            }
        }
    }

    std::ifstream file2{"../input2.txt"};
    int regs[4]{0};

    while (getline(file2, segment)) {
        std::stringstream ss{segment};
        int opcode, a, b, c;

        ss >> opcode >> a >> b >> c;
        if (opcode_numbers[opcode][0]) regs[c] = regs[a] + regs[b];
        if (opcode_numbers[opcode][1]) regs[c] = regs[a] + b;
        if (opcode_numbers[opcode][2]) regs[c] = regs[a] * regs[b];
        if (opcode_numbers[opcode][3]) regs[c] = regs[a] * b;
        if (opcode_numbers[opcode][4]) regs[c] = (regs[a] & regs[b]);
        if (opcode_numbers[opcode][5]) regs[c] = (regs[a] & b);
        if (opcode_numbers[opcode][6]) regs[c] = (regs[a] | regs[b]);
        if (opcode_numbers[opcode][7]) regs[c] = (regs[a] | b);
        if (opcode_numbers[opcode][8]) regs[c] = regs[a];
        if (opcode_numbers[opcode][9]) regs[c] = a;
        if (opcode_numbers[opcode][10]) regs[c] = (a > regs[b]);
        if (opcode_numbers[opcode][11]) regs[c] = (regs[a] > b);
        if (opcode_numbers[opcode][12]) regs[c] = (regs[a] > regs[b]);
        if (opcode_numbers[opcode][13]) regs[c] = (a == regs[b]);
        if (opcode_numbers[opcode][14]) regs[c] = (regs[a] == b);
        if (opcode_numbers[opcode][15]) regs[c] = (regs[a] == regs[b]);
    }

    std::cout << "Solution part 2: " << regs[0] << std::endl;
}