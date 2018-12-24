#include <nanobots.hpp>

#include <algorithm>
#include <cassert>
#include <iterator>
#include <limits>
#include <ostream>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>

Vec3::Vec3(int xx, int yy, int zz)
    :x(xx), y(yy), z(zz)
{}

bool operator==(Vec3 const& lhs, Vec3 const& rhs)
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z);
}

std::vector<Nanobot> parseInput(std::string_view input)
{
    std::regex rx_line(R"(pos=\<(-?\d+),(-?\d+),(-?\d+)\>, r=(-?\d+))");
    using regex_it = std::regex_iterator<std::string_view::iterator>;
    auto const it_begin = regex_it(begin(input), end(input), rx_line);
    auto const it_end = regex_it();

    std::vector<Nanobot> ret;
    for(auto it = it_begin; it != it_end; ++it) {
        auto const& match = *it;
        Nanobot b;
        b.position.x = std::stoi(match[1]);
        b.position.y = std::stoi(match[2]);
        b.position.z = std::stoi(match[3]);
        b.radius = std::stoi(match[4]);
        ret.push_back(b);
    }

    return ret;
}

namespace {
int dist(Vec3 const& lhs, Vec3 const& rhs) {
    return std::abs(lhs.x - rhs.x) +
           std::abs(lhs.y - rhs.y) +
           std::abs(lhs.z - rhs.z);
}

int dist(Nanobot const& lhs, Nanobot const& rhs) {
    return dist(lhs.position, rhs.position);
}
}

int inRangeOfLargestSignal(std::vector<Nanobot> const& bots)
{
    auto const it_begin = begin(bots);
    auto const it_end = end(bots);
    auto const it_largest_signal =
        std::max_element(it_begin, it_end,
                         [](Nanobot const& lhs, Nanobot const& rhs) { return lhs.radius < rhs.radius; });

    Nanobot const& l = *it_largest_signal;
    return static_cast<int>(
        std::count_if(it_begin, it_end, [&l](Nanobot const& b) { return (dist(l, b) <= l.radius); }));
}

Bounding getBounding(std::vector<Nanobot> const& bots)
{
    Bounding ret;
    ret.min = bots.front().position;
    ret.max = bots.front().position;
    for(auto const& b : bots) {
        ret.min.x = std::min(ret.min.x, b.position.x);
        ret.max.x = std::max(ret.max.x, b.position.x);
        ret.min.y = std::min(ret.min.y, b.position.y);
        ret.max.y = std::max(ret.max.y, b.position.y);
        ret.min.z = std::min(ret.min.z, b.position.z);
        ret.max.z = std::max(ret.max.z, b.position.z);
    }
    return ret;
}

int& Overlaps::getOverlap(int x, int y, int z)
{
    assert((x >= 0) && (x <= width));
    assert((y >= 0) && (y <= height));
    assert((z >= 0) && (z <= depth));
    return overlaps[(z * width * height) + y * width + x];
}

int Overlaps::getOverlap(int x, int y, int z) const
{
    assert((x >= 0) && (x <= width));
    assert((y >= 0) && (y <= height));
    assert((z >= 0) && (z <= depth));
    return overlaps[(z * width * height) + y * width + x];
}

Overlaps overlaps(Bounding const& brect, std::vector<Nanobot> const& bots)
{
    Overlaps ret;
    int const w = (brect.max.x + 1) - brect.min.x;
    int const h = (brect.max.y + 1) - brect.min.y;
    int const d = (brect.max.z + 1) - brect.min.z;
    assert((w > 0) && (h > 0) && (d > 0));
    ret.width = w;
    ret.height = h;
    ret.depth = d;
    ret.overlaps.reserve(w * h * d);
    int max_acc = 0;
    Vec3 max_coords(0, 0, 0);
    for(int iz = 0; iz <= d; ++iz) {
        int const z = iz + brect.min.z;
        for(int iy = 0; iy <= h; ++iy) {
            int const y = iy + brect.min.y;
            for(int ix = 0; ix <= w; ++ix) {
                int const x = ix + brect.min.x;
                Vec3 const v(x, y, z);
                int acc = 0;
                for(auto const& b : bots) {
                    if(dist(b.position, v) <= b.radius) { ++acc; }
                }
                if(acc > max_acc) {
                    max_acc = acc;
                    max_coords = Vec3(x, y, z);
                }
                ret.overlaps.push_back(acc);
            }
        }
    }
    ret.max_coords = max_coords;
    ret.max_overlap = max_acc;
    return ret;
}

bool intersects(Nanobot const& bot, Bounding const& brect)
{
    assert((brect.min.x <= brect.max.x) && (brect.min.y <= brect.max.y) && (brect.min.z <= brect.max.z));
    int dist = static_cast<int64_t>(bot.radius);
    if      (bot.position.x < brect.min.x) { dist -= std::abs(bot.position.x - brect.min.x); }
    else if (bot.position.x > brect.max.x) { dist -= std::abs(bot.position.x - brect.max.x); }
    if      (bot.position.y < brect.min.y) { dist -= std::abs(bot.position.y - brect.min.y); }
    else if (bot.position.y > brect.max.y) { dist -= std::abs(bot.position.y - brect.max.y); }
    if      (bot.position.z < brect.min.z) { dist -= std::abs(bot.position.z - brect.min.z); }
    else if (bot.position.z > brect.max.z) { dist -= std::abs(bot.position.z - brect.max.z); }
    return dist > 0;
    /*
    auto const square = [](double d) -> double { return d*d; };
    double dist_squared = static_cast<double>(bot.radius) * static_cast<double>(bot.radius);
    if      (bot.position.x < brect.min.x) { dist_squared -= square(bot.position.x - brect.min.x); }
    else if (bot.position.x > brect.max.x) { dist_squared -= square(bot.position.x - brect.max.x); }
    if      (bot.position.y < brect.min.y) { dist_squared -= square(bot.position.y - brect.min.y); }
    else if (bot.position.y > brect.max.y) { dist_squared -= square(bot.position.y - brect.max.y); }
    if      (bot.position.z < brect.min.z) { dist_squared -= square(bot.position.z - brect.min.z); }
    else if (bot.position.z > brect.max.z) { dist_squared -= square(bot.position.z - brect.max.z); }
    return dist_squared > 0;
    */
}
