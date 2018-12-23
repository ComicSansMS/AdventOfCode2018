#include <spelunking.hpp>

#include <algorithm>
#include <cassert>
#include <iterator>
#include <limits>
#include <ostream>
#include <queue>
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

Spelunker::Spelunker(int n_due_time)
    :position(0, 0), equipped(Tool::Torch), due_time(n_due_time)
{}

int Cave::shortestPaths() const
{
    struct Comparator {
        bool operator()(Spelunker const& lhs, Spelunker const& rhs) const { return lhs.due_time > rhs.due_time; };
    };
    std::priority_queue<Spelunker, std::vector<Spelunker>, Comparator> queue;
    queue.push(Spelunker(0));

    auto const tool_matches_field = [](Tool t, Region r) -> bool {
        if(t == Tool::Torch) {
            return (r != Region::Wet);
        } else if(t == Tool::ClimbingGear) {
            return (r != Region::Narrow);
        } else {
            assert(t == Tool::Neither);
            return (r != Region::Rocky);
        }
    };

    struct ReachTime {
        int withTorch;
        int withClimbingGear;
        int withNeither;

        ReachTime()
            :withTorch(std::numeric_limits<int>::max()),
             withClimbingGear(std::numeric_limits<int>::max()),
             withNeither(std::numeric_limits<int>::max())
        {}

        void set(Tool t, int time) {
            if(t == Tool::Torch) {
                withTorch = std::min(withTorch, time);
            } else if(t == Tool::ClimbingGear) {
                withClimbingGear = std::min(withClimbingGear, time);
            } else {
                withNeither = std::min(withNeither, time);
            }
        }

        int get(Tool t) const {
            if(t == Tool::Torch) {
                return withTorch;
            } else if(t == Tool::ClimbingGear) {
                return withClimbingGear;
            } else {
                return withNeither;
            }
        }
    };

    std::vector<ReachTime> time_to_reach;
    time_to_reach.resize(width*height);

    auto push_neighbor = [this, tool_matches_field, &queue, &time_to_reach](Spelunker const& s, Vec2 new_pos) {
        Region const target_field = regions[(new_pos.y * width) + new_pos.x];
        if(tool_matches_field(s.equipped, target_field)) {
            Spelunker new_s = s;
            new_s.position = new_pos;
            assert(new_s.equipped == s.equipped);
            ++new_s.due_time;
            if(new_s.due_time < time_to_reach[new_pos.y * width + new_pos.x].get(new_s.equipped)) {
                queue.push(new_s);
            }
        } else {
            Spelunker new_s = s;
            new_s.position = new_pos;
            new_s.due_time += 8;
            new_s.equipped = static_cast<Tool>((static_cast<int>(s.equipped) + 1) % 3);
            if(new_s.due_time < time_to_reach[new_pos.y * width + new_pos.x].get(new_s.equipped)) {
                queue.push(new_s);
            }
            new_s.equipped = static_cast<Tool>((static_cast<int>(s.equipped) + 2) % 3);
            if(new_s.due_time < time_to_reach[new_pos.y * width + new_pos.x].get(new_s.equipped)) {
                queue.push(new_s);
            }
        }
    };

    int time = -1;
    while(!queue.empty()) {
        assert(time < queue.top().due_time);
        time = queue.top().due_time;
        while((!queue.empty()) && (queue.top().due_time == time)) {
            Spelunker const s = queue.top();
            queue.pop();
            auto& ttr = time_to_reach[(s.position.y * width) + s.position.x];
            if(ttr.get(s.equipped) <= s.due_time) { continue; }
            ttr.set(s.equipped, s.due_time);
            // top
            if(s.position.y > 0)          { push_neighbor(s, Vec2(s.position.x, s.position.y - 1)); }
            // bottom
            if(s.position.y < height - 1) { push_neighbor(s, Vec2(s.position.x, s.position.y + 1)); }
            // left
            if(s.position.x > 0)          { push_neighbor(s, Vec2(s.position.x - 1, s.position.y)); }
            // right
            if(s.position.x < width - 1)  { push_neighbor(s, Vec2(s.position.x + 1, s.position.y)); }
        }
    }
    auto ttr_target = time_to_reach[(scan.target_coords.y * width) + scan.target_coords.x];
    return std::min(ttr_target.withTorch + 7, ttr_target.withClimbingGear + 7);
}
