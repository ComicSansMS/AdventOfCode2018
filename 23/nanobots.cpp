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

Vec3::Vec3(int64_t xx, int64_t yy, int64_t zz)
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
int64_t dist(Vec3 const& lhs, Vec3 const& rhs) {
    return std::abs(lhs.x - rhs.x) +
           std::abs(lhs.y - rhs.y) +
           std::abs(lhs.z - rhs.z);
}

int64_t dist(Nanobot const& lhs, Nanobot const& rhs) {
    return dist(lhs.position, rhs.position);
}
}

int64_t inRangeOfLargestSignal(std::vector<Nanobot> const& bots)
{
    auto const it_begin = begin(bots);
    auto const it_end = end(bots);
    auto const it_largest_signal =
        std::max_element(it_begin, it_end,
                         [](Nanobot const& lhs, Nanobot const& rhs) { return lhs.radius < rhs.radius; });

    Nanobot const& l = *it_largest_signal;
    return std::count_if(it_begin, it_end, [&l](Nanobot const& b) { return (dist(l, b) <= l.radius); });
}
