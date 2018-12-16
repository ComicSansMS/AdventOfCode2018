#include <assembly_reverse.hpp>

#include <catch.hpp>

TEST_CASE("Assembly Reverse")
{
    char const sample_input[] = "Before: [3, 2, 1, 1]" "\n"
                                "9 2 1 2" "\n"
                                "After:  [3, 2, 2, 1]" "\n"
                                "\n\n\n"
                                "1 2 3 4" "\n";
    SECTION("Parse Input Samples")
    {
        auto const program = parseInput(sample_input);
        REQUIRE(program.samples.size() == 1);
        CHECK(program.samples[0].registers_before == Registers{3, 2, 1, 1});
        CHECK(program.samples[0].instruction.Opcode == 9);
        CHECK(program.samples[0].instruction.Input1 == 2);
        CHECK(program.samples[0].instruction.Input2 == 1);
        CHECK(program.samples[0].instruction.Output == 2);
        CHECK(program.samples[0].registers_after == Registers{3, 2, 2, 1});
        REQUIRE(program.program.size() == 1);
        REQUIRE(program.program[0].Opcode == 1);
        REQUIRE(program.program[0].Input1 == 2);
        REQUIRE(program.program[0].Input2 == 3);
        REQUIRE(program.program[0].Output == 4);
    }

    SECTION("AddR")
    {
        // addr (add register) stores into register C the result of adding register A and register B.
        CHECK(executeAddR(Instruction{99, 2, 3, 1}, Registers{5, 6, 7, 8}) == Registers{5, 15, 7, 8});
    }

    SECTION("AddI")
    {
        // addi (add immediate) stores into register C the result of adding register A and value B.
        CHECK(executeAddI(Instruction{99, 2, 3, 1}, Registers{5, 6, 7, 8}) == Registers{5, 10, 7, 8});
    }

    SECTION("MulR")
    {
        // mulr (multiply register) stores into register C the result of multiplying register A and register B.
        CHECK(executeMulR(Instruction{99, 2, 3, 1}, Registers{5, 6, 7, 8}) == Registers{5, 56, 7, 8});
    }

    SECTION("MulI")
    {
        // muli (multiply immediate) stores into register C the result of multiplying register A and value B.
        CHECK(executeMulI(Instruction{99, 2, 3, 1}, Registers{5, 6, 7, 8}) == Registers{5, 21, 7, 8});
    }

    SECTION("BAnR")
    {
        // banr (bitwise AND register) stores into register C the result of the bitwise AND of
        // register A and register B.
        CHECK(executeBAnR(Instruction{99, 2, 3, 1}, Registers{5, 6, 7, 4}) == Registers{5, 4, 7, 4});
    }

    SECTION("BAnI")
    {
        // bani (bitwise AND immediate) stores into register C the result of the bitwise AND of register A and value B.
        CHECK(executeBAnI(Instruction{99, 2, 3, 1}, Registers{5, 6, 7, 8}) == Registers{5, 3, 7, 8});
    }

    SECTION("BOrR")
    {
        // borr (bitwise OR register) stores into register C the result of the bitwise OR of register A and register B.
        CHECK(executeBOrR(Instruction{99, 2, 3, 1}, Registers{5, 6, 7, 8}) == Registers{5, 15, 7, 8});
    }

    SECTION("BOrI")
    {
        // bori (bitwise OR immediate) stores into register C the result of the bitwise OR of register A and value B.
        CHECK(executeBOrI(Instruction{99, 2, 3, 1}, Registers{5, 6, 7, 8}) == Registers{5, 7, 7, 8});
    }

    SECTION("SetR")
    {
        // setr (set register) copies the contents of register A into register C. (Input B is ignored.)
        CHECK(executeSetR(Instruction{99, 2, 99, 1}, Registers{5, 6, 42, 8}) == Registers{5, 42, 42, 8});
    }

    SECTION("SetI")
    {
        // seti (set immediate) stores value A into register C. (Input B is ignored.)
        CHECK(executeSetI(Instruction{99, 42, 99, 1}, Registers{5, 6, 7, 8}) == Registers{5, 42, 7, 8});
    }

    SECTION("GTIR")
    {
        // gtir (greater-than immediate/register) sets register C to 1 if value A is greater than register B.
        // Otherwise, register C is set to 0.
        CHECK(executeGTIR(Instruction{99, 8, 2, 1}, Registers{5, 6, 7, 8}) == Registers{5, 1, 7, 8});
        CHECK(executeGTIR(Instruction{99, 7, 2, 1}, Registers{5, 6, 7, 8}) == Registers{5, 0, 7, 8});
    }

    SECTION("GTRI")
    {
        // gtri (greater-than register/immediate) sets register C to 1 if register A is greater than value B.
        // Otherwise, register C is set to 0.
        CHECK(executeGTRI(Instruction{99, 2, 8, 1}, Registers{5, 6, 7, 8}) == Registers{5, 0, 7, 8});
        CHECK(executeGTRI(Instruction{99, 2, 6, 1}, Registers{5, 6, 7, 8}) == Registers{5, 1, 7, 8});
    }

    SECTION("GTRR")
    {
        // gtrr (greater-than register/register) sets register C to 1 if register A is greater than register B.
        // Otherwise, register C is set to 0.
        CHECK(executeGTRR(Instruction{99, 2, 3, 1}, Registers{5, 6, 7, 8}) == Registers{5, 0, 7, 8});
        CHECK(executeGTRR(Instruction{99, 3, 2, 1}, Registers{5, 6, 7, 8}) == Registers{5, 1, 7, 8});
    }

    SECTION("EQIR")
    {
        // eqir (equal immediate/register) sets register C to 1 if value A is equal to register B.
        // Otherwise, register C is set to 0.
        CHECK(executeEQIR(Instruction{99, 8, 2, 1}, Registers{5, 6, 7, 8}) == Registers{5, 0, 7, 8});
        CHECK(executeEQIR(Instruction{99, 7, 2, 1}, Registers{5, 6, 7, 8}) == Registers{5, 1, 7, 8});
    }

    SECTION("EQRI")
    {
        // eqri (equal register/immediate) sets register C to 1 if register A is equal to value B.
        // Otherwise, register C is set to 0.
        CHECK(executeEQRI(Instruction{99, 2, 8, 1}, Registers{5, 6, 7, 8}) == Registers{5, 0, 7, 8});
        CHECK(executeEQRI(Instruction{99, 2, 7, 1}, Registers{5, 6, 7, 8}) == Registers{5, 1, 7, 8});
    }

    SECTION("EQRR")
    {
        // eqrr (equal register/register) sets register C to 1 if register A is equal to register B.
        // Otherwise, register C is set to 0.
        CHECK(executeEQRR(Instruction{99, 2, 3, 1}, Registers{5, 6, 7, 8}) == Registers{5, 0, 7, 8});
        CHECK(executeEQRR(Instruction{99, 2, 2, 1}, Registers{5, 6, 7, 8}) == Registers{5, 1, 7, 8});
    }

    SECTION("Guessing Game")
    {
        auto const program = parseInput(sample_input);
        auto const sample = program.samples[0];
        CHECK(executeMulR(sample.instruction, sample.registers_before) == sample.registers_after);
        CHECK(executeAddI(sample.instruction, sample.registers_before) == sample.registers_after);
        CHECK(executeSetI(sample.instruction, sample.registers_before) == sample.registers_after);
        CHECK_FALSE(executeAddR(sample.instruction, sample.registers_before) == sample.registers_after);

        CHECK(guessingGame(sample).size() == 3);
    }

    SECTION("Count Samples 3 or More")
    {
        auto const program = parseInput(sample_input);
        CHECK(countSamples3OrMore(program) == 1);
    }
}
