#include <nanobots.hpp>

#include <fstream>
#include <iostream>
#include <optional>
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

    int exponent = 23;
    std::vector<Nanobot> b2 = bots;
    for(auto& bb : b2) {
        bb.position.x >>= exponent;
        bb.position.y >>= exponent;
        bb.position.z >>= exponent;
        bb.radius >>= exponent;
    }
    auto bounding = getBounding(b2);
    auto ov = overlaps(bounding, b2);

    while(exponent > 0) {
        --exponent;
        b2 = bots;
        for(auto& bb : b2) {
            bb.position.x >>= exponent;
            bb.position.y >>= exponent;
            bb.position.z >>= exponent;
            bb.radius >>= exponent;
        }
        bounding = getBounding(b2);
        int halfpoint_x = (bounding.max.x - bounding.min.x) / 2;
        int halfpoint_y = (bounding.max.y - bounding.min.y) / 2;
        int halfpoint_z = (bounding.max.z - bounding.min.z) / 2;

        if(halfpoint_x < ov.max_coords.x) { bounding.min.x = halfpoint_x; } else { bounding.max.x = halfpoint_x; }
        if(halfpoint_y < ov.max_coords.y) { bounding.min.y = halfpoint_y; } else { bounding.max.y = halfpoint_y; }
        if(halfpoint_z < ov.max_coords.z) { bounding.min.z = halfpoint_z; } else { bounding.max.z = halfpoint_z; }
        ov = overlaps(bounding, b2);
        std::cout << " - " << exponent << " (" << ov.max_coords.x << "," << ov.max_coords.y << "," << ov.max_coords.z << ") " << ov.max_overlap << std::endl;
    }

    std::cout << "First result is " << inRangeOfLargestSignal(bots) << std::endl;
    //std::cout << "Second result is " << "" << std::endl;

    return 0;
}
