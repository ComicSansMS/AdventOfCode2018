#include <game_of_woods.hpp>

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

    using namespace std::chrono_literals;
    Acre const a = parseInput(*input);
    std::cout << "First result is " << getTotalResourceValueAfter(a, 10min) << std::endl;

    auto const [acres, cycle_start] = findCycle(a);
    int64_t const cycle_length = (static_cast<int64_t>(acres.size()) - 1) - cycle_start;
    int64_t const target_iteration = 1'000'000'000;
    int64_t const target_cycle_offset = (target_iteration - cycle_start) % cycle_length;
    std::cout << "Second result is " << acres[cycle_start + target_cycle_offset].totalResourceValue() << std::endl;

    return 0;
}
