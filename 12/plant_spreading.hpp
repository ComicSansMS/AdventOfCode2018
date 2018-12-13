#ifndef ADVENT_OF_CODE_12_PLANT_SPREADING_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_12_PLANT_SPREADING_HPP_INCLUDE_GUARD

#include <array>
#include <string_view>
#include <unordered_set>

struct Rule {
    std::array<bool, 5> before;
    bool after;
};

class Plants {
private:
    std::unordered_set<int64_t> m_pots;
    std::vector<Rule> m_rules;
    Plants() = default;
public:
    static Plants parseFromInput(std::string_view input);

    bool potHasPlant(int64_t idx) const;

    int64_t countPlants() const;

    std::vector<Rule> getRules() const;

    void simulateStep();

    int64_t accumulate() const;
};

#endif
