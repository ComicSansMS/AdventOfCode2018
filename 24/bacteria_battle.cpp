#include <bacteria_battle.hpp>

#include <algorithm>
#include <cassert>
#include <iterator>
#include <limits>
#include <numeric>
#include <optional>
#include <ostream>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>

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

int Battlefield::effectivePower(int group_idx) const
{
    assert((group_idx >= 0) && (group_idx < static_cast<int>(groups.size())));
    Group const& g = groups[group_idx];
    return g.units * g.stats.attackDamage;
}

int Battlefield::attackDamage(int attacker_idx, int target_idx) const
{
    Group const& attacker = groups[attacker_idx];
    if(attacker.units <= 0) { return 0; }
    Group const& target = groups[target_idx];
    assert(attacker.stats.faction != target.stats.faction);
    AttackType type = attacker.stats.attackType;
    if(std::find(begin(target.stats.immunities), end(target.stats.immunities), type) != end(target.stats.immunities)) {
        // target immune
        return 0;
    }
    int const baseDamage = effectivePower(attacker_idx);
    if(std::find(begin(target.stats.weaknesses), end(target.stats.weaknesses), type) != end(target.stats.weaknesses)) {
        // weakness: double damage
        return 2 * baseDamage;
    }
    return baseDamage;
}

std::tuple<int, std::optional<int>> Battlefield::selectTarget(int attacker_idx) const
{
    Group const& g = groups[attacker_idx];

    int max_damage = 0;
    std::optional<int> target_group;
    for(int j = 0; j < static_cast<int>(groups.size()); ++j) {
        if(std::find(begin(selected_targets), end(selected_targets), j) != end(selected_targets)) { continue; }
        Group const& h = groups[j];
        if(h.stats.faction != g.stats.faction) {
            int const damage = attackDamage(attacker_idx, j);
            if(damage == 0) { continue; }
            if(damage >= max_damage) {
                if((damage > max_damage) || (!target_group)) {
                    max_damage = damage;
                    target_group = j;
                } else {
                    assert(damage == max_damage);
                    assert(target_group);
                    int const ep1 = effectivePower(*target_group);
                    int const ep2 = effectivePower(j);
                    if(ep2 > ep1) {
                        max_damage = damage;
                        target_group = j;
                    } else if(ep1 == ep2) {
                        if(h.stats.initiative > groups[*target_group].stats.initiative) {
                            max_damage = damage;
                            target_group = j;
                        }
                    }
                }
            }
        }
    }
    return std::make_tuple(max_damage, target_group);
}

void Battlefield::targetSelection()
{
    std::vector<int> turn_order(groups.size());
    std::iota(begin(turn_order), end(turn_order), 0);
    std::sort(begin(turn_order), end(turn_order), [this](int i1, int i2) {
        int const ep1 = effectivePower(i1);
        int const ep2 = effectivePower(i2);
        if(ep1 != ep2) {
            return ep2 < ep1;
        }
        return groups[i2].stats.initiative < groups[i1].stats.initiative;
    });

    selected_targets.assign(groups.size(), -1);
    for(auto const& i : turn_order) {
        auto const [max_damage, target_idx] = selectTarget(i);
        selected_targets[i] = target_idx.value_or(-1);
    }
}

bool Battlefield::attackPhase()
{
    std::vector<int> turn_order(groups.size());
    std::iota(begin(turn_order), end(turn_order), 0);
    std::sort(begin(turn_order), end(turn_order), [this](int i1, int i2) {
        return groups[i2].stats.initiative < groups[i1].stats.initiative;
    });

    for(auto const& i : turn_order) {
        if(selected_targets[i] == -1) { continue; }
        Group& target = groups[selected_targets[i]];
        int const damage = attackDamage(i, selected_targets[i]);
        int units_lost = damage / target.stats.hitPoints;
        target.units -= units_lost;
    }

    groups.erase(std::remove_if(begin(groups), end(groups), [](Group const& g) { return g.units <= 0; }), end(groups));
    Faction const f = groups.front().stats.faction;
    return std::all_of(begin(groups), end(groups), [f](Group const& g) { return g.stats.faction == f; });
}

namespace {
bool isTied(std::vector<Group> const& g1, std::vector<Group> g2) {
    if(g1.size() != g2.size()) { return false; }
    for(std::size_t i = 0; i < g1.size(); ++i) {
        if(g1[i].units != g2[i].units) { return false; }
    }
    return true;
}
}

int Battlefield::simulateBattle()
{
    for(;;) {
        targetSelection();
        auto const old_groups = groups;
        if(attackPhase()) { break; }
        if(isTied(old_groups, groups)) { return -1; }
    }
    return std::accumulate(begin(groups), end(groups), 0, [](int acc, Group const& g) { return acc + g.units; });
}

void Battlefield::boostUnits(Faction f, int boost)
{
    for(auto& g :groups) {
        if(g.stats.faction == f) {
            g.stats.attackDamage += boost;
        }
    }
}

int findSmallestBoost(Battlefield const& b)
{
    int boost_lower_bound = 1;
    for(int current_boost = boost_lower_bound; ; current_boost *= 2) {
        Battlefield t = b;
        t.boostUnits(Faction::Immune, current_boost);
        int remains = t.simulateBattle();
        if(remains != -1) {
            if(t.groups.front().stats.faction == Faction::Immune) {
                break;
            }
        }
        boost_lower_bound = current_boost;
    }
    for(int current_boost = boost_lower_bound + 1; ; ++current_boost) {
        Battlefield t = b;
        t.boostUnits(Faction::Immune, current_boost);
        int remains = t.simulateBattle();
        if(remains != -1) {
            if(t.groups.front().stats.faction == Faction::Immune) {
                return remains;
            }
        }
    }
}
