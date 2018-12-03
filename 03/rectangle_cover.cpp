#include <rectangle_cover.hpp>

#include <range/v3/core.hpp>
#include <range/v3/algorithm/count_if.hpp>
#include <range/v3/view/transform.hpp>

#include <array>
#include <cassert>
#include <limits>
#include <regex>
#include <string>

std::vector<Rectangle> parseInput(std::string_view input)
{
    std::regex rx_line(R"(#(\d+) @ (\d+),(\d+): (\d+)x(\d+))");

    using regex_it = std::regex_iterator<std::string_view::iterator>;
    auto const it_begin = regex_it(begin(input), end(input), rx_line);
    auto const it_end = regex_it();

    int count = 1;
    std::vector<Rectangle> ret = ranges::make_iterator_range(it_begin, it_end) |
        ranges::view::transform([&count](std::match_results<std::string_view::iterator> const& match) -> Rectangle {
        assert(std::stoi(match[1]) == count);
        ++count;
        return Rectangle{std::stoi(match[2]), std::stoi(match[3]), std::stoi(match[4]), std::stoi(match[5])};
    });
    return ret;
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

int overlap_1d(int start1, int extent1, int start2, int extent2)
{
    if(start2 < start1) { std::swap(start1, start2); std::swap(extent1, extent2); }
    int block1_right_border = start1 + extent1;
    int distance_block1_right_block2_left = block1_right_border - start2;
    return std::max(0, std::min(distance_block1_right_block2_left, extent2));
}

int overlap(Rectangle const& lhs, Rectangle const& rhs)
{
    return overlap_1d(lhs.left, lhs.width, rhs.left, rhs.width) *
           overlap_1d(lhs.top, lhs.height, rhs.top, rhs.height);
}

int total_overlap(std::vector<Rectangle> const& rects)
{
    assert(rects.size() < std::numeric_limits<int>::max());
    int acc = 0;
    int const r_size = static_cast<int>(rects.size());
    for(int i1 = 0; i1 < r_size; ++i1) {
        for(int i2 = i1 + 1; i2 < r_size; ++i2) {
            acc += overlap(rects[i1], rects[i2]);
        }
    }
    return acc;
}

Rectangle getFieldDimensions(std::vector<Rectangle> const& rects)
{
    assert(!rects.empty());
    Rectangle ret = rects.front();
    for(auto const& r : rects) {
        ret.left = std::min(ret.left, r.left);
        ret.top = std::min(ret.top, r.top);
        int const width = (r.left + r.width) - ret.left + 1;
        ret.width = std::max(ret.width, width);
        int const height = (r.top + r.height) - ret.top + 1;
        ret.height = std::max(ret.height, height);
    }
    return ret;
}

Field::Field(Rectangle const& dimensions)
    :m_field(dimensions.width * dimensions.height),
     m_dimensions(dimensions)
{}

Rectangle Field::getDimensions() const
{
    return m_dimensions;
}

int Field::getCellIndex(int x, int y) const
{
    int const rel_x = x - m_dimensions.left;
    int const rel_y = y - m_dimensions.top;
    int const rel_width = m_dimensions.width;
    return (rel_y * rel_width) + rel_x;
}

int Field::getCell(int x, int y) const
{
    auto const idx = getCellIndex(x, y);
    return m_field[idx];
}

void Field::placeRectangle(Rectangle const& r)
{
    assert(overlap(m_dimensions, r) == r.width * r.height);
    int const base_y = (r.top - m_dimensions.top);
    int const base_x = (r.left - m_dimensions.left);
    for(int iy = 0; iy < r.height; ++iy) {
        int const y = r.top + iy;
        for(int ix = 0; ix < r.width; ++ix) {
            int const x = r.left + ix;
            auto const idx = getCellIndex(x, y);
            ++m_field[idx];
        }
    }
}

int Field::getOverlap() const
{
    auto const ret = ranges::count_if(m_field, [](int i) -> bool { return i >= 2; });
    assert(ret < std::numeric_limits<int>::max());
    return static_cast<int>(ret);
}

std::ostream& operator<<(std::ostream& os, Field const& f)
{
    int i = 1;
    int const stride = f.m_dimensions.width;
    for(auto const& c : f.m_field) {
        os << " " << c;
        if(i == stride) { i = 0; os << '\n'; }
        ++i;
    }
    os << '\n';
    return os;
}
