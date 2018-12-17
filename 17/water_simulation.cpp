#include <water_simulation.hpp>

#include <algorithm>
#include <cassert>
#include <iterator>
#include <limits>
#include <numeric>
#include <ostream>
#include <regex>
#include <string>
#include <tuple>
#include <unordered_map>

#include <iostream>
#include <fstream>

std::ofstream logger;

Simulation parseInput(std::string_view input)
{
    std::regex rx_line(R"((\w)=(\d+), (\w)=(\d+)\.\.(\d+))");

    logger.open("out.log");

    using regex_it = std::regex_iterator<std::string_view::iterator>;
    auto const it_begin = regex_it(begin(input), end(input), rx_line);
    auto const it_end = regex_it();

    Simulation ret {
        Limit{ Vec2(500, 0),
               Vec2(500, 0) },
        {}, Vec2(500, 0) };
    auto adjustLimits = [&ret](int min_x, int max_x, int min_y, int max_y) {
        ret.m_limits.min.x = std::min(ret.m_limits.min.x, min_x);
        ret.m_limits.min.y = std::min(ret.m_limits.min.y, min_y);
        ret.m_limits.max.x = std::max(ret.m_limits.max.x, max_x);
        ret.m_limits.max.y = std::max(ret.m_limits.max.y, max_y);
    };
    auto addVerticalLine = [&ret, adjustLimits ](int x, int start_y, int stop_y)
        {
            for(int y = start_y; y <= stop_y; ++y) {
                ret.m_field[Vec2(x, y)] = Field::Clay;
            }
            adjustLimits(x, x, start_y, stop_y);
        };
    auto addHorizontalLine = [&ret, adjustLimits ](int y, int start_x, int stop_x)
        {
            for(int x = start_x; x <= stop_x; ++x) {
                ret.m_field[Vec2(x, y)] = Field::Clay;
            }
            adjustLimits(start_x, stop_x, y, y);
        };

    for(auto it = it_begin; it != it_end; ++it) {
        auto match = *it;
        if(match[1] == "x") {
            assert(match[3] == "y");
            addVerticalLine(std::stoi(match[2]), std::stoi(match[4]), std::stoi(match[5]));
        } else {
            assert(match[1] == "y");
            assert(match[3] == "x");
            addHorizontalLine(std::stoi(match[2]), std::stoi(match[4]), std::stoi(match[5]));
        }
    }
    ret.m_limits.min.x -= 2;
    ret.m_limits.max.x += 2;

    assert(ret.m_limits.min.y == 0);
    ret.m_vfield.resize((ret.m_limits.max.x - ret.m_limits.min.x + 1) * (ret.m_limits.max.y + 1), Field::Empty);
    for(auto const [pos, field] : ret.m_field) {
        ret.getField(pos) = field;
    }

    return ret;
}

std::ostream& operator<<(std::ostream& os, Simulation const& sim)
{
    for(int y = sim.m_limits.min.y; y <= sim.m_limits.max.y; ++y) {
        for(int x = sim.m_limits.min.x; x <= sim.m_limits.max.x; ++x) {
            auto const f = sim.getField(Vec2(x, y));
            if((x == sim.m_source.x) && (y == sim.m_source.y)) {
                os << '+';
            } else if(f == Field::Empty) {
                os << '.';
            } else if(f == Field::Clay) {
                os << '#';
            } else if(f == Field::Water_Flowing) {
                os << '|';
            } else {
                assert(f == Field::Water_Still);
                os << '~';
            }
        }
        os << '\n';
    }
    return os;
}

Field& Simulation::getField(Vec2 const& pos)
{
    int const stride = m_limits.max.x - m_limits.min.x;
    return m_vfield[pos.y * stride + (pos.x - m_limits.min.x)];
}

Field Simulation::getField(Vec2 const& pos) const
{
    int const stride = m_limits.max.x - m_limits.min.x;
    return m_vfield[pos.y * stride + (pos.x - m_limits.min.x)];
}

bool fieldIsFree(Field f)
{
    return (f == Field::Empty) || (f == Field::Water_Flowing);
}

std::optional<Vec2> Simulation::flowDown(Vec2 const& start_pos)
{
    Vec2 pos = start_pos;
    while(pos.y <= m_limits.max.y) {
        assert((pos.x >= m_limits.min.x) && (pos.x <= m_limits.max.x) && (pos.y >= m_limits.min.y));
        if(!fieldIsFree(getField(pos))) { return std::nullopt; }
        auto const field_below = getField(Vec2(pos.x, pos.y + 1));
        if(fieldIsFree(field_below)) {
            // below is free, flow down
            getField(pos) = Field::Water_Flowing;
            ++pos.y;
            continue;
        }
        assert(!fieldIsFree(field_below));
        return pos;
    }
    // fell out of the map
    return std::nullopt;
}

std::tuple<Status, Vec2> Simulation::flowLeft(Vec2 const& start_pos)
{
    Vec2 pos = start_pos;
    while(pos.x >= m_limits.min.x) {
        assert((pos.x <= m_limits.max.x) && (pos.y >= m_limits.min.y) && (pos.y <= m_limits.max.y));
        assert(fieldIsFree(getField(pos)));
        getField(pos) = Field::Water_Flowing;
        auto const field_below = getField(Vec2(pos.x, pos.y + 1));
        if(fieldIsFree(field_below)) { return std::make_tuple(Status::FloorMissing, pos); }
        auto const field_left = getField(Vec2(pos.x - 1, pos.y));
        if(fieldIsFree(field_left)) {
            // left is free, keep flowing
            --pos.x;
            continue;
        }
        return std::make_tuple(Status::FoundPosition, pos);
    }
    return std::make_tuple(Status::NoPosition, pos);
}

std::tuple<Status, Vec2> Simulation::flowRight(Vec2 const& start_pos)
{
    Vec2 pos = start_pos;
    while(pos.x <= m_limits.max.x) {
        assert((pos.x >= m_limits.min.x) && (pos.y >= m_limits.min.y) && (pos.y <= m_limits.max.y));
        assert(fieldIsFree(getField(pos)));
        getField(pos) = Field::Water_Flowing;
        auto const field_below = getField(Vec2(pos.x, pos.y + 1));
        if(fieldIsFree(field_below)) { return std::make_tuple(Status::FloorMissing, pos); }
        auto const field_right = getField(Vec2(pos.x + 1, pos.y));
        if(fieldIsFree(field_right)) {
            // right is free, keep flowing
            ++pos.x;
            continue;
        }
        return std::make_tuple(Status::FoundPosition, pos);
    }
    return std::make_tuple(Status::NoPosition, pos);
}

bool Simulation::spawn(Vec2 const& spawn_source)
{
    for(;;) {
        auto const flow_end = flowDown(spawn_source);
        if(!flow_end) { /* fell out of the map */ return false; }
        auto const [left_status, left_position] = flowLeft(*flow_end);
        auto const [right_status, right_position] = flowRight(*flow_end);
        assert((left_status != Status::NoPosition) && (right_status != Status::NoPosition));
        if((left_status == Status::FoundPosition) && (right_status == Status::FoundPosition)) {
            if(left_position == *flow_end) {
                getField(right_position) = Field::Water_Still;
            } else {
                getField(left_position) = Field::Water_Still;
            }
            return true;
        }
        if(left_status == Status::FloorMissing) {
            if(spawn(left_position)) { return true; }
        }
        if(right_status == Status::FloorMissing) {
            if(spawn(right_position)) { return true; }
        }
        return false;
    }
}

bool spawnWater(Simulation& sim)
{
    //std::cout << ".";
    //logger << sim << "\n\n\n";
    return sim.spawn(sim.m_source);
}

int64_t simulateFlow(Simulation& sim)
{
    for(;;) {
        bool did_fill_something = false;
        std::vector<Vec2> spawn_point;
        spawn_point.push_back(sim.m_source);
        while(!spawn_point.empty()) {
            bool did_fill = false;
            auto const spawn_source = spawn_point.back();
            auto const flow_end = sim.flowDown(spawn_source);
            if(!flow_end) { /* fell out of the map */ spawn_point.pop_back(); continue; }
            auto const [left_status, left_position] = sim.flowLeft(*flow_end);
            auto const [right_status, right_position] = sim.flowRight(*flow_end);
            assert((left_status != Status::NoPosition) && (right_status != Status::NoPosition));
            if((left_status == Status::FoundPosition) && (right_status == Status::FoundPosition)) {
                did_fill_something = did_fill = true;
                for(int i=left_position.x; i<=right_position.x; ++i) {
                    sim.getField(Vec2(i, left_position.y)) = Field::Water_Still;
                }
                continue;
            }
            if(!did_fill) { spawn_point.pop_back(); }
            if(left_status == Status::FloorMissing) {
                spawn_point.push_back(left_position);
            }
            if(right_status == Status::FloorMissing) {
                spawn_point.push_back(right_position);
            }
        }
        if(!did_fill_something) { break; }
        logger << sim << "\n\n\n\n\n";
    }

    logger << sim;

    auto const it = std::find(begin(sim.m_vfield), end(sim.m_vfield), Field::Clay);
    int const dist = static_cast<int>(std::distance(begin(sim.m_vfield), it));
    int const stride = sim.m_limits.max.x - sim.m_limits.min.x;
    int start_row = dist / stride;

    return std::count_if(begin(sim.m_vfield) + (start_row * stride), end(sim.m_vfield), [](auto const f) {
            return (f == Field::Water_Flowing) || (f == Field::Water_Still);
        });
}
