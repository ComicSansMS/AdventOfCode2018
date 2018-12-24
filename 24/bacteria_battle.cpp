#include <bacteria_battle.hpp>

#include <algorithm>
#include <cassert>
#include <iterator>
#include <limits>
#include <optional>
#include <ostream>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>

/*
Immune System:
17 units each with 5390 hit points (weak to radiation, bludgeoning) with
an attack that does 4507 fire damage at initiative 2
989 units each with 1274 hit points (immune to fire; weak to bludgeoning,
slashing) with an attack that does 25 slashing damage at initiative 3

Infection:
801 units each with 4706 hit points (weak to radiation) with an attack
that does 116 bludgeoning damage at initiative 1
4485 units each with 2961 hit points (immune to radiation; weak to fire,
cold) with an attack that does 12 slashing damage at initiative 4
*/

AttackType deserialize(std::string const& str)
{
    if(str == "bludgeoning") {
        return AttackType::Bludgeoning;
    } else if(str == "cold") {
        return AttackType::Cold;
    } else if(str == "fire") {
        return AttackType::Fire;
    } else if(str == "radiation") {
        return AttackType::Radiation;
    } else {
        assert(str == "slashing");
        return AttackType::Slashing;
    }
}

namespace {
std::vector<AttackType> parseGeneric(std::string const& str, std::string_view look_for)
{
    assert((str[0] == ' ') && (str[1] == '(') && (str[str.length() - 1] == ')'));
    auto idx_start = str.find(look_for);
    if(idx_start == std::string::npos) { return {}; }
    idx_start += look_for.length();
    std::vector<AttackType> ret;
    for(;;) {
        auto idx_end = str.find_first_of(",;)", idx_start);
        assert(idx_end != std::string::npos);
        ret.push_back(deserialize(str.substr(idx_start, idx_end - idx_start)));
        if(str[idx_end] == ',') {
            idx_start = idx_end + 2;
        } else {
            break;
        }
    }
    return ret;
}
}

std::vector<AttackType> parseWeaknesses(std::string const& str)
{
    return parseGeneric(str, "weak to ");
}

std::vector<AttackType> parseImmunities(std::string const& str)
{
    return parseGeneric(str, "immune to ");
}

Battlefield parseInput(std::string_view input)
{
    std::regex rx_line(R"((\d+) units each with (\d+) hit points( \(.*\))? with an attack that does (\d+) (\w+) damage at initiative (\d+))");
    std::smatch matches;

    Battlefield ret;
    std::stringstream sstr(std::string{input});
    std::string line;
    std::optional<Faction> current_faction;
    while(std::getline(sstr, line)) {
        if(line == "Immune System:") {
            current_faction = Faction::Immune;
        } else if(line == "Infection:") {
            current_faction = Faction::Infection;
        } else if(line == "") {
            current_faction = std::nullopt;
        } else {
            std::regex_match(line, matches, rx_line);
            UnitStats stats;
            stats.faction = *current_faction;
            stats.hitPoints = std::stoi(matches[2]);
            stats.attackDamage = std::stoi(matches[4]);
            stats.attackType = deserialize(matches[5]);
            stats.initiative = std::stoi(matches[6]);
            if(matches[3].matched) {
                stats.weaknesses = parseWeaknesses(matches[3]);
                stats.immunities = parseImmunities(matches[3]);
            }
            int const n_units = std::stoi(matches[1]);
            Group g{ stats, n_units };
            ret.groups.push_back(g);
        }
    }

    return ret;
}
