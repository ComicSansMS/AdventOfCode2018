#ifndef ADVENT_OF_CODE_22_SPELUNKING_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_22_SPELUNKING_HPP_INCLUDE_GUARD

#include <iosfwd>
#include <optional>
#include <string_view>
#include <tuple>
#include <vector>

struct Vec2 {
    int x;
    int y;

    Vec2() = default;
    Vec2(int xx, int yy);
};

bool operator==(Vec2 const& lhs, Vec2 const& rhs);

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

struct Scan {
    int depth;
    Vec2 target_coords;
};

Scan parseInput(std::string_view input);

enum class Region {
    Rocky = 0,
    Wet,
    Narrow
};

enum class Tool {
    Torch,
    ClimbingGear,
    Neither
};

struct Spelunker {
    Vec2 position;
    Tool equipped;
    int due_time;

    Spelunker(int n_due_time);
};

struct Cave {
    std::vector<int64_t> erosion_levels;
    std::vector<Region> regions;
    int64_t width;
    int64_t height;
    Scan scan;

    Cave(Scan const& n_scan, int64_t n_width, int64_t n_height);

    int riskLevelToTarget() const;

    int shortestPaths() const;
};

std::ostream& operator<<(std::ostream& os, Cave const& c);

#endif
