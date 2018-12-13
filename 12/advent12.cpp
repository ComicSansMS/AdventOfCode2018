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
    //plants = Plants::parseFromInput(*input);
    //for(int64_t i=0; i<50000000000; ++i) {
    //    if((i%1000000) == 0) { std::cout << '.'; }
    //    plants.simulateStep();
    //}
    //std::cout << "Second result is " << plants.accumulate() << std::endl;

    return 0;
}
