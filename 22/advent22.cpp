#include <spelunking.hpp>

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

    Scan const scan = parseInput(*input);

    Cave c(scan, scan.target_coords.x * 2, scan.target_coords.y * 2);
    //std::ofstream fout("out.log");
    //fout << c;

    std::cout << "First result is " << c.riskLevelToTarget() << std::endl;
    //std::cout << "Second result is " << "" << std::endl;

    return 0;
}
