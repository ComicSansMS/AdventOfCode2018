#include <calibration.hpp>

#include <range/v3/core.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/cycle.hpp>
#include <range/v3/view/transform.hpp>

#include <cassert>
#include <iterator>
#include <regex>
#include <unordered_set>

std::vector<int> parseInput(std::string_view input)
{
    std::regex rx_line(R"(([+-])(\d+))");

    using regex_it = std::regex_iterator<std::string_view::iterator>;
    auto const it_begin = regex_it(begin(input), end(input), rx_line);
    auto const it_end = regex_it();

    std::vector<int> ret = ranges::make_iterator_range(it_begin, it_end) |
        ranges::view::transform([](std::match_results<std::string_view::iterator> const& match) -> int {
            return std::stoi(match[0]);
        });
    return ret;
}

int calculateResultFrequency(std::vector<int> const& frequency_changes)
{
    return ranges::accumulate(frequency_changes, 0);
}

int findRecurringFrequency(std::vector<int> const& frequency_changes)
{
    std::unordered_set<int> known_frequencies;
    auto rng = frequency_changes | ranges::view::cycle;
    auto it = begin(rng);

    // keep accumulating until a frequency is encountered twice
    int current_frequency = 0;
    while(known_frequencies.find(current_frequency) == end(known_frequencies)) {
        known_frequencies.insert(current_frequency);
        current_frequency += *it;
        ++it;
    }
    return current_frequency;
}
