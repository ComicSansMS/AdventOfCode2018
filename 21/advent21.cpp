#include <register_machine_ip.hpp>

#include <algorithm>
#include <cassert>
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

    RegisterMachine rm = parseInput(*input);
    int break_instruction = -1;
    int break_register = -1;
    for(int i = 0; i < static_cast<int>(rm.program.size()); ++i) {
        if((rm.program[i].opcode == Opcode::eqrr) && ((rm.program[i].input1 == 0) || (rm.program[i].input2 == 0))) {
            break_instruction = i;
            break_register = (rm.program[i].input1 == 0) ? rm.program[i].input2 : rm.program[i].input1;
            break;
        }
    }
    assert(break_instruction > 0);

    std::vector<int> results;
    while(step(rm)) {
        if(rm.ip == break_instruction) {
            if(std::find(begin(results), end(results), rm.registers[break_register]) != end(results)) {
                break;
            }
            results.push_back(rm.registers[break_register]);
        }
    }

    std::cout << "First result is " << results.front() << std::endl;
    std::cout << "Second result is " << results.back() << std::endl;

    return 0;
}
