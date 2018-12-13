#include <plant_spreading.hpp>

#include <algorithm>
#include <cassert>
#include <iterator>
#include <limits>
#include <numeric>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

/* static */
Plants Plants::parseFromInput(std::string_view input)
{
    std::stringstream sstr{std::string(input)};
    std::string line;
    std::getline(sstr, line);

    Plants ret;
    assert(line.substr(0, 15) == "initial state: ");
    for(int i=0; i<line.size() - 15; ++i) {
        char const c = line[i + 15];
        assert((c == '#') || (c == '.'));
        if(c == '#') {
            ret.m_pots.insert(i);
        }
    }

    // skip newline
    std::getline(sstr, line);
    assert(line.empty());

    while(std::getline(sstr, line)) {
        ret.m_rules.push_back(Rule{});
        for(int i=0; i<5; ++i) {
            char const c = line[i];
            assert((c == '#') || (c == '.'));
            ret.m_rules.back().before[i] = (c == '#');
        }
        assert(line.substr(5, 4) == " => ");

        assert((line[9] == '#') || (line[9] == '.'));
        ret.m_rules.back().after = (line[9] == '#');
    }

    return ret;
}

bool Plants::potHasPlant(int64_t idx) const
{
    return m_pots.find(idx) != end(m_pots);
}

int64_t Plants::countPlants() const
{
    return static_cast<int64_t>(m_pots.size());
}

std::vector<Rule> Plants::getRules() const
{
    return m_rules;
}

void Plants::simulateStep()
{
    auto const [smallest_index_it, biggest_index_it] = std::minmax_element(begin(m_pots), end(m_pots));
    int64_t const smallest_index = *smallest_index_it;
    int64_t const biggest_index = *biggest_index_it;
    std::unordered_set<int64_t> new_pots;
    for(int64_t i=smallest_index - 2; i<biggest_index + 3; ++i) {
        std::array<bool, 5> pots;
        pots[0] = potHasPlant(i - 2);
        pots[1] = potHasPlant(i - 1);
        pots[2] = potHasPlant(i);
        pots[3] = potHasPlant(i + 1);
        pots[4] = potHasPlant(i + 2);
        for(auto const& r : m_rules) {
            if(pots == r.before) {
                if(r.after) {
                    new_pots.insert(i);
                }
                break;
            }
        }
    }
    m_pots = new_pots;
}

int64_t Plants::accumulate() const
{
    return std::accumulate(begin(m_pots), end(m_pots), static_cast<int64_t>(0));
}
