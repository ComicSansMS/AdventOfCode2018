#ifndef ADVENT_OF_CODE_01_CALIBRATION_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_01_CALIBRATION_HPP_INCLUDE_GUARD

#include <string_view>
#include <vector>

std::vector<int> parseInput(std::string_view input);

int calculateResultFrequency(std::vector<int> const& frequency_changes);

int findRecurringFrequency(std::vector<int> const& frequency_changes);

#endif
