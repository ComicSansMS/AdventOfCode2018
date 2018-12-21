#ifndef ADVENT_OF_CODE_20_REGEX_LABYRINTH_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_20_REGEX_LABYRINTH_HPP_INCLUDE_GUARD

#include <array>
#include <functional>
#include <iosfwd>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <vector>

struct Vec2 {
    int x;
    int y;

    Vec2() = default;
    Vec2(int xx, int yy)
        :x(xx), y(yy)
    {}
};

bool operator==(Vec2 const& lhs, Vec2 const& rhs);

struct Room {
    enum Direction {
        North = 0,
        South,
        West,
        East
    };
    Vec2 position;
    std::array<bool, 4> adjacencies;

    Room() = default;
    explicit Room(Vec2 const& pos);
};

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

struct Map {
    std::unordered_map<Vec2, Room> rooms;
    Vec2 min;
    Vec2 max;
};

Map constructMap(std::string_view regex);

std::ostream& operator<<(std::ostream& os, Map const& m);

std::tuple<int, int> shortestPaths(Map const& m);

#endif
