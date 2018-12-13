#include <plant_spreading.hpp>

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

    auto plants = Plants::parseFromInput(*input);
    for(int i=0; i<20; ++i) { plants.simulateStep(); }
    std::cout << "First result is " << plants.accumulate() << std::endl;
    plants = Plants::parseFromInput(*input);
    int64_t last_step = 0;
    int64_t diff = 0;
    int64_t target_step = 50000000000;
    int64_t precompute_steps = 500;
    for(int64_t i=0; i<precompute_steps; ++i) {
        plants.simulateStep();
        int64_t const new_step = plants.accumulate();
        diff = new_step - last_step;
        last_step = new_step;
    }
    std::cout << "Second result is " << (plants.accumulate() + diff * (target_step - precompute_steps)) << std::endl;

    return 0;
}
