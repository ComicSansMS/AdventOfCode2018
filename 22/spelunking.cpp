#include <spelunking.hpp>

#include <algorithm>
#include <cassert>
#include <iterator>
#include <limits>
#include <ostream>
#include <sstream>
#include <string>
#include <unordered_map>

Vec2::Vec2(int xx, int yy)
    :x(xx), y(yy)
{}

bool operator==(Vec2 const& lhs, Vec2 const& rhs)
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

Scan parseInput(std::string_view input)
{
    std::stringstream sstr(std::string{input});

    Scan ret;

    std::string line;
    std::getline(sstr, line);
    assert(line.substr(0, 7) == "depth: ");
    ret.depth = std::stoi(line.substr(7));

    std::getline(sstr, line);
    assert(line.substr(0, 8) == "target: ");
    line = line.substr(8);
    auto const it_sep = std::find(begin(line), end(line), ',');
    assert(it_sep != end(line));
    ret.target_coords.x = std::stoi(std::string(begin(line), it_sep));
    ret.target_coords.y = std::stoi(std::string(it_sep + 1, end(line)));

    return ret;
}

Cave::Cave(Scan const& n_scan, int64_t n_width, int64_t n_height)
    :width(n_width), height(n_height), scan(n_scan)
{
    assert((width > scan.target_coords.x) && (height > scan.target_coords.y));
    regions.reserve(width * height);
    erosion_levels.reserve(width * height);
    auto const geologic_index = [this](int64_t x, int64_t y) -> int64_t {
        if((x == 0) && (y == 0)) {
            return 0;
        } else if((x == scan.target_coords.x) && (y == scan.target_coords.y)) {
            return 0;
        } else if(y == 0) {
            return x * 16807;
        } else if(x == 0) {
            return y * 48271;
        } else {
            return erosion_levels[(y * width) + (x - 1)] * erosion_levels[((y - 1) * width) + x];
        }
    };

    for(int64_t y = 0; y < height; ++y) {
        for(int64_t x = 0; x < width; ++x) {
            int64_t const erosion_level = (geologic_index(x, y) + scan.depth) % 20183;
            erosion_levels.push_back(erosion_level);
            Region const r = static_cast<Region>(erosion_level % 3);
            regions.push_back(r);
        }
    }
}

int Cave::riskLevelToTarget() const
{
    int acc = 0;
    for(int y = 0; y <= scan.target_coords.y; ++y) {
        for(int x = 0; x <= scan.target_coords.x; ++x) {
            acc += static_cast<int>(regions[y * width + x]);
        }
    }
    return acc;
}

std::ostream& operator<<(std::ostream& os, Cave const& c)
{
    for(int y = 0; y < c.height; ++y) {
        for(int x = 0; x < c.width; ++x) {
            if((x == 0) && (y == 0)) { os << 'M'; continue; }
            else if((x == c.scan.target_coords.x) && (y == c.scan.target_coords.y)) { os << 'T'; continue; }
            Region const r = c.regions[(y * c.width) + x];
            os << [r]() {
                if(r == Region::Rocky) {
                    return '.';
                } else if(r == Region::Wet) {
                    return '=';
                } else {
                    assert(r == Region::Narrow);
                    return '|';
                }
            }();
        }
        os << '\n';
    }
    return os;
}
