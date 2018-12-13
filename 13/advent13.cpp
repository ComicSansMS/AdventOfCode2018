#include <mine_cart_madness.hpp>

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

    auto field = parseInput(*input);
    std::vector<Cart> crash;
    while(crash.empty()) { crash = simulateStep(field); }
    std::cout << "First result is "
              << std::get<0>(crash.front().coords) << ","
              << std::get<1>(crash.front().coords) << std::endl;
    //std::cout << "Second result is " << (plants.accumulate() + diff * (target_step - precompute_steps)) << std::endl;

    return 0;
}
