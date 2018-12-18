#include <game_of_woods.hpp>

#include <algorithm>
#include <cassert>
#include <iterator>
#include <limits>
#include <ostream>
#include <regex>
#include <unordered_map>

Field& Acre::get(int x, int y)
{
    assert((x >= 0) && (x < m_width) && (y >= 0) && (y <= m_height));
    return m_field[y*m_width + x];
}

Field Acre::get(int x, int y) const
{
    assert((x >= 0) && (x < m_width) && (y >= 0) && (y <= m_height));
    return m_field[y*m_width + x];
}

namespace {
template<typename Func>
void traverse8x8(Acre const& a, int x, int y, Func func)
{
    // top row
    if(y > 0) {
        if(x > 0) {
            func(a.get(x-1, y-1));
        }
        func(a.get(x, y-1));
        if(x < a.m_width - 1) {
            if(func(a.get(x+1, y-1))) { return; }
        }
    }
    // middle row
    if(x > 0) {
        func(a.get(x-1, y));
    }
    if(x < a.m_width - 1) {
        if(func(a.get(x+1, y))) { return; }
    }
    // bottom row
    if(y < a.m_height - 1) {
        if(x > 0) {
            func(a.get(x-1, y+1));
        }
        func(a.get(x, y+1));
        if(x < a.m_width - 1) {
            func(a.get(x+1, y+1));
        }
    }
}
}

bool Acre::openCondition(int x, int y) const
{
    assert(get(x, y) == Field::Open);
    int tree_count = 0;
    traverse8x8(*this, x, y, [&](Field f) {
        if(f == Field::Trees)  { ++tree_count; }
        return (tree_count >= 3);
    });
    return (tree_count >= 3);
}

bool Acre::treeCondition(int x, int y) const
{
    assert(get(x, y) == Field::Trees);
    int yard_count = 0;
    traverse8x8(*this, x, y, [&](Field f) {
        if(f == Field::Yard)  { ++yard_count; }
        return (yard_count >= 3);
    });
    return (yard_count >= 3);
}

bool Acre::yardCondition(int x, int y) const
{
    assert(get(x, y) == Field::Yard);
    bool found_yard = false;
    bool found_tree = false;
    traverse8x8(*this, x, y, [&](Field f) {
        if(f == Field::Trees) { found_tree = true; }
        if(f == Field::Yard)  { found_yard = true; }
        return (found_tree && found_yard);
    });
    return !(found_tree && found_yard);
}

namespace {
Field charToField(char c)
{
    if(c == '.') {
        return Field::Open;
    } else if(c == '|') {
        return Field::Trees;
    } else {
        assert(c == '#');
        return Field::Yard;
    }
}
}

Acre parseInput(std::string_view input)
{
    Acre ret;
    ret.m_width = -1;
    ret.m_height = 0;
    int chars_in_line = 0;
    for(auto const& c : input)
    {
        if(c != '\n') {
            ret.m_field.push_back(charToField(c));
            ++chars_in_line;
        } else {
            if(ret.m_width == -1) {
                ret.m_width = chars_in_line;
            }
            assert(chars_in_line == ret.m_width);
            chars_in_line = 0;
            ++ret.m_height;
        }
    }
    assert(chars_in_line == 0);
    return ret;
}

std::ostream& operator<<(std::ostream& os, Acre const& a)
{
    for(int y = 0; y < a.m_height; ++y) {
        for(int x = 0; x < a.m_width; ++x) {
            Field const f = a.get(x, y);
            if(f == Field::Open) {
                os << '.';
            } else if(f == Field::Yard) {
                os << '#';
            } else {
                assert(f == Field::Trees);
                os << '|';
            }
        }
        os << '\n';
    }
    return os;
}

Acre step(Acre const& a)
{
    Acre ret;
    ret.m_width = a.m_width;
    ret.m_height = a.m_height;
    ret.m_field.reserve(a.m_field.size());
    for(int y = 0; y < a.m_height; ++y) {
        for(int x = 0; x < a.m_width; ++x) {
            Field const f = a.get(x, y);
            Field const n = [f, x, y, &a]() {
                if(f == Field::Open) {
                    return (a.openCondition(x, y)) ? Field::Trees : Field::Open;
                } else if(f == Field::Trees) {
                    return (a.treeCondition(x, y)) ? Field::Yard : Field::Trees;
                } else {
                    assert(f == Field::Yard);
                    return (a.yardCondition(x, y)) ? Field::Open : Field::Yard;
                }
            }();
            ret.m_field.push_back(n);
        }
    }
    return ret;
}

int Acre::totalResourceValue() const
{
    int n_trees = 0;
    int n_yards = 0;
    for(auto const& f : m_field) {
        if(f == Field::Trees) { ++n_trees; }
        if(f == Field::Yard)  { ++n_yards; }
    }
    return n_trees * n_yards;
}

int getTotalResourceValueAfter(Acre const& a, std::chrono::minutes count)
{
    Acre acc = step(a);
    for(int i = 1; i < count.count(); ++i) {
        acc = step(acc);
    }
    return acc.totalResourceValue();
}

std::tuple<std::vector<Acre>, int> findCycle(Acre const& a)
{
    std::vector<Acre> acres;
    acres.push_back(a);
    int cycle_start = -1;
    while(cycle_start == -1) {
        Acre const new_a = step(acres.back());
        for(int j = 0; j < static_cast<int>(acres.size()); ++j) {
            auto const& old_a = acres[j];
            if(new_a.m_field == old_a.m_field) {
                // found cycle
                cycle_start = j;
                break;
            }
        }
        acres.push_back(new_a);
    }
    return std::make_tuple(acres, cycle_start);
}
