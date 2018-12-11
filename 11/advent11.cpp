#include <fuel_cell_grid.hpp>

#include <fstream>
#include <iostream>
#include <optional>
#include <string>

std::optional<std::string> readInput(char const* filename)
{
    std::ifstream fin(filename);
    if(!fin) {
        std::cerr << "Unable to open input file '" << filename << "' for reading." << std::endl;
        return std::nullopt;
    }

    std::string ret;
    std::getline(fin, ret);
    if(!fin) {
        std::cerr << "Unable to read input from file '" << filename << "'." << std::endl;
        return std::nullopt;
    }
    return ret;
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

    int const grid_serial_number = std::stoi(*input);

    auto const [p1, x1, y1] = getLargestSquare(grid_serial_number, 3);
    std::cout << "First result is " << x1 << "," << y1 << std::endl;
    auto const [p2, x2, y2, s2] = getLargestOfAllSquares(grid_serial_number);
    std::cout << "Second result is " << x2 << "," << y2 << "," << s2 << std::endl;

    return 0;
}
