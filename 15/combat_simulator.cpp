#include <combat_simulator.hpp>

#include <algorithm>
#include <cassert>
#include <deque>
#include <iterator>
#include <limits>
#include <numeric>
#include <sstream>
#include <string>
#include <tuple>

bool operator==(Position const& lhs, Position const& rhs)
{
    return std::tie(lhs.x, lhs.y) == std::tie(rhs.x, rhs.y);
}

bool operator<(Position const& lhs, Position const& rhs)
{
    return std::tie(lhs.y, lhs.x) < std::tie(rhs.y, rhs.x);
}

Faction operator!(Faction f)
{
    return (f == Faction::Elf) ? Faction::Goblin : Faction::Elf;
}

bool operator<(Unit const& lhs, Unit const& rhs)
{
    return lhs.position < rhs.position;
}

Battlefield parseInput(std::string_view input)
{
    Battlefield ret;

    std::stringstream sstr{std::string(input)};

    std::string line;
    bool first_iter = true;
    int unit_id_count = 0;
    ret.nElves = 0;
    ret.nGoblins = 0;
    while(std::getline(sstr, line)) {
        if(first_iter) {
            assert(line.size() < std::numeric_limits<int>::max());
            ret.fieldWidth = static_cast<int>(line.size());
            ret.fieldHeight = 0;
            first_iter = false;
        }
        assert(line.size() == static_cast<std::size_t>(ret.fieldWidth));
        for(int i = 0; i < ret.fieldWidth; ++i) {
            char const c = line[i];
            if((c == '.') || (c == '#')) {
                ret.field.push_back(c);
            } else if(c == 'G') {
                ret.field.push_back('.');
                ret.units.push_back(Unit{ ++unit_id_count, Faction::Goblin, Position{i, ret.fieldHeight}, 200 });
                ++ret.nGoblins;
            } else {
                assert(c == 'E');
                ret.field.push_back('.');
                ret.units.push_back(Unit{ ++unit_id_count, Faction::Elf, Position{i, ret.fieldHeight}, 200 });
                ++ret.nElves;
            }
        }
        ++ret.fieldHeight;
    }
    return ret;
}

char Battlefield::getField(int x, int y) const
{
    assert((x >= 0) && (x < fieldWidth));
    assert((y >= 0) && (y < fieldHeight));
    return field[y * fieldWidth + x];
}

std::optional<std::size_t> Battlefield::hasUnit(int x, int y) const
{
    assert((x >= 0) && (x < fieldWidth));
    assert((y >= 0) && (y < fieldHeight));
    auto const it_begin = begin(units);
    auto const it_end = end(units);
    auto it = std::find_if(it_begin, it_end,
                           [pos = Position{x, y}](Unit const& u) { return u.position == pos; });
    return (it != it_end) ? std::make_optional(std::distance(it_begin, it)) : std::nullopt;
}

std::ostream& operator<<(std::ostream& os, Battlefield const& b)
{
    for(int y = 0; y < b.fieldHeight; ++y) {
        for(int x = 0; x < b.fieldWidth; ++x) {
            if(b.getField(x, y) == '#') {
                os << '#';
            } else {
                auto unit_idx = b.hasUnit(x, y);
                os << (unit_idx ? ((b.units[*unit_idx].faction == Faction::Elf) ? 'E' : 'G' ) : '.');
            }
        }
        os << '\n';
    }
    return os;
}

bool Battlefield::isInRangeFor(Faction f, int x, int y) const
{
    assert(getField(x, y) != '#');
    Faction const enemy_faction = !f;
    // check left
    if(x > 0) {
        auto const u = hasUnit(x - 1, y);
        if(u && (units[*u].faction == enemy_faction)) { return true; }
    }
    // check right
    if(x < fieldWidth - 1) {
        auto const u = hasUnit(x + 1, y);
        if(u && (units[*u].faction == enemy_faction)) { return true; }
    }
    // check top
    if(y > 0) {
        auto const u = hasUnit(x, y - 1);
        if(u && (units[*u].faction == enemy_faction)) { return true; }
    }
    // check bottom
    if(y < fieldHeight - 1) {
        auto const u = hasUnit(x, y + 1);
        if(u && (units[*u].faction == enemy_faction)) { return true; }
    }
    return false;
}

std::vector<Position> Battlefield::shortestPaths(Unit const& u) const
{
    if(isInRangeFor(u.faction, u.position.x, u.position.y)) {
        // no need to move; already in range
        return {};
    }

    shortestPathHelper.assign(field.size(), PathHelper{-1, Position{-1, -1}});
    auto getHelper = [this](Position const& pos) -> PathHelper& {
        return shortestPathHelper[pos.y * fieldWidth + pos.x];
    };

    std::deque<Position> stack;
    stack.push_back(u.position);
    getHelper(u.position) = PathHelper{0, Position{-1, -1}};

    int distance = 0;
    std::vector<Position> candidates;

    auto push_neighbor =
        [this, distance, getHelper, &stack, &candidates, &u](int pred_dist, Position const& pred_pos, int x, int y) {
            if(getField(x, y) == '#') { return; }
            if(hasUnit(x, y)) { return; }
            Position const pos{x, y};
            auto& helper = getHelper(pos);
            if(helper.shortestDistance != -1) { return; }
            if(isInRangeFor(u.faction, x, y)) {
                candidates.push_back(pos);
            }
            stack.push_back(pos);
            helper = PathHelper{ pred_dist + 1, pred_pos };
        };

    while(!stack.empty()) {
        Position next = stack.front();
        stack.pop_front();
        PathHelper& helper = getHelper(next);
        if(helper.shortestDistance != distance) {
            // we only consider stopping after evaluating all nodes with the current distance
            assert(helper.shortestDistance == distance + 1);
            ++distance;
            if(!candidates.empty()) { break; }
        }
        assert(helper.shortestDistance == distance);
        // prefer neighbors with lower reading order
        push_neighbor(distance, next, next.x, next.y - 1);
        push_neighbor(distance, next, next.x - 1, next.y);
        push_neighbor(distance, next, next.x + 1, next.y);
        push_neighbor(distance, next, next.x, next.y + 1);
    }

    if(candidates.empty()) {
        // no room to move
        return {};
    }

    // choose first candidate in reading order
    std::sort(begin(candidates), end(candidates));
    Position const target = candidates.front();

    // determine shortest path by traversing predecessors in PathHelpers
    PathHelper const* helper = &getHelper(target);
    std::vector<Position> path;
    path.reserve(helper->shortestDistance);
    path.push_back(target);
    while(helper->shortestDistance > 1) {
        path.push_back(helper->previousField);
        helper = &getHelper(helper->previousField);
    }
    assert(helper->shortestDistance == 1);

    return path;
}

bool Battlefield::attack(Unit const& u)
{
    std::vector<std::size_t > candidates;
    auto const add_candidate = [this, &candidates, enemy_faction = (!u.faction)](int xx, int yy) {
        auto const has_unit = hasUnit(xx, yy);
        if(has_unit && (units[*has_unit].faction == enemy_faction)) {
            candidates.push_back(*has_unit);
        }
    };
    // evaluate in reading order
    add_candidate(u.position.x, u.position.y - 1);
    add_candidate(u.position.x - 1, u.position.y);
    add_candidate(u.position.x + 1, u.position.y);
    add_candidate(u.position.x, u.position.y + 1);
    if(candidates.empty()) { /* nothing to attack in range */ return false; }
    auto target_it = std::min_element(begin(candidates), end(candidates),
        [this](std::size_t lhs, std::size_t rhs) { return units[lhs].hitpoints < units[rhs].hitpoints; });

    auto& target = units[*target_it];
    target.hitpoints -= 3;
    if(target.hitpoints <= 0) {
        // target died from attack
        graveyard.push_back(target);
        units.erase(begin(units) + *target_it);
        return true;
    }
    return false;
}


bool Battlefield::playTurn()
{
    if((nGoblins == 0) || (nElves == 0)) { return false; }
    // ensure read-order
    std::sort(begin(units), end(units));

    for(auto it = begin(units); it != end(units); ++it) {
        auto& u = *it;
        auto const path = shortestPaths(u);
        if(!path.empty()) {
            assert((abs(path.back().x - u.position.x) + abs(path.back().y - u.position.y)) == 1);
            u.position = path.back();
        }

        int const unit_id = u.id;
        Faction const unit_faction = u.faction;
        bool const did_kill = attack(u);
        if(did_kill) {
            // killing invalidates iterators; find our unit again
            it = std::find_if(begin(units), end(units), [unit_id](Unit const& ut) { return ut.id == unit_id; });
            if(unit_faction == Faction::Elf) {
                --nGoblins;
            } else {
                assert(unit_faction == Faction::Goblin);
                --nElves;
            }
            if((nGoblins == 0) || (nElves == 0)) {
                return (it + 1 == end(units));
            }
        }
    }
    return true;
}

BattleStats Battlefield::simulateBattle()
{
    BattleStats stats{0, 0, 0};
    while(playTurn()) {
        ++stats.turns_played;
    }
    stats.hitpoints_remaining = std::accumulate(begin(units), end(units), 0,
                                                [](int acc, Unit const& u) { return acc + u.hitpoints; });
    stats.outcome = stats.hitpoints_remaining * stats.turns_played;
    return stats;
}
