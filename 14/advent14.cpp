#include <hot_chocolate.hpp>

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

    std::string line;
    std::getline(fin, line);
    if(!fin) {
        std::cerr << "Unable to read input from file '" << filename << "'." << std::endl;
        return std::nullopt;
    }
    return line;
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

    int const target_number = std::stoi(*input);
    std::vector<int> const target_sequence = parseInput(*input);

    std::cout << "First result is " << RecipeCombiner().cook_until(target_number) << std::endl;
    std::cout << "Second result is " << RecipeCombiner().cook_until2(target_sequence) << std::endl;

    return 0;
}
