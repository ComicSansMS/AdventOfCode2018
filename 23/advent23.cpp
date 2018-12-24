#include <nanobots.hpp>

#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <optional>
#include <queue>
#include <sstream>
#include <string>

std::optional<std::string> readInput(char const* filename)
{
    std::ifstream fin(filename);
    if(!fin) {
        std::cerr << "Unable to open input file '" << filename << "' for reading." << std::endl;
        return std::nullopt;
    }

    std::stringstream sstr;
    sstr << fin.rdbuf();
    if(!fin) {
        std::cerr << "Unable to read input from file '" << filename << "'." << std::endl;
        return std::nullopt;
    }
    return sstr.str();
}

struct QueueEntry {
    Bounding bounding;
    int intersections;
};

struct Comparator {
    bool operator()(QueueEntry const& lhs, QueueEntry const& rhs) const { return lhs.intersections < rhs.intersections; };
};


int main(int argc, char* argv[])
{
    char const* input_filename = "input";
    if(argc == 2) {
        input_filename = argv[1];
    }

    auto const input = readInput(input_filename);

    if(!input) {
        return 1;
    }

    auto const bots = parseInput(*input);
    Bounding bounding = getBounding(bots);
    std::priority_queue<QueueEntry, std::vector<QueueEntry>, Comparator> queue;

    auto count_intersections = [&bots](Bounding const& brect) -> int {
        return static_cast<int>(
            std::count_if(begin(bots), end(bots),
                          [&brect](Nanobot const& b) { return intersects(b, brect); }));
    };
    queue.push(QueueEntry{bounding, count_intersections(bounding)});

    auto check_dimensions = [](Bounding const& brect, int limit) {
        return ((brect.max.x - brect.min.x) < limit) &&
               ((brect.max.y - brect.min.y) < limit) &&
               ((brect.max.z - brect.min.z) < limit);
    };

    auto split_bounding = [](Bounding const& brect) -> std::array<Bounding, 8> {
        int const halfway_x = ((brect.max.x - brect.min.x) / 2) + brect.min.x;
        int const halfway_y = ((brect.max.y - brect.min.y) / 2) + brect.min.y;
        int const halfway_z = ((brect.max.z - brect.min.z) / 2) + brect.min.z;
        assert((brect.min.x < halfway_x) && (brect.max.x > halfway_x));
        assert((brect.min.y < halfway_y) && (brect.max.y > halfway_y));
        assert((brect.min.z < halfway_z) && (brect.max.z > halfway_z));
        std::array<Bounding, 8> ret;
        ret.fill(brect);
        ret[0].max.x = halfway_x;
        ret[0].max.y = halfway_y;
        ret[0].max.z = halfway_z;
        ret[1].min.x = halfway_x;
        ret[1].max.y = halfway_y;
        ret[1].max.z = halfway_z;
        ret[2].max.x = halfway_x;
        ret[2].min.y = halfway_y;
        ret[2].max.z = halfway_z;
        ret[3].min.x = halfway_x;
        ret[3].min.y = halfway_y;
        ret[3].max.z = halfway_z;

        ret[4].max.x = halfway_x;
        ret[4].max.y = halfway_y;
        ret[4].min.z = halfway_z;
        ret[5].min.x = halfway_x;
        ret[5].max.y = halfway_y;
        ret[5].min.z = halfway_z;
        ret[6].max.x = halfway_x;
        ret[6].min.y = halfway_y;
        ret[6].min.z = halfway_z;
        ret[7].min.x = halfway_x;
        ret[7].min.y = halfway_y;
        ret[7].min.z = halfway_z;

        return ret;
    };

    while(true) {
        auto const [brect, n_int] = queue.top();
        if(check_dimensions(brect, 50)) {
            bounding = brect; break;
        }
        queue.pop();
        auto sub_boundings = split_bounding(brect);
        for(auto const& s : sub_boundings) {
            queue.push(QueueEntry{ s, count_intersections(s) });
        }
    }
    auto const ov = overlaps(bounding, bots);
    std::cout << " " << ov.max_coords.x << "," << ov.max_coords.y << "," << ov.max_coords.z << "  " << ov.max_overlap << std::endl;

    std::cout << "First result is " << inRangeOfLargestSignal(bots) << std::endl;
    std::cout << "Second result is " << (std::abs(ov.max_coords.x) + std::abs(ov.max_coords.y) + std::abs(ov.max_coords.z)) << std::endl;

    return 0;
}
