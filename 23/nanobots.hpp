#ifndef ADVENT_OF_CODE_23_NANOBOTS_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_23_NANOBOTS_HPP_INCLUDE_GUARD

#include <iosfwd>
#include <optional>
#include <string_view>
#include <tuple>
#include <vector>

struct Vec3 {
    int64_t x;
    int64_t y;
    int64_t z;

    Vec3() = default;
    Vec3(int64_t xx, int64_t yy, int64_t zz);
};

bool operator==(Vec3 const& lhs, Vec3 const& rhs);

namespace std
{
template<> struct hash<Vec3>
{
    typedef Vec3 argument_type;
    typedef std::size_t result_type;
    result_type operator()(argument_type const& v) const noexcept
    {
        return static_cast<std::size_t>(v.x) ^
               static_cast<std::size_t>(v.y) ^
               static_cast<std::size_t>(v.z);
    }
};
}

struct Nanobot {
    Vec3 position;
    int64_t radius;
};

std::vector<Nanobot> parseInput(std::string_view input);

int64_t inRangeOfLargestSignal(std::vector<Nanobot> const& bots);

#endif
