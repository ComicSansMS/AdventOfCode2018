#ifndef ADVENT_OF_CODE_10_MOVING_STARS_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_10_MOVING_STARS_HPP_INCLUDE_GUARD

#include <functional>
#include <iosfwd>
#include <string_view>
#include <vector>

struct Vec2 {
    int x;
    int y;

    Vec2(int xx, int yy);
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

struct Star {
    Vec2 position;
    Vec2 velocity;
};
inline bool operator==(Star const& lhs, Star const& rhs)
{
    return (lhs.position == rhs.position) && (lhs.velocity == rhs.velocity);
}

std::vector<Star> parseInput(std::string_view input);

void simulateStep(std::vector<Star>& stars);

std::vector<std::vector<Vec2>> cluster(std::vector<Star> const& stars, int limit);

std::ostream& operator<<(std::ostream& os, std::vector<Star> const& stars);

int findAndPrintMessage(std::vector<Star> stars, std::ostream& os);

#endif
