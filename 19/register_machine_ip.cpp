#include <register_machine_ip.hpp>

#include <range/v3/core.hpp>
#include <range/v3/view/split.hpp>

#include <algorithm>
#include <cassert>
#include <iterator>
#include <limits>
#include <regex>

bool operator==(Instruction const& lhs, Instruction const& rhs)
{
    return (lhs.opcode == rhs.opcode) &&
           (lhs.input1 == rhs.input1) &&
           (lhs.input2 == rhs.input2) &&
           (lhs.output == rhs.output);
}

Opcode parseOpcode(std::string const& str)
{
    if(str == "addr") {
        return Opcode::addr;
    } else if(str == "addi") {
        return Opcode::addi;
    } else if(str == "mulr") {
        return Opcode::mulr;
    } else if(str == "muli") {
        return Opcode::muli;
    } else if(str == "banr") {
        return Opcode::banr;
    } else if(str == "bani") {
        return Opcode::bani;
    } else if(str == "borr") {
        return Opcode::borr;
    } else if(str == "bori") {
        return Opcode::bori;
    } else if(str == "setr") {
        return Opcode::setr;
    } else if(str == "seti") {
        return Opcode::seti;
    } else if(str == "gtir") {
        return Opcode::gtir;
    } else if(str == "gtri") {
        return Opcode::gtri;
    } else if(str == "gtrr") {
        return Opcode::gtrr;
    } else if(str == "eqir") {
        return Opcode::eqir;
    } else if(str == "eqri") {
        return Opcode::eqri;
    } else {
        assert(str == "eqrr");
        return Opcode::eqrr;
    }
}

RegisterMachine parseInput(std::string_view input) {
    RegisterMachine ret{ 0, -1, Registers{}, {} };

    std::regex rx_line(R"((#ip (\d+))|((\w{4}) (\d+) (\d+) (\d+)))");
    using regex_it = std::regex_iterator<std::string_view::iterator>;
    auto const it_begin = regex_it(begin(input) /*+ end_first_line*/, end(input), rx_line);
    auto const it_end = regex_it();

    for(auto it = it_begin; it != it_end; ++it) {
        auto match = *it;
        if(match[1].matched) {
            assert(ret.ip_bound_register == -1);
            ret.ip_bound_register = std::stoi(match[2]);
        } else {
            assert(match[3].matched);
            Opcode const opc = parseOpcode(match[4].str());
            ret.program.push_back(Instruction{ opc, std::stoi(match[5]),
                                               std::stoi(match[6]), std::stoi(match[7]) });
        }
    }

    return ret;
}


namespace {
    bool checkRegisterIndex(int register_index) {
        return (register_index >= 0) && (register_index < 6);
    }
}

Registers executeAddR(Instruction const& instruction, Registers const& registers)
{
    assert(checkRegisterIndex(instruction.input1));
    assert(checkRegisterIndex(instruction.input2));
    assert(checkRegisterIndex(instruction.output));
    Registers ret = registers;
    ret[instruction.output] = ret[instruction.input1] + ret[instruction.input2];
    return ret;
}

Registers executeAddI(Instruction const& instruction, Registers const& registers)
{
    assert(checkRegisterIndex(instruction.input1));
    assert(checkRegisterIndex(instruction.output));
    Registers ret = registers;
    ret[instruction.output] = ret[instruction.input1] + instruction.input2;
    return ret;
}


Registers executeMulR(Instruction const& instruction, Registers const& registers)
{
    assert(checkRegisterIndex(instruction.input1));
    assert(checkRegisterIndex(instruction.input2));
    assert(checkRegisterIndex(instruction.output));
    Registers ret = registers;
    ret[instruction.output] = ret[instruction.input1] * ret[instruction.input2];
    return ret;
}


Registers executeMulI(Instruction const& instruction, Registers const& registers)
{
    assert(checkRegisterIndex(instruction.input1));
    assert(checkRegisterIndex(instruction.output));
    Registers ret = registers;
    ret[instruction.output] = ret[instruction.input1] * instruction.input2;
    return ret;
}


Registers executeBAnR(Instruction const& instruction, Registers const& registers)
{
    assert(checkRegisterIndex(instruction.input1));
    assert(checkRegisterIndex(instruction.input2));
    assert(checkRegisterIndex(instruction.output));
    Registers ret = registers;
    ret[instruction.output] = ret[instruction.input1] & ret[instruction.input2];
    return ret;
}


Registers executeBAnI(Instruction const& instruction, Registers const& registers)
{
    assert(checkRegisterIndex(instruction.input1));
    assert(checkRegisterIndex(instruction.output));
    Registers ret = registers;
    ret[instruction.output] = ret[instruction.input1] & instruction.input2;
    return ret;
}


Registers executeBOrR(Instruction const& instruction, Registers const& registers)
{
    assert(checkRegisterIndex(instruction.input1));
    assert(checkRegisterIndex(instruction.input2));
    assert(checkRegisterIndex(instruction.output));
    Registers ret = registers;
    ret[instruction.output] = ret[instruction.input1] | ret[instruction.input2];
    return ret;
}


Registers executeBOrI(Instruction const& instruction, Registers const& registers)
{
    assert(checkRegisterIndex(instruction.input1));
    assert(checkRegisterIndex(instruction.output));
    Registers ret = registers;
    ret[instruction.output] = ret[instruction.input1] | instruction.input2;
    return ret;
}


Registers executeSetR(Instruction const& instruction, Registers const& registers)
{
    assert(checkRegisterIndex(instruction.input1));
    assert(checkRegisterIndex(instruction.output));
    Registers ret = registers;
    ret[instruction.output] = ret[instruction.input1];
    return ret;
}


Registers executeSetI(Instruction const& instruction, Registers const& registers)
{
    assert(checkRegisterIndex(instruction.output));
    Registers ret = registers;
    ret[instruction.output] = instruction.input1;
    return ret;
}


Registers executeGTIR(Instruction const& instruction, Registers const& registers)
{
    assert(checkRegisterIndex(instruction.input2));
    assert(checkRegisterIndex(instruction.output));
    Registers ret = registers;
    ret[instruction.output] = (instruction.input1 > ret[instruction.input2]) ? 1 : 0;
    return ret;
}


Registers executeGTRI(Instruction const& instruction, Registers const& registers)
{
    assert(checkRegisterIndex(instruction.input1));
    assert(checkRegisterIndex(instruction.output));
    Registers ret = registers;
    ret[instruction.output] = (ret[instruction.input1] > instruction.input2) ? 1 : 0;
    return ret;
}


Registers executeGTRR(Instruction const& instruction, Registers const& registers)
{
    assert(checkRegisterIndex(instruction.input1));
    assert(checkRegisterIndex(instruction.input2));
    assert(checkRegisterIndex(instruction.output));
    Registers ret = registers;
    ret[instruction.output] = (ret[instruction.input1] > ret[instruction.input2]) ? 1 : 0;
    return ret;
}


Registers executeEQIR(Instruction const& instruction, Registers const& registers)
{
    assert(checkRegisterIndex(instruction.input2));
    assert(checkRegisterIndex(instruction.output));
    Registers ret = registers;
    ret[instruction.output] = (instruction.input1 == ret[instruction.input2]) ? 1 : 0;
    return ret;
}


Registers executeEQRI(Instruction const& instruction, Registers const& registers)
{
    assert(checkRegisterIndex(instruction.input1));
    assert(checkRegisterIndex(instruction.output));
    Registers ret = registers;
    ret[instruction.output] = (ret[instruction.input1] == instruction.input2) ? 1 : 0;
    return ret;
}


Registers executeEQRR(Instruction const& instruction, Registers const& registers)
{
    assert(checkRegisterIndex(instruction.input1));
    assert(checkRegisterIndex(instruction.input2));
    assert(checkRegisterIndex(instruction.output));
    Registers ret = registers;
    ret[instruction.output] = (ret[instruction.input1] == ret[instruction.input2]) ? 1 : 0;
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
        return checkRegisterIndex(ins.input1) && checkRegisterIndex(ins.input2) && checkRegisterIndex(ins.output);
    } else if(op == Opcode::addi) {
        return checkRegisterIndex(ins.input1) && checkRegisterIndex(ins.output);
    } else if(op == Opcode::mulr) {
        return checkRegisterIndex(ins.input1) && checkRegisterIndex(ins.input2) && checkRegisterIndex(ins.output);
    } else if(op == Opcode::muli) {
        return checkRegisterIndex(ins.input1) && checkRegisterIndex(ins.output);
    } else if(op == Opcode::banr) {
        return checkRegisterIndex(ins.input1) && checkRegisterIndex(ins.input2) && checkRegisterIndex(ins.output);
    } else if(op == Opcode::bani) {
        return checkRegisterIndex(ins.input1) && checkRegisterIndex(ins.output);
    } else if(op == Opcode::borr) {
        return checkRegisterIndex(ins.input1) && checkRegisterIndex(ins.input2) && checkRegisterIndex(ins.output);
    } else if(op == Opcode::bori) {
        return checkRegisterIndex(ins.input1) && checkRegisterIndex(ins.output);
    } else if(op == Opcode::setr) {
        return checkRegisterIndex(ins.input1) && checkRegisterIndex(ins.output);
    } else if(op == Opcode::seti) {
        return checkRegisterIndex(ins.output);
    } else if(op == Opcode::gtir) {
        return checkRegisterIndex(ins.input2) && checkRegisterIndex(ins.output);
    } else if(op == Opcode::gtri) {
        return checkRegisterIndex(ins.input1) && checkRegisterIndex(ins.output);
    } else if(op == Opcode::gtrr) {
        return checkRegisterIndex(ins.input1) && checkRegisterIndex(ins.input2) && checkRegisterIndex(ins.output);
    } else if(op == Opcode::eqir) {
        return checkRegisterIndex(ins.input2) && checkRegisterIndex(ins.output);
    } else if(op == Opcode::eqri) {
        return checkRegisterIndex(ins.input1) && checkRegisterIndex(ins.output);
    } else {
        assert(op == Opcode::eqrr);
        return checkRegisterIndex(ins.input1) && checkRegisterIndex(ins.input2) && checkRegisterIndex(ins.output);
    }
}

bool step(RegisterMachine& rm)
{
    Instruction const& instr = rm.program[rm.ip];
    rm.registers[rm.ip_bound_register] = rm.ip;
    Registers const result = getFunctionFor(instr.opcode)(instr, rm.registers);
    rm.registers = result;
    rm.ip = rm.registers[rm.ip_bound_register];
    ++rm.ip;
    return (rm.ip >= 0) && (rm.ip < static_cast<int>(rm.program.size()));
}

void runToCompletion(RegisterMachine& rm)
{
    while(step(rm)) ;
}
