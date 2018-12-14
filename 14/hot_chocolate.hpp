#ifndef ADVENT_OF_CODE_14_HOT_CHOCOLATE_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_14_HOT_CHOCOLATE_HPP_INCLUDE_GUARD

#include <string_view>
#include <tuple>
#include <vector>

std::vector<int> parseInput(std::string_view input);

struct RecipeCombiner {
    std::vector<int> scores;
    std::vector<int> elves;

    RecipeCombiner(std::vector<int> initial_scores);

    void combine();

    std::string cook_until(int target_score);
};

#endif
