#ifndef ADVENT_OF_CODE_16_ASSEMBLY_REVERSE_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_16_ASSEMBLY_REVERSE_HPP_INCLUDE_GUARD

#include <array>
#include <string_view>
#include <vector>

struct Instruction {
    int Opcode;
    int Input1;
    int Input2;
    int Output;
};

using Registers = std::array<int, 4>;

struct InstructionSample {
    Registers registers_before;
    Instruction instruction;
    Registers registers_after;
};

struct Program {
    std::vector<InstructionSample> samples;
    std::vector<Instruction> program;
};

Program parseInput(std::string_view input);

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

using ExecuteFunction = Registers (*)(Instruction const&, Registers const&);

ExecuteFunction getFunctionFor(Opcode op);

bool checkPreconditions(Opcode op, Instruction ins);

std::vector<Opcode> guessingGame(InstructionSample const& s);

int countSamples3OrMore(Program const& p);

#endif
