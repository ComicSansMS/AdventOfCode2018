#include <lib.hpp>

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

    std::string input;
    std::getline(fin, input);
    if(!fin) {
        std::cerr << "Unable to read input from file '" << filename << "'." << std::endl;
        return std::nullopt;
    }
    return input;
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

    std::stringstream sstr(*input);
    int result01, result02;
    sstr >> result01 >> result02;
    std::cout << "First result is " << foo(result01) << std::endl;

    std::cout << "Second result is " << foo(result02) << std::endl;
    return 0;
}
