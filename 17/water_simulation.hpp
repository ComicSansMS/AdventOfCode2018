#ifndef ADVENT_OF_CODE_17_WATER_SIMULATION_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_17_WATER_SIMULATION_HPP_INCLUDE_GUARD

#include <functional>
#include <iosfwd>
#include <optional>
#include <string_view>
#include <tuple>

struct Vec2 {
    int x;
    int y;

    Vec2(int xx, int yy)
        :x(xx), y(yy)
    {}
    Vec2(Vec2 const&) = default;
    Vec2& operator=(Vec2 const&) = default;
};

inline bool operator==(Vec2 const& lhs, Vec2 const& rhs)
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

namespace std
{
template<> struct hash<Vec2>
{
    typedef Vec2 argument_type;
    typedef std::size_t result_type;
    result_type operator()(argument_type const& v) const noexcept
    {
        return (static_cast<std::size_t>(v.x) << static_cast<std::size_t>(16)) | static_cast<std::size_t>(v.y);
    }
};
}

struct Limit {
    Vec2 min;
    Vec2 max;
};

enum class Field {
    Empty = 0,
    Clay,
    Water_Flowing,
    Water_Still
};

enum class Status {
    FoundPosition,
    FloorMissing
};

struct Simulation {
    Limit m_limits;
    Vec2 m_source;
    std::vector<Field> m_field;

    Field& getField(Vec2 const& pos);
    Field getField(Vec2 const& pos) const;
    bool spawn(Vec2 const& spawn_source);
    std::optional<Vec2> flowDown(Vec2 const& start_pos);
    std::tuple<Status, Vec2> flowLeft(Vec2 const& start_pos);
    std::tuple<Status, Vec2> flowRight(Vec2 const& start_pos);
};

Simulation parseInput(std::string_view input);

std::ostream& operator<<(std::ostream& os, Simulation const& sim);

bool spawnWater(Simulation& sim);

std::tuple<int64_t, int64_t> simulateFlow(Simulation& sim);

#endif
