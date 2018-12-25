#include <adventures_in_spacetime.hpp>

#include <algorithm>
#include <cassert>
#include <iterator>
#include <limits>
#include <numeric>
#include <regex>

Vec4::Vec4(int xx, int yy, int zz, int tt)
    :x(xx), y(yy), z(zz), t(tt)
{}

bool operator==(Vec4 const& lhs, Vec4 const& rhs)
{
    return (lhs.x == rhs.x) &&
           (lhs.y == rhs.y) &&
           (lhs.z == rhs.z) &&
           (lhs.t == rhs.t);
}

std::vector<Vec4> parseInput(std::string_view input)
{
    std::regex rx_line(R"((-?\d+),(-?\d+),(-?\d+),(-?\d+))");

    using regex_it = std::regex_iterator<std::string_view::iterator>;
    auto const it_begin = regex_it(begin(input), end(input), rx_line);
    auto const it_end = regex_it();

    auto const match1 = *it_begin;
    std::vector<Vec4> ret;
    std::transform(it_begin, it_end, std::back_inserter(ret), [](auto const& matches) -> Vec4 {
        return Vec4(std::stoi(matches[1]), std::stoi(matches[2]) , std::stoi(matches[3]), std::stoi(matches[4]));
    });
    return ret;
}

int distance(Vec4 const& lhs, Vec4 const& rhs)
{
    return std::abs(lhs.x - rhs.x) +
           std::abs(lhs.y - rhs.y) +
           std::abs(lhs.z - rhs.z) +
           std::abs(lhs.t - rhs.t);
}

std::vector<std::vector<Vec4>> cluster(std::vector<Vec4> const& vs)
{
    std::vector<std::vector<Vec4>> ret;
    std::vector<Vec4> remains = vs;
    while(!remains.empty()) {
        std::vector<Vec4> cluster;
        cluster.push_back(remains.back());
        remains.pop_back();

        for(auto it_candidate = begin(remains); it_candidate != end(remains);) {
            auto const& candidate = *it_candidate;
            auto const it = std::find_if(begin(cluster), end(cluster),
                [&candidate](Vec4 const& c) { return distance(candidate, c) <= 3; });
            if(it != end(cluster)) {
                cluster.push_back(candidate);
                remains.erase(it_candidate);
                it_candidate = begin(remains);
            } else {
                ++it_candidate;
            }
        }
        ret.push_back(std::move(cluster));
    }
    return ret;
}
