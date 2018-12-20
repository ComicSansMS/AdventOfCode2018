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

int manual_compute(int r5)
{
    int r0 = 0;
    for(int r1 = 1; r1 <= r5; ++r1) {
        for(int r3 = 1; r3 <= r5; ++r3) {
            if(r1 * r3 == r5) {
                r0 += r1;
            }
        }
    }
    return r0;
}

int manual_compute_fast(int r5)
{
    int r0 = r5;
    for(int r1 = 1; r1 <= r5 / 2; ++r1) {
        if(r5 % r1 == 0) {
            r0 += r1;
        }
    }
    return r0;
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

    RegisterMachine rm2 = rm;
    rm2.registers[0] = 1;
    while(rm2.ip != 1) { step(rm2); }
    int const target_value = rm2.registers[5];
    int const result = manual_compute_fast(target_value);
    std::cout << "Second result is " << result << std::endl;

    return 0;
}
