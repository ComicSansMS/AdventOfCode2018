#include <register_machine_ip.hpp>

#include <catch.hpp>

#include <sstream>

TEST_CASE("Game of Woods")
{
    char const sample_input[] = "#ip 0"      "\n"
                                "seti 5 0 1" "\n"
                                "seti 6 0 2" "\n"
                                "addi 0 1 0" "\n"
                                "addr 1 2 3" "\n"
                                "setr 1 0 0" "\n"
                                "seti 8 0 4" "\n"
                                "seti 9 0 5" "\n";

    SECTION("Parse Input")
    {
        auto rm = parseInput(sample_input);

        CHECK(rm.ip_bound_register == 0);
        CHECK(rm.ip == 0);
        CHECK(rm.registers == Registers{0, 0, 0, 0, 0, 0});
        REQUIRE(rm.program.size() == 7);
        CHECK(rm.program[0] == Instruction{Opcode::seti, 5, 0, 1});
        CHECK(rm.program[1] == Instruction{Opcode::seti, 6, 0, 2});
        CHECK(rm.program[2] == Instruction{Opcode::addi, 0, 1, 0});
        CHECK(rm.program[3] == Instruction{Opcode::addr, 1, 2, 3});
        CHECK(rm.program[4] == Instruction{Opcode::setr, 1, 0, 0});
        CHECK(rm.program[5] == Instruction{Opcode::seti, 8, 0, 4});
        CHECK(rm.program[6] == Instruction{Opcode::seti, 9, 0, 5});
    }

    SECTION("Parse Opcode")
    {
        CHECK(parseOpcode("addr") == Opcode::addr);
        CHECK(parseOpcode("addi") == Opcode::addi);
        CHECK(parseOpcode("mulr") == Opcode::mulr);
        CHECK(parseOpcode("muli") == Opcode::muli);
        CHECK(parseOpcode("banr") == Opcode::banr);
        CHECK(parseOpcode("bani") == Opcode::bani);
        CHECK(parseOpcode("borr") == Opcode::borr);
        CHECK(parseOpcode("bori") == Opcode::bori);
        CHECK(parseOpcode("setr") == Opcode::setr);
        CHECK(parseOpcode("seti") == Opcode::seti);
        CHECK(parseOpcode("gtir") == Opcode::gtir);
        CHECK(parseOpcode("gtri") == Opcode::gtri);
        CHECK(parseOpcode("gtrr") == Opcode::gtrr);
        CHECK(parseOpcode("eqir") == Opcode::eqir);
        CHECK(parseOpcode("eqri") == Opcode::eqri);
        CHECK(parseOpcode("eqrr") == Opcode::eqrr);
    }

    SECTION("AddR")
    {
        // addr (add register) stores into register C the result of adding register A and register B.
        CHECK(executeAddR(Instruction{Opcode::addr, 2, 3, 1}, Registers{5, 6, 7, 8}) == Registers{5, 15, 7, 8});
    }

    SECTION("AddI")
    {
        // addi (add immediate) stores into register C the result of adding register A and value B.
        CHECK(executeAddI(Instruction{Opcode::addi, 2, 3, 1}, Registers{5, 6, 7, 8}) == Registers{5, 10, 7, 8});
    }

    SECTION("MulR")
    {
        // mulr (multiply register) stores into register C the result of multiplying register A and register B.
        CHECK(executeMulR(Instruction{Opcode::mulr, 2, 3, 1}, Registers{5, 6, 7, 8}) == Registers{5, 56, 7, 8});
    }

    SECTION("MulI")
    {
        // muli (multiply immediate) stores into register C the result of multiplying register A and value B.
        CHECK(executeMulI(Instruction{Opcode::muli, 2, 3, 1}, Registers{5, 6, 7, 8}) == Registers{5, 21, 7, 8});
    }

    SECTION("BAnR")
    {
        // banr (bitwise AND register) stores into register C the result of the bitwise AND of
        // register A and register B.
        CHECK(executeBAnR(Instruction{Opcode::banr, 2, 3, 1}, Registers{5, 6, 7, 4}) == Registers{5, 4, 7, 4});
    }

    SECTION("BAnI")
    {
        // bani (bitwise AND immediate) stores into register C the result of the bitwise AND of register A and value B.
        CHECK(executeBAnI(Instruction{Opcode::bani, 2, 3, 1}, Registers{5, 6, 7, 8}) == Registers{5, 3, 7, 8});
    }

    SECTION("BOrR")
    {
        // borr (bitwise OR register) stores into register C the result of the bitwise OR of register A and register B.
        CHECK(executeBOrR(Instruction{Opcode::borr, 2, 3, 1}, Registers{5, 6, 7, 8}) == Registers{5, 15, 7, 8});
    }

    SECTION("BOrI")
    {
        // bori (bitwise OR immediate) stores into register C the result of the bitwise OR of register A and value B.
        CHECK(executeBOrI(Instruction{Opcode::bori, 2, 3, 1}, Registers{5, 6, 7, 8}) == Registers{5, 7, 7, 8});
    }

    SECTION("SetR")
    {
        // setr (set register) copies the contents of register A into register C. (Input B is ignored.)
        CHECK(executeSetR(Instruction{Opcode::setr, 2, 99, 1}, Registers{5, 6, 42, 8}) == Registers{5, 42, 42, 8});
    }

    SECTION("SetI")
    {
        // seti (set immediate) stores value A into register C. (Input B is ignored.)
        CHECK(executeSetI(Instruction{Opcode::seti, 42, 99, 1}, Registers{5, 6, 7, 8}) == Registers{5, 42, 7, 8});
    }

    SECTION("GTIR")
    {
        // gtir (greater-than immediate/register) sets register C to 1 if value A is greater than register B.
        // Otherwise, register C is set to 0.
        CHECK(executeGTIR(Instruction{Opcode::gtir, 8, 2, 1}, Registers{5, 6, 7, 8}) == Registers{5, 1, 7, 8});
        CHECK(executeGTIR(Instruction{Opcode::gtir, 7, 2, 1}, Registers{5, 6, 7, 8}) == Registers{5, 0, 7, 8});
    }

    SECTION("GTRI")
    {
        // gtri (greater-than register/immediate) sets register C to 1 if register A is greater than value B.
        // Otherwise, register C is set to 0.
        CHECK(executeGTRI(Instruction{Opcode::gtri, 2, 8, 1}, Registers{5, 6, 7, 8}) == Registers{5, 0, 7, 8});
        CHECK(executeGTRI(Instruction{Opcode::gtri, 2, 6, 1}, Registers{5, 6, 7, 8}) == Registers{5, 1, 7, 8});
    }

    SECTION("GTRR")
    {
        // gtrr (greater-than register/register) sets register C to 1 if register A is greater than register B.
        // Otherwise, register C is set to 0.
        CHECK(executeGTRR(Instruction{Opcode::gtrr, 2, 3, 1}, Registers{5, 6, 7, 8}) == Registers{5, 0, 7, 8});
        CHECK(executeGTRR(Instruction{Opcode::gtrr, 3, 2, 1}, Registers{5, 6, 7, 8}) == Registers{5, 1, 7, 8});
    }

    SECTION("EQIR")
    {
        // eqir (equal immediate/register) sets register C to 1 if value A is equal to register B.
        // Otherwise, register C is set to 0.
        CHECK(executeEQIR(Instruction{Opcode::eqir, 8, 2, 1}, Registers{5, 6, 7, 8}) == Registers{5, 0, 7, 8});
        CHECK(executeEQIR(Instruction{Opcode::eqir, 7, 2, 1}, Registers{5, 6, 7, 8}) == Registers{5, 1, 7, 8});
    }

    SECTION("EQRI")
    {
        // eqri (equal register/immediate) sets register C to 1 if register A is equal to value B.
        // Otherwise, register C is set to 0.
        CHECK(executeEQRI(Instruction{Opcode::eqri, 2, 8, 1}, Registers{5, 6, 7, 8}) == Registers{5, 0, 7, 8});
        CHECK(executeEQRI(Instruction{Opcode::eqri, 2, 7, 1}, Registers{5, 6, 7, 8}) == Registers{5, 1, 7, 8});
    }

    SECTION("EQRR")
    {
        // eqrr (equal register/register) sets register C to 1 if register A is equal to register B.
        // Otherwise, register C is set to 0.
        CHECK(executeEQRR(Instruction{Opcode::eqrr, 2, 3, 1}, Registers{5, 6, 7, 8}) == Registers{5, 0, 7, 8});
        CHECK(executeEQRR(Instruction{Opcode::eqrr, 2, 2, 1}, Registers{5, 6, 7, 8}) == Registers{5, 1, 7, 8});
    }

    SECTION("Execution Step")
    {
        auto rm = parseInput(sample_input);

        CHECK(step(rm));
        CHECK(rm.ip == 1);
        CHECK(rm.registers == Registers{0, 5, 0, 0, 0, 0});

        CHECK(step(rm));
        CHECK(rm.ip == 2);
        CHECK(rm.registers == Registers{1, 5, 6, 0, 0, 0});

        CHECK(step(rm));
        CHECK(rm.ip == 4);
        CHECK(rm.registers == Registers{3, 5, 6, 0, 0, 0});

        CHECK(step(rm));
        CHECK(rm.ip == 6);
        CHECK(rm.registers == Registers{5, 5, 6, 0, 0, 0});

        CHECK(!step(rm));
        CHECK(rm.ip == 7);
        CHECK(rm.registers == Registers{6, 5, 6, 0, 0, 9});
    }

    SECTION("Run To Completion")
    {
        auto rm = parseInput(sample_input);

        runToCompletion(rm);
        CHECK(rm.ip == 7);
        CHECK(rm.registers == Registers{6, 5, 6, 0, 0, 9});
    }
}
