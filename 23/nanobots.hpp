#ifndef ADVENT_OF_CODE_23_NANOBOTS_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_23_NANOBOTS_HPP_INCLUDE_GUARD

#include <iosfwd>
#include <optional>
#include <string_view>
#include <tuple>
#include <vector>

struct Vec3 {
    int x;
    int y;
    int z;

    Vec3() = default;
    Vec3(int xx, int yy, int zz);
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
    int radius;
};

struct Bounding {
    Vec3 min;
    Vec3 max;
};

std::vector<Nanobot> parseInput(std::string_view input);

int inRangeOfLargestSignal(std::vector<Nanobot> const& bots);

Bounding getBounding(std::vector<Nanobot> const& bots);

struct Overlaps {
    int width;
    int height;
    int depth;
    std::vector<int> overlaps;
    Vec3 max_coords;
    int max_overlap;

    int& getOverlap(int x, int y, int z);
    int getOverlap(int x, int y, int z) const;
};

Overlaps overlaps(Bounding const& brect, std::vector<Nanobot> const& bots);

bool intersects(Nanobot const& bot, Bounding const& brect);

/*
bool doesCubeIntersectSphere(vec3 C1, vec3 C2, vec3 S, float R)
{
float dist_squared = R * R;
// assume C1 and C2 are element-wise sorted, if not, do that now
if (S.X < C1.X) dist_squared -= squared(S.X - C1.X);
else if (S.X > C2.X) dist_squared -= squared(S.X - C2.X);
if (S.Y < C1.Y) dist_squared -= squared(S.Y - C1.Y);
else if (S.Y > C2.Y) dist_squared -= squared(S.Y - C2.Y);
if (S.Z < C1.Z) dist_squared -= squared(S.Z - C1.Z);
else if (S.Z > C2.Z) dist_squared -= squared(S.Z - C2.Z);
return dist_squared > 0;
}
*/

#endif
