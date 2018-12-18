#ifndef ADVENT_OF_CODE_18_GAME_OF_WOODS_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_18_GAME_OF_WOODS_HPP_INCLUDE_GUARD

#include <chrono>
#include <iosfwd>
#include <optional>
#include <string_view>
#include <tuple>
#include <vector>

enum class Field {
    Open,
    Trees,
    Yard
};

struct Acre {
    std::vector<Field> m_field;
    int m_width;
    int m_height;

    Field get(int x, int y) const;

    bool openCondition(int x, int y) const;
    bool treeCondition(int x, int y) const;
    bool yardCondition(int x, int y) const;

    int totalResourceValue() const;
};

Acre parseInput(std::string_view input);

std::ostream& operator<<(std::ostream& os, Acre const& a);

Acre step(Acre const& a);

int getTotalResourceValueAfter(Acre const& a, std::chrono::minutes count);

std::tuple<std::vector<Acre>, int> findCycle(Acre const& a);

#endif
