#ifndef ADVENT_OF_CODE_14_HOT_CHOCOLATE_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_14_HOT_CHOCOLATE_HPP_INCLUDE_GUARD

#include <string_view>
#include <tuple>
#include <vector>

std::vector<int> parseInput(std::string_view input);

struct RecipeCombiner {
    std::vector<int> scores;
    std::vector<std::size_t> elves;

    RecipeCombiner();

    void combine();

    std::string cook_until(int target_score);

    int cook_until2(std::vector<int> const& target_sequence);
};

#endif
