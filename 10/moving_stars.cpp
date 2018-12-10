#include <moving_stars.hpp>

#include <range/v3/core.hpp>
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/algorithm/minmax_element.hpp>
#include <range/v3/algorithm/transform.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/utility/iterator.hpp>

#include <algorithm>
#include <cassert>
#include <iterator>
#include <limits>
#include <numeric>
#include <ostream>
#include <regex>
#include <string>
#include <unordered_set>

Vec2::Vec2(int xx, int yy)
    :x(xx), y(yy)
{}

std::vector<Star> parseInput(std::string_view input)
{
    std::regex rx_line(R"(position=<\s*(-?[\d]+),\s*(-?[\d]+)> velocity=<\s*(-?[\d]+),\s*(-?[\d]+)>)");

    using regex_it = std::regex_iterator<std::string_view::iterator>;
    auto const it_begin = regex_it(begin(input), end(input), rx_line);
    auto const it_end = regex_it();

    std::vector<Star> ret = ranges::make_iterator_range(it_begin, it_end) |
        ranges::view::transform([](std::match_results<std::string_view::iterator> const& match) -> Star {
                                    return Star{ Vec2(std::stoi(match[1]), std::stoi(match[2])),
                                                 Vec2(std::stoi(match[3]), std::stoi(match[4])) };
                                });
    return ret;
}

void simulateStep(std::vector<Star>& stars)
{
    ranges::for_each(stars, [](auto& s) {
        s.position.x += s.velocity.x;
        s.position.y += s.velocity.y;
    });
}

namespace {
std::vector<Vec2> extract_cluster(std::unordered_set<Vec2>& s)
{
    assert(!s.empty());
    std::vector<Vec2> stack;
    auto extract = [&](Vec2 const& v) {
        if(s.find(v) != end(s)) {
            stack.push_back(v);
            s.erase(v);
        }
    };
    extract(*begin(s));

    std::vector<Vec2> ret;
    while(!stack.empty()) {
        auto v = stack.back();
        stack.pop_back();
        ret.push_back(v);

        Vec2 top(v.x, v.y - 1);
        extract(top);
        Vec2 bottom(v.x, v.y + 1);
        extract(bottom);
        Vec2 left(v.x - 1, v.y);
        extract(left);
        Vec2 right(v.x + 1, v.y);
        extract(right);
    }
    return ret;
}

std::unordered_set<Vec2> stars_to_position_set(std::vector<Star> const& stars)
{
    std::unordered_set<Vec2> ret;
    ranges::transform(stars, ranges::inserter(ret, ranges::end(ret)),
                      [](Star const& s) { return s.position; });
    return ret;
}
}

std::vector<std::vector<Vec2>> cluster(std::vector<Star> const& stars, int limit)
{
    std::vector<std::vector<Vec2>> clusters;
    std::unordered_set<Vec2> all_stars = stars_to_position_set(stars);
    while(!all_stars.empty() && (static_cast<int>(clusters.size()) < limit)) {
        clusters.push_back( extract_cluster(all_stars) );
    }
    return clusters;
}

std::ostream& operator<<(std::ostream& os, std::vector<Star> const& stars)
{
    auto const minmax_x = ranges::minmax_element(stars,
        [](Star const& l, Star const& r) { return l.position.x < r.position.x; });
    auto const minmax_y = ranges::minmax_element(stars,
        [](Star const& l, Star const& r) { return l.position.y < r.position.y; });
    Vec2 const min(minmax_x.first->position.x, minmax_y.first->position.y);
    Vec2 const max(minmax_x.second->position.x, minmax_y.second->position.y);
    std::unordered_set<Vec2> all_stars = stars_to_position_set(stars);
    for(int y : ranges::view::iota(0, (max.y - min.y) + 1)) {
        for(int x : ranges::view::iota(0, (max.x - min.x) + 1)) {
            os << ((all_stars.find(Vec2(x + min.x, y + min.y)) != end(all_stars)) ? '#' : ' ');
        }
        os << '\n';
    }
    return os;
}

int findAndPrintMessage(std::vector<Star> stars, std::ostream& os)
{
    int const cluster_limit = static_cast<int>(stars.size()) / 5;
    int seconds_elapsed = 0;
    while(static_cast<int>(cluster(stars, cluster_limit).size()) >= cluster_limit) {
        simulateStep(stars);
        ++seconds_elapsed;
    }
    os << stars;
    return seconds_elapsed;
}
