#include <assembly_reverse.hpp>

#include <algorithm>
#include <cassert>
#include <iterator>
#include <limits>
#include <numeric>
#include <regex>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>

Program parseInput(std::string_view input)
{
    std::regex rx_line_sample_before(R"(^Before: \[(\d+), (\d+), (\d+), (\d+)\]$)");
    std::regex rx_line_sample_after (R"(^After:  \[(\d+), (\d+), (\d+), (\d+)\]$)");
    std::regex rx_line_instruction(R"(^(\d+) (\d+) (\d+) (\d+)$)");

    std::stringstream sstr(std::string{input});
    std::string line;
    Program ret;
    while(std::getline(sstr, line)) {
        if(line.empty()) {
            std::getline(sstr, line);
            assert(line.empty());
            break;
        }

        ret.samples.emplace_back();

        std::smatch smatch_before;
        std::regex_match(line, smatch_before, rx_line_sample_before);
        ret.samples.back().registers_before[0] = std::stoi(smatch_before[1]);
        ret.samples.back().registers_before[1] = std::stoi(smatch_before[2]);
        ret.samples.back().registers_before[2] = std::stoi(smatch_before[3]);
        ret.samples.back().registers_before[3] = std::stoi(smatch_before[4]);

        std::getline(sstr, line);
        std::smatch smatch_instruction;
        std::regex_match(line, smatch_instruction, rx_line_instruction);
        ret.samples.back().instruction.Opcode = std::stoi(smatch_instruction[1]);
        ret.samples.back().instruction.Input1 = std::stoi(smatch_instruction[2]);
        ret.samples.back().instruction.Input2 = std::stoi(smatch_instruction[3]);
        ret.samples.back().instruction.Output = std::stoi(smatch_instruction[4]);

        std::getline(sstr, line);
        std::smatch smatch_after;
        std::regex_match(line, smatch_after, rx_line_sample_after);
        ret.samples.back().registers_after[0] = std::stoi(smatch_before[1]);
        ret.samples.back().registers_after[1] = std::stoi(smatch_before[2]);
        ret.samples.back().registers_after[2] = std::stoi(smatch_before[3]);
        ret.samples.back().registers_after[3] = std::stoi(smatch_before[4]);

        std::getline(sstr, line);
        assert(line.empty());
    }

    while(std::getline(sstr, line)) {
        std::smatch smatch_instruction;
        std::regex_match(line, smatch_instruction, rx_line_instruction);
        ret.program.push_back(Instruction{ std::stoi(smatch_instruction[1]), std::stoi(smatch_instruction[2]),
                                           std::stoi(smatch_instruction[3]), std::stoi(smatch_instruction[4]) });
    }

    return ret;
}

namespace {
bool checkRegisterIndex(int register_index) {
    return (register_index >= 0) && (register_index < 4);
}
}

Registers executeAddR(Instruction const& instruction, Registers const& registers)
{
    assert(checkRegisterIndex(instruction.Input1));
    assert(checkRegisterIndex(instruction.Input2));
    assert(checkRegisterIndex(instruction.Output));
    Registers ret = registers;
    ret[instruction.Output] = ret[instruction.Input1] + ret[instruction.Input2];
    return ret;
}

Registers executeAddI(Instruction const& instruction, Registers const& registers)
{
    assert(checkRegisterIndex(instruction.Input1));
    assert(checkRegisterIndex(instruction.Output));
    Registers ret = registers;
    ret[instruction.Output] = ret[instruction.Input1] + instruction.Input2;
    return ret;
}


Registers executeMulR(Instruction const& instruction, Registers const& registers)
{
    assert(checkRegisterIndex(instruction.Input1));
    assert(checkRegisterIndex(instruction.Input2));
    assert(checkRegisterIndex(instruction.Output));
    Registers ret = registers;
    ret[instruction.Output] = ret[instruction.Input1] * ret[instruction.Input2];
    return ret;
}


Registers executeMulI(Instruction const& instruction, Registers const& registers)
{
    assert(checkRegisterIndex(instruction.Input1));
    assert(checkRegisterIndex(instruction.Output));
    Registers ret = registers;
    ret[instruction.Output] = ret[instruction.Input1] * instruction.Input2;
    return ret;
}


Registers executeBAnR(Instruction const& instruction, Registers const& registers)
{
    assert(checkRegisterIndex(instruction.Input1));
    assert(checkRegisterIndex(instruction.Input2));
    assert(checkRegisterIndex(instruction.Output));
    Registers ret = registers;
    ret[instruction.Output] = ret[instruction.Input1] & ret[instruction.Input2];
    return ret;
}


Registers executeBAnI(Instruction const& instruction, Registers const& registers)
{
    assert(checkRegisterIndex(instruction.Input1));
    assert(checkRegisterIndex(instruction.Output));
    Registers ret = registers;
    ret[instruction.Output] = ret[instruction.Input1] & instruction.Input2;
    return ret;
}


Registers executeBOrR(Instruction const& instruction, Registers const& registers)
{
    assert(checkRegisterIndex(instruction.Input1));
    assert(checkRegisterIndex(instruction.Input2));
    assert(checkRegisterIndex(instruction.Output));
    Registers ret = registers;
    ret[instruction.Output] = ret[instruction.Input1] | ret[instruction.Input2];
    return ret;
}


Registers executeBOrI(Instruction const& instruction, Registers const& registers)
{
    assert(checkRegisterIndex(instruction.Input1));
    assert(checkRegisterIndex(instruction.Output));
    Registers ret = registers;
    ret[instruction.Output] = ret[instruction.Input1] | instruction.Input2;
    return ret;
}


Registers executeSetR(Instruction const& instruction, Registers const& registers)
{
    assert(checkRegisterIndex(instruction.Input1));
    assert(checkRegisterIndex(instruction.Output));
    Registers ret = registers;
    ret[instruction.Output] = ret[instruction.Input1];
    return ret;
}


Registers executeSetI(Instruction const& instruction, Registers const& registers)
{
    assert(checkRegisterIndex(instruction.Output));
    Registers ret = registers;
    ret[instruction.Output] = instruction.Input1;
    return ret;
}


Registers executeGTIR(Instruction const& instruction, Registers const& registers)
{
    assert(checkRegisterIndex(instruction.Input2));
    assert(checkRegisterIndex(instruction.Output));
    Registers ret = registers;
    ret[instruction.Output] = (instruction.Input1 > ret[instruction.Input2]) ? 1 : 0;
    return ret;
}


Registers executeGTRI(Instruction const& instruction, Registers const& registers)
{
    assert(checkRegisterIndex(instruction.Input1));
    assert(checkRegisterIndex(instruction.Output));
    Registers ret = registers;
    ret[instruction.Output] = (ret[instruction.Input1] > instruction.Input2) ? 1 : 0;
    return ret;
}


Registers executeGTRR(Instruction const& instruction, Registers const& registers)
{
    assert(checkRegisterIndex(instruction.Input1));
    assert(checkRegisterIndex(instruction.Input2));
    assert(checkRegisterIndex(instruction.Output));
    Registers ret = registers;
    ret[instruction.Output] = (ret[instruction.Input1] > ret[instruction.Input2]) ? 1 : 0;
    return ret;
}


Registers executeEQIR(Instruction const& instruction, Registers const& registers)
{
    assert(checkRegisterIndex(instruction.Input2));
    assert(checkRegisterIndex(instruction.Output));
    Registers ret = registers;
    ret[instruction.Output] = (instruction.Input1 == ret[instruction.Input2]) ? 1 : 0;
    return ret;
}


Registers executeEQRI(Instruction const& instruction, Registers const& registers)
{
    assert(checkRegisterIndex(instruction.Input1));
    assert(checkRegisterIndex(instruction.Output));
    Registers ret = registers;
    ret[instruction.Output] = (ret[instruction.Input1] == instruction.Input2) ? 1 : 0;
    return ret;
}


Registers executeEQRR(Instruction const& instruction, Registers const& registers)
{
    assert(checkRegisterIndex(instruction.Input1));
    assert(checkRegisterIndex(instruction.Input2));
    assert(checkRegisterIndex(instruction.Output));
    Registers ret = registers;
    ret[instruction.Output] = (ret[instruction.Input1] == ret[instruction.Input2]) ? 1 : 0;
    return ret;
}

ExecuteFunction getFunctionFor(Opcode op)
{
    if(op == Opcode::addr) {
        return executeAddR;
    } else if(op == Opcode::addi) {
        return executeAddI;
    } else if(op == Opcode::mulr) {
        return executeMulR;
    } else if(op == Opcode::muli) {
        return executeMulI;
    } else if(op == Opcode::banr) {
        return executeBAnR;
    } else if(op == Opcode::bani) {
        return executeBAnI;
    } else if(op == Opcode::borr) {
        return executeBOrR;
    } else if(op == Opcode::bori) {
        return executeBOrI;
    } else if(op == Opcode::setr) {
        return executeSetR;
    } else if(op == Opcode::seti) {
        return executeSetI;
    } else if(op == Opcode::gtir) {
        return executeGTIR;
    } else if(op == Opcode::gtri) {
        return executeGTRI;
    } else if(op == Opcode::gtrr) {
        return executeGTRR;
    } else if(op == Opcode::eqir) {
        return executeEQIR;
    } else if(op == Opcode::eqri) {
        return executeEQRI;
    } else {
        assert(op == Opcode::eqrr);
        return executeEQRR;
    }
}

bool checkPreconditions(Opcode op, Instruction ins)
{
    if(op == Opcode::addr) {
        return checkRegisterIndex(ins.Input1) && checkRegisterIndex(ins.Input2) && checkRegisterIndex(ins.Output);
    } else if(op == Opcode::addi) {
        return checkRegisterIndex(ins.Input1) && checkRegisterIndex(ins.Output);
    } else if(op == Opcode::mulr) {
        return checkRegisterIndex(ins.Input1) && checkRegisterIndex(ins.Input2) && checkRegisterIndex(ins.Output);
    } else if(op == Opcode::muli) {
        return checkRegisterIndex(ins.Input1) && checkRegisterIndex(ins.Output);
    } else if(op == Opcode::banr) {
        return checkRegisterIndex(ins.Input1) && checkRegisterIndex(ins.Input2) && checkRegisterIndex(ins.Output);
    } else if(op == Opcode::bani) {
        return checkRegisterIndex(ins.Input1) && checkRegisterIndex(ins.Output);
    } else if(op == Opcode::borr) {
        return checkRegisterIndex(ins.Input1) && checkRegisterIndex(ins.Input2) && checkRegisterIndex(ins.Output);
    } else if(op == Opcode::bori) {
        return checkRegisterIndex(ins.Input1) && checkRegisterIndex(ins.Output);
    } else if(op == Opcode::setr) {
        return checkRegisterIndex(ins.Input1) && checkRegisterIndex(ins.Output);
    } else if(op == Opcode::seti) {
        return checkRegisterIndex(ins.Output);
    } else if(op == Opcode::gtir) {
        return checkRegisterIndex(ins.Input2) && checkRegisterIndex(ins.Output);
    } else if(op == Opcode::gtri) {
        return checkRegisterIndex(ins.Input1) && checkRegisterIndex(ins.Output);
    } else if(op == Opcode::gtrr) {
        return checkRegisterIndex(ins.Input1) && checkRegisterIndex(ins.Input2) && checkRegisterIndex(ins.Output);
    } else if(op == Opcode::eqir) {
        return checkRegisterIndex(ins.Input2) && checkRegisterIndex(ins.Output);
    } else if(op == Opcode::eqri) {
        return checkRegisterIndex(ins.Input1) && checkRegisterIndex(ins.Output);
    } else {
        assert(op == Opcode::eqrr);
        return checkRegisterIndex(ins.Input1) && checkRegisterIndex(ins.Input2) && checkRegisterIndex(ins.Output);
    }
}

std::vector<Opcode> guessingGame(InstructionSample const& s)
{
    std::vector<Opcode> ret;
    for(int i = 0; i < 16; ++i) {
        Opcode opc = static_cast<Opcode>(i);
        if(checkPreconditions(opc, s.instruction)) {
            if(getFunctionFor(opc)(s.instruction, s.registers_before) == s.registers_after) {
                ret.push_back(opc);
            }
        }
    }
    return ret;
}

int countSamples3OrMore(Program const& p)
{
    return static_cast<int>(
        std::count_if(begin(p.samples), end(p.samples),
                      [](InstructionSample const& s) { return guessingGame(s).size() >= 3; }));
}

std::array<Opcode, 16> determineOpcodes(Program const& p)
{
    std::vector<std::vector<Opcode>> guesses;
    guesses.reserve(p.samples.size());
    for(auto const& s : p.samples) {
        guesses.push_back(guessingGame(s));
    }

    std::unordered_map<int, Opcode> deduced_ops;
    while(deduced_ops.size() < 16) {
        for(int i = 0; i < p.samples.size(); ++i) {
            int const opc = p.samples[i].instruction.Opcode;
            auto const& g = guesses[i];
            if(g.size() == 1) {
                if(deduced_ops.find(opc) == end(deduced_ops)) {
                    Opcode const deduced_opcode = g.back();
                    deduced_ops[opc] = deduced_opcode;
                    for(auto& gs : guesses) {
                        gs.erase(std::remove(begin(gs), end(gs), deduced_opcode), end(gs));
                    }
                } else {
                    assert(deduced_ops[opc] == g.back());
                }
            }
        }
    }

    std::array<Opcode, 16> ret;
    for(int i = 0; i < 16; ++i) {
        assert(deduced_ops.find(i) != end(deduced_ops));
        ret[i] = deduced_ops[i];
    }
    return ret;
}

Registers executeProgram(Program const& p, std::array<Opcode, 16> const& opcode_map)
{
    Registers r = {0, 0, 0, 0};
    for(auto const& i : p.program) {
        Opcode const opc = opcode_map[i.Opcode];
        r = getFunctionFor(opc)(i, r);
    }
    return r;
}
