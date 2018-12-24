#ifndef ADVENT_OF_CODE_24_BACTERIA_BATTLE_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_24_BACTERIA_BATTLE_HPP_INCLUDE_GUARD

#include <string>
#include <string_view>
#include <vector>

enum class AttackType {
    Bludgeoning,
    Cold,
    Fire,
    Radiation,
    Slashing
};

enum class Faction {
    Immune,
    Infection
};

struct UnitStats {
    Faction faction;
    int hitPoints;
    int attackDamage;
    AttackType attackType;
    int initiative;
    std::vector<AttackType> weaknesses;
    std::vector<AttackType> immunities;
};

struct Group {
    const UnitStats stats;
    int units;
};

struct Battlefield {
    std::vector<Group> groups;
};

AttackType deserialize(std::string const& str);

std::vector<AttackType> parseWeaknesses(std::string const& str);
std::vector<AttackType> parseImmunities(std::string const& str);

Battlefield parseInput(std::string_view input);

#endif
