#include <calibration.hpp>

#include <algorithm>
#include <iterator>
#include <numeric>
#include <regex>
#include <unordered_set>

std::vector<int> parseInput(std::string_view input)
{
    std::regex rx_line(R"(^([+-])(\d+)$)");

    using regex_it = std::regex_iterator<std::string_view::iterator>;
    auto const it_begin = regex_it(begin(input), end(input), rx_line);
    auto const it_end = regex_it();

    std::vector<int> ret;
    std::transform(it_begin, it_end, std::back_inserter(ret),
                   [](std::match_results<std::string_view::iterator> const& match) -> int { 
                       return std::stoi(match[0]);
                   });
    return ret;
}

int calculateResultFrequency(std::vector<int> const& frequency_changes)
{
    return std::accumulate(begin(frequency_changes), end(frequency_changes), 0);
}

int findRecurringFrequency(std::vector<int> const& frequency_changes)
{
    std::unordered_set<int> known_frequencies;
    auto const it_begin = begin(frequency_changes);
    auto const it_end = end(frequency_changes);
    auto it = it_begin;

    // keep accumulating until a frequency is encountered twice
    int current_frequency = 0;
    while(known_frequencies.find(current_frequency) == end(known_frequencies)) {
        known_frequencies.insert(current_frequency);
        current_frequency += *it;
        ++it;
        // wrap around when reaching end of the buffer
        if(it == it_end) { it = it_begin; }
    }
    return current_frequency;
}
