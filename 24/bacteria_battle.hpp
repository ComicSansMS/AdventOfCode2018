#ifndef ADVENT_OF_CODE_24_BACTERIA_BATTLE_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_24_BACTERIA_BATTLE_HPP_INCLUDE_GUARD

#include <optional>
#include <string>
#include <string_view>
#include <tuple>
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
    UnitStats stats;
    int units;
};

struct Battlefield {
    std::vector<Group> groups;
    std::vector<int> selected_targets;

    int effectivePower(int group_idx) const;
    int attackDamage(int attacker_idx, int target_idx) const;
    std::tuple<int, std::optional<int>> selectTarget(int attacker_idx) const;

    void targetSelection();
    bool attackPhase();

    int simulateBattle();
};

AttackType deserialize(std::string const& str);

std::vector<AttackType> parseWeaknesses(std::string const& str);
std::vector<AttackType> parseImmunities(std::string const& str);

Battlefield parseInput(std::string_view input);

#endif
