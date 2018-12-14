#include <hot_chocolate.hpp>

#include <algorithm>
#include <cassert>
#include <iterator>
#include <limits>
#include <numeric>
#include <sstream>
#include <string>

std::vector<int> parseInput(std::string_view input)
{
    std::vector<int> ret;
    ret.reserve(input.size());
    std::transform(begin(input), end(input), std::back_inserter(ret), [](char c) {
        assert((c >= '0') && (c <= '9'));
        return c - '0';
    });
    return ret;
}

RecipeCombiner::RecipeCombiner()
    :scores({3, 7}), elves(2, 0)
{
    std::iota(begin(elves), end(elves), 0);
}

void RecipeCombiner::combine()
{
    int const sum = std::accumulate(begin(elves), end(elves), 0, [this](int acc, std::size_t elve) {
        return acc + scores[elve];
    });
    std::vector<int> const digits = [](int i) {
        if(i == 0) { return std::vector<int>{0}; }
        std::vector<int> ret;
        while(i > 0) {
            ret.push_back(i % 10);
            i /= 10;
        }
        return ret;
    }(sum);
    scores.insert(end(scores), digits.rbegin(), digits.rend());

    for(auto& e : elves) {
        std::size_t const steps_forward = scores[e] + 1;
        e = (e + steps_forward) % scores.size();
    }
}

std::string RecipeCombiner::cook_until(int target_score)
{
    while(scores.size() < target_score + 10) {
        combine();
    }
    std::string ret;
    ret.reserve(10);
    std::transform(begin(scores) + target_score, begin(scores) + target_score + 10, std::back_inserter(ret),
                   [](int d) {
                       assert((d >= 0) && (d <= 9));
                       return static_cast<char>(d + '0');
                   });
    return ret;
}

int RecipeCombiner::cook_until2(std::vector<int> const& target_sequence)
{
    for(;;) {
        combine();
        std::vector<int>::iterator it_start = (end(scores) - std::min(2 + target_sequence.size(), scores.size()));
        auto it_found = std::search(it_start, end(scores), begin(target_sequence), end(target_sequence));
        if(it_found != end(scores)) {
            return static_cast<int>(std::distance(begin(scores), it_found));
        }
    }
}
