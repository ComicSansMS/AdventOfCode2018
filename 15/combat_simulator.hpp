#ifndef ADVENT_OF_CODE_15_COMBAT_SIMULATOR_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_15_COMBAT_SIMULATOR_HPP_INCLUDE_GUARD

#include <iosfwd>
#include <optional>
#include <string_view>
#include <tuple>
#include <vector>

enum class Faction {
    Elf,
    Goblin
};
Faction operator!(Faction f);

struct Position {
    int x;
    int y;
};
bool operator==(Position const& lhs, Position const& rhs);
bool operator<(Position const& lhs, Position const& rhs);

struct Unit {
    int id;
    Faction faction;
    Position position;
    int hitpoints;
};
bool operator<(Unit const& lhs, Unit const& rhs);

struct PathHelper {
    int shortestDistance;
    Position previousField;
};

struct BattleStats {
    int turns_played;
    int hitpoints_remaining;
    int outcome;
};

struct Battlefield {
    std::vector<char> field;
    mutable std::vector<PathHelper> shortestPathHelper;
    int fieldWidth;
    int fieldHeight;
    std::vector<Unit> units;
    std::vector<Unit> graveyard;
    int nElves;
    int nGoblins;
    int elf_attack_power;

    char getField(int x, int y) const;

    std::optional<std::size_t> hasUnit(int x, int y) const;

    bool isInRangeFor(Faction f, int x, int y) const;

    std::vector<Position> shortestPaths(Unit const& u) const;

    bool attack(Unit const& u);

    bool playTurn();

    BattleStats simulateBattle(bool abort_if_elf_dies = false);
};

std::ostream& operator<<(std::ostream& os, Battlefield const& b);

Battlefield parseInput(std::string_view input);

BattleStats winWithElves(Battlefield const& b);

#endif
