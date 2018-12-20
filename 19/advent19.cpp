#include <register_machine_ip.hpp>

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

    RegisterMachine const rm = parseInput(*input);
    RegisterMachine rm1 = rm;
    runToCompletion(rm1);
    std::cout << "First result is " << rm1.registers[0] << std::endl;
    //RegisterMachine rm2 = rm;
    //rm2.registers[0] = 1;
    //runToCompletion(rm2);
    //std::cout << "Second result is " << rm2.registers[0] << std::endl;

    return 0;
}
