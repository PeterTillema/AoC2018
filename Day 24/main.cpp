#include <iostream>
#include <vector>
#include <algorithm>

enum attackType {
    NOTHING,
    SLASHING,
    RADIATION,
    FIRE,
    BLUDGEONING,
    COLD
};

struct group {
    int units{0};
    int hitpoints{0};
    attackType immune1{NOTHING};
    attackType immune2{NOTHING};
    attackType immune3{NOTHING};
    attackType weak1{NOTHING};
    attackType weak2{NOTHING};
    int attackDamage{0};
    attackType attack{NOTHING};
    int initiative{0};
    int targetGroup{-1};
    bool beingAttacked{false};
};

void printGroup(std::vector<group> x) {
    std::cout << "Group:" << std::endl;
    for (auto a : x) {
        std::cout << "Units: " << a.units << ", attackGroup: " << a.targetGroup << ", initiative: " << a.initiative
                  << ", power: " << a.attackDamage << ", hitpoints: " << a.hitpoints << std::endl;
    }
}

void assignTargets(std::vector<group> &attackers, std::vector<group> &defenders) {
    for (auto &x : attackers) {
        int index{0};
        int maxDamage{0};

        for (auto &y : defenders) {
            // Attacking won't help if the defending group is immune to the attack weapon
            if (x.attack != y.immune1 && x.attack != y.immune2 && x.attack != y.immune3 && !y.beingAttacked) {
                int attackPower{x.units * x.attackDamage};

                // The defender is weak to this attack weapon
                if (x.attack == y.weak1 || x.attack == y.weak2) {
                    attackPower *= 2;
                }

                // Let's find out which target to attack!
                if (attackPower > maxDamage) {
                    x.targetGroup = index;
                    maxDamage = attackPower;
                } else if (attackPower == maxDamage) {
                    // Damage is equal, which means we want to select the enemy with the highest effective power
                    auto z = defenders[x.targetGroup];

                    if (y.units * y.attackDamage > z.units * z.attackDamage) {
                        x.targetGroup = index;
                    } else if (y.units * y.attackDamage == z.units * z.attackDamage) {
                        // Still a tie, sort by initative
                        if (y.initiative > z.initiative) {
                            x.targetGroup = index;
                        }
                    }
                }
            }

            index++;
        }

        // This enemy is already being attacked
        if (maxDamage) {
            (&defenders[x.targetGroup])->beingAttacked = true;
        }
    }
}

int main() {
    int bonusAttack{0};
    bool hasPart1{false};
    while (true) {
        std::vector<group> immuneSystem = {
                {1614, 8016, SLASHING,    NOTHING, NOTHING, RADIATION,   NOTHING, 48,  FIRE,      9},
                {3730, 5611, BLUDGEONING, NOTHING, NOTHING, FIRE,        NOTHING, 14,  RADIATION, 16},
                {1627, 9770, NOTHING,     NOTHING, NOTHING, COLD,        NOTHING, 55,  FIRE,      3},
                {4665, 9782, NOTHING,     NOTHING, NOTHING, FIRE,        NOTHING, 18,  RADIATION, 10},
                {281,  5764, FIRE,        NOTHING, NOTHING, RADIATION,   NOTHING, 187, SLASHING,  19},
                {524,  9344, NOTHING,     NOTHING, NOTHING, NOTHING,     NOTHING, 158, COLD,      15},
                {5013, 9768, NOTHING,     NOTHING, NOTHING, NOTHING,     NOTHING, 15,  COLD,      14},
                {1143, 1822, NOTHING,     NOTHING, NOTHING, RADIATION,   NOTHING, 15,  FIRE,      18},
                {136,  6830, NOTHING,     NOTHING, NOTHING, RADIATION,   NOTHING, 420, SLASHING,  7},
                {665,  7973, SLASHING,    NOTHING, NOTHING, BLUDGEONING, NOTHING, 119, FIRE,      11}
        };

        std::vector<group> infectionSystem = {
                {515,  8712,  RADIATION, NOTHING, NOTHING,   SLASHING, FIRE,      30,  COLD,        1},
                {5542, 56769, NOTHING,   NOTHING, NOTHING,   NOTHING,  NOTHING,   16,  BLUDGEONING, 4},
                {1663, 10437, SLASHING,  FIRE,    RADIATION, NOTHING,  NOTHING,   12,  RADIATION,   12},
                {574,  50124, NOTHING,   NOTHING, NOTHING,   SLASHING, RADIATION, 171, FIRE,        8},
                {1190, 10652, NOTHING,   NOTHING, NOTHING,   NOTHING,  NOTHING,   16,  COLD,        17},
                {3446, 23450, NOTHING,   NOTHING, NOTHING,   NOTHING,  NOTHING,   12,  FIRE,        5},
                {5887, 14556, NOTHING,   NOTHING, NOTHING,   SLASHING, NOTHING,   4,   RADIATION,   2},
                {1761, 41839, NOTHING,   NOTHING, NOTHING,   COLD,     NOTHING,   35,  COLD,        20},
                {4194, 16090, FIRE,      NOTHING, NOTHING,   SLASHING, NOTHING,   6,   FIRE,        6},
                {2127, 27065, NOTHING,   NOTHING, NOTHING,   COLD,     SLASHING,  24,  SLASHING,    13}
        };

        for (auto &a : immuneSystem) {
            a.attackDamage += bonusAttack;
        }

        while (!immuneSystem.empty() && !infectionSystem.empty()) {
            bool didMove{false};

            // Sort the teams by effective power / initiative
            std::sort(immuneSystem.begin(), immuneSystem.end(), [](const group& a, const group& b) {
                int c{a.units * a.attackDamage}, d{b.units * b.attackDamage};
                return (c > d) || ((c == d) && (a.initiative > b.initiative));
            });
            std::sort(infectionSystem.begin(), infectionSystem.end(), [](const group& a, const group& b) {
                int c{a.units * a.attackDamage}, d{b.units * b.attackDamage};
                return (c > d) || ((c == d) && (a.initiative > b.initiative));
            });

            // Assign the targets
            assignTargets(immuneSystem, infectionSystem);
            assignTargets(infectionSystem, immuneSystem);

            // Attack!
            for (int a = 20; a >= 1; a--) {
                for (auto &b : immuneSystem) {
                    b.beingAttacked = false;
                    if (b.initiative == a && b.targetGroup != -1) {
                        int damage{b.units * b.attackDamage};
                        auto& c = infectionSystem[b.targetGroup];

                        if (b.attack == c.weak1 || b.attack == c.weak2) {
                            damage *= 2;
                        }

                        int lostUnits = damage / c.hitpoints;
                        if (lostUnits >= c.units) {
                            c.units = 0;
                        } else {
                            c.units -= lostUnits;
                            if (lostUnits)
                                didMove = true;
                        }
                    }
                }

                for (auto &b : infectionSystem) {
                    b.beingAttacked = false;
                    if (b.initiative == a && b.targetGroup != -1) {
                        int damage{b.units * b.attackDamage};
                        auto& c = immuneSystem[b.targetGroup];

                        if (b.attack == c.weak1 || b.attack == c.weak2) {
                            damage *= 2;
                        }

                        int lostUnits = damage / c.hitpoints;

                        if (lostUnits >= c.units) {
                            c.units = 0;
                        } else {
                            if (lostUnits)
                                didMove = true;
                            c.units -= lostUnits;
                        }
                    }
                }
            }

            // Remove groups with 0 units
            for (auto it = immuneSystem.begin(); it < immuneSystem.end();) {
                (*it).targetGroup = -1;
                if (!(*it).units) {
                    it = immuneSystem.erase(it);
                } else
                    it++;
            }
            for (auto it = infectionSystem.begin(); it < infectionSystem.end();) {
                (*it).targetGroup = -1;
                if (!(*it).units) {
                    it = infectionSystem.erase(it);
                } else
                    it++;
            }

            // If no group can kill another unit anymore, we need to break, otherwise the program freezes,
            // because the teams are not empty, but they can't kill any unit while they want to.
            if (!didMove) {
                break;
            }
        }

        int sum{0};
        for (auto a : immuneSystem) {
            sum += a.units;
        }
        for (auto a : infectionSystem) {
            sum += a.units;
        }
        if (!hasPart1) {
            std::cout << "Solution part 1: " << sum << std::endl;
            hasPart1 = true;
        }
        if (infectionSystem.empty()) {
            std::cout << "Solution part 2: " << sum << std::endl;
            break;
        }
        bonusAttack++;
    }
}