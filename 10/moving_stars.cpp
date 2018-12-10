#include <moving_stars.hpp>

#include <range/v3/core.hpp>

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

    std::vector<Star> ret;
    std::transform(it_begin, it_end, std::back_inserter(ret),
        [](std::match_results<std::string_view::iterator> const& match) -> Star {
            return Star{ Vec2(std::stoi(match[1]), std::stoi(match[2])),
                         Vec2(std::stoi(match[3]), std::stoi(match[4])) };
    });
    return ret;
}

void simulateStep(std::vector<Star>& stars)
{
    for(auto& s : stars) {
        s.position.x += s.velocity.x;
        s.position.y += s.velocity.y;
    }
}
/*
int findLargestAreaFloodFill_SinglePoint(std::vector<Coordinate> const& points, int limit, Coordinate const& start_point)
{
    std::vector<Coordinate> stack;
    stack.push_back(start_point);
    std::unordered_map<int, std::unordered_map<int, int>> g;
    auto total_mandist = [&points](int x, int y) {
        return std::accumulate(begin(points), end(points), 0, [x, y](int acc, Coordinate const& p) {
            return acc + manhattanDistance(p, Coordinate{x, y});
        });
    };
    auto process_point = [&g, &stack, limit, total_mandist](int xx, int yy) {
        if((total_mandist(xx, yy) < limit) && (g[xx][yy] == 0)) {
            stack.push_back(Coordinate{xx, yy});
        }
    };
    int total_count = 0;
    while(!stack.empty()) {
        auto node = stack.back();
        stack.pop_back();

        int const x = node.x;
        int const y = node.y;
        if(g[x][y] != 0) { continue; }
        ++total_count;

        g[x][y] = 1;
        // left neighbor
        process_point(x-1, y);
        // right neighbor
        process_point(x+1, y);
        // top neighbor
        process_point(x, y-1);
        // bottom neighbor
        process_point(x, y+1);
    }
    return total_count;
}
*/

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
}

std::vector<std::vector<Vec2>> cluster(std::vector<Star> const& stars, int limit)
{
    std::vector<std::vector<Vec2>> clusters;
    std::unordered_set<Vec2> all_stars;
    std::transform(begin(stars), end(stars), std::inserter(all_stars, end(all_stars)),
                   [](Star const& s) { return s.position; });
    while(!all_stars.empty() && (clusters.size() < limit)) {
        clusters.push_back( extract_cluster(all_stars) );
    }
    return clusters;
}

std::ostream& operator<<(std::ostream& os, std::vector<Star> const& stars)
{
    Vec2 min(std::min_element(begin(stars), end(stars), 
                              [](Star const& l, Star const& r) { return l.position.x < r.position.x; })->position.x,
             std::min_element(begin(stars), end(stars),
                              [](Star const& l, Star const& r) { return l.position.y < r.position.y; })->position.y);
    Vec2 max(std::max_element(begin(stars), end(stars), 
                              [](Star const& l, Star const& r) { return l.position.x < r.position.x; })->position.x,
             std::max_element(begin(stars), end(stars),
                              [](Star const& l, Star const& r) { return l.position.y < r.position.y; })->position.y);
    std::unordered_set<Vec2> all_stars;
    std::transform(begin(stars), end(stars), std::inserter(all_stars, end(all_stars)),
                   [](Star const& s) { return s.position; });
    for(int y=0; y<=(max.y - min.y); ++y) {
        for(int x=0; x<=(max.x - min.x); ++x) {
            if(all_stars.find(Vec2(x + min.x, y + min.y)) != end(all_stars)) {
                os << '#';
            } else {
                os << ' ';
            }
        }
        os << '\n';
    }
    return os;
}

void findAndPrintMessage(std::vector<Star> stars, std::ostream& os)
{
    int const cluster_limit = stars.size() / 5;
    while(cluster(stars, cluster_limit).size() >= cluster_limit) {
        simulateStep(stars);
    }
    os << stars;
}
