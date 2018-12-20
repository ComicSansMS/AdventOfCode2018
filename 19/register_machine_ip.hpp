#ifndef ADVENT_OF_CODE_19_REGISTER_MACHINE_IP_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_19_REGISTER_MACHINE_IP_HPP_INCLUDE_GUARD

#include <array>
#include <string>
#include <string_view>
#include <vector>

enum class Opcode {
    addr = 0,
    addi,
    mulr,
    muli,
    banr,
    bani,
    borr,
    bori,
    setr,
    seti,
    gtir,
    gtri,
    gtrr,
    eqir,
    eqri,
    eqrr
};

struct Instruction {
    Opcode opcode;
    int input1;
    int input2;
    int output;
};

bool operator==(Instruction const& lhs, Instruction const& rhs);

using Registers = std::array<int, 6>;

struct RegisterMachine {
    int ip;
    int ip_bound_register;
    Registers registers;
    std::vector<Instruction> program;
};

Registers executeAddR(Instruction const& instruction, Registers const& registers);
Registers executeAddI(Instruction const& instruction, Registers const& registers);
Registers executeMulR(Instruction const& instruction, Registers const& registers);
Registers executeMulI(Instruction const& instruction, Registers const& registers);
Registers executeBAnR(Instruction const& instruction, Registers const& registers);
Registers executeBAnI(Instruction const& instruction, Registers const& registers);
Registers executeBOrR(Instruction const& instruction, Registers const& registers);
Registers executeBOrI(Instruction const& instruction, Registers const& registers);
Registers executeSetR(Instruction const& instruction, Registers const& registers);
Registers executeSetI(Instruction const& instruction, Registers const& registers);
Registers executeGTIR(Instruction const& instruction, Registers const& registers);
Registers executeGTRI(Instruction const& instruction, Registers const& registers);
Registers executeGTRR(Instruction const& instruction, Registers const& registers);
Registers executeEQIR(Instruction const& instruction, Registers const& registers);
Registers executeEQRI(Instruction const& instruction, Registers const& registers);
Registers executeEQRR(Instruction const& instruction, Registers const& registers);

using ExecuteFunction = Registers (*)(Instruction const&, Registers const&);

ExecuteFunction getFunctionFor(Opcode op);

bool checkPreconditions(Opcode op, Instruction ins);

Opcode parseOpcode(std::string const& str);

RegisterMachine parseInput(std::string_view input);

bool step(RegisterMachine& rm);

void runToCompletion(RegisterMachine& rm);

#endif
