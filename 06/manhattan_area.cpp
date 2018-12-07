#include <manhattan_area.hpp>

#include <range/v3/core.hpp>
#include <range/v3/view/iota.hpp>

#include <algorithm>
#include <cassert>
#include <iterator>
#include <numeric>
#include <ostream>
#include <regex>
#include <string>
#include <unordered_set>

bool operator==(Coordinate const& lhs, Coordinate const& rhs)
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

Rectangle::Rectangle(int l, int t, int w, int h)
    :left(l), top(t), width(w), height(h)
{}

bool operator==(Rectangle const& lhs, Rectangle const& rhs)
{
    return (lhs.left == rhs.left) &&
           (lhs.top == rhs.top) &&
           (lhs.width == rhs.width) &&
           (lhs.height == rhs.height);
}

std::ostream& operator<<(std::ostream& os, Rectangle const& r)
{
    return os << "{ l: " << r.left << " t: " << r.top << " w: " << r.width << " h: " << r.height << " }";
}

std::vector<Coordinate> parseInput(std::string_view input)
{
    std::regex rx_line(R"((\d+), (\d+))");

    using regex_it = std::regex_iterator<std::string_view::iterator>;
    auto const it_begin = regex_it(begin(input), end(input), rx_line);
    auto const it_end = regex_it();

    std::vector<Coordinate> ret;
    std::transform(it_begin, it_end, std::back_inserter(ret),
                   [](std::match_results<std::string_view::iterator> const& match) -> Coordinate {
                       return Coordinate{std::stoi(match[1]), std::stoi(match[2])};
                   });
    return ret;
}

Rectangle getFieldDimensions(std::vector<Coordinate> const& coordinates)
{
    assert(!coordinates.empty());
    auto const minmax_x = std::minmax_element(begin(coordinates), end(coordinates),
                                              [](Coordinate const& c1, Coordinate const& c2) { return c1.x < c2.x; });
    auto const minmax_y = std::minmax_element(begin(coordinates), end(coordinates),
                                              [](Coordinate const& c1, Coordinate const& c2) { return c1.y < c2.y; });
    return Rectangle( (minmax_x.first->x - 1),
                      (minmax_y.first->y - 1),
                      ((minmax_x.second->x - minmax_x.first->x) + 3),
                      ((minmax_y.second->y - minmax_y.first->y) + 3) );
}

int getCellIndex(Rectangle const& dimensions, int x, int y)
{
    int const rel_x = x - dimensions.left;
    int const rel_y = y - dimensions.top;
    int const rel_width = dimensions.width;
    return (rel_y * rel_width) + rel_x;
}

int manhattanDistance(Coordinate c1, Coordinate c2)
{
    return std::abs(c1.x - c2.x) + std::abs(c1.y - c2.y);
}

std::vector<FieldCell> fillField(std::vector<Coordinate> points)
{
    auto const dimensions = getFieldDimensions(points);
    std::vector<FieldCell> ret((dimensions.width + 1) * (dimensions.height + 1));
    int const stride = dimensions.width + 1;

    auto rx = ranges::view::iota(dimensions.left, dimensions.left + dimensions.width + 1);
    auto ry = ranges::view::iota(dimensions.top, dimensions.top + dimensions.height + 1);

    int iy = 0;
    for(auto y : ry) {
        int ix = 0;
        for(auto x : rx) {
            int min_distance = std::numeric_limits<int>::max();
            int min_point = -1;
            int ip = 0;
            for(auto const& p : points) {
                int const distance = manhattanDistance(p, Coordinate{x, y});
                if(distance < min_distance) {
                    min_distance = distance;
                    min_point = ip;
                } else if(distance == min_distance) {
                    min_point = -1;
                }
                ++ip;
            }
            ret[iy*stride + ix].closestPoint = min_point;
            ret[iy*stride + ix].distance = min_distance;
            ++ix;
        }
        ++iy;
    }

    return ret;
}

int findLargestArea(std::vector<FieldCell> const& field, std::vector<Coordinate> points)
{
    auto const dimensions = getFieldDimensions(points);
    // blacklist all letters on the edges
    std::unordered_set<int> blacklist;
    blacklist.insert(-1);
    // top
    for(auto i : ranges::view::iota(0, dimensions.width + 1)) {
        blacklist.insert(field[i].closestPoint);
    }
    // bottom
    for(auto i : ranges::view::iota((dimensions.width * (dimensions.height + 1)),
                                    ((dimensions.width + 1) * dimensions.height + 1))) {
        blacklist.insert(field[i].closestPoint);
    }
    // left & right
    for(int iy = 0; iy < (dimensions.height + 1); ++iy)
    {
        blacklist.insert(field[iy * (dimensions.width + 1)].closestPoint);
        blacklist.insert(field[(iy + 1) * (dimensions.width + 1) - 1].closestPoint);
    }

    int max_count = 0;
    for(auto i : ranges::view::iota(0, static_cast<int>(points.size()))) {
        if(blacklist.find(i) != end(blacklist)) { continue; }
        int count = static_cast<int>(std::count_if(begin(field), end(field),
                                     [i](auto const& c) { return c.closestPoint == i; }));
        if(count > max_count) {
            max_count = count;
        }
    }

    return max_count;
}

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

int findLargestAreaFloodFill(std::vector<Coordinate> const& points, int limit)
{
    int max_area = 0;
    for(auto const& p : points) {
        max_area = std::max(max_area, findLargestAreaFloodFill_SinglePoint(points, limit, p));
    }
    return max_area;
}

/*
void floodFill(GridData& g, std::size_t start_row, std::size_t start_col)
{
    std::vector<Coordinates> stack;
    stack.push_back(Coordinates(static_cast<int>(start_row), static_cast<int>(start_col)));
    while(!stack.empty()) {
        auto node = stack.back();
        stack.pop_back();
        if(g[node.r][node.c]) {
            g[node.r][node.c].flip();
        }
        // left neighbor
        if((node.r > 0) && (g[node.r - 1][node.c])) {
            stack.emplace_back(node.r - 1, node.c);
        }
        // right neighbor
        if((node.r < 127) && (g[node.r + 1][node.c])) {
            stack.emplace_back(node.r + 1, node.c);
        }
        // top neighbor
        if((node.c > 0) && (g[node.r][node.c - 1])) {
            stack.emplace_back(node.r, node.c - 1);
        }
        // bottom neighbor
        if((node.c < 127) && (g[node.r][node.c + 1])) {
            stack.emplace_back(node.r, node.c + 1);
        }
    }
}
*/
