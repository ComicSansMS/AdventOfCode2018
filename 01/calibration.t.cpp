#include <catch.hpp>

#include <calibration.hpp>

TEST_CASE("Frequency Calibration")
{
    char const* sample_input_01 = "+1\n-2\n+3\n+1\n";
    char const* sample_input_02 = "+1\n+1\n+1\n";
    char const* sample_input_03 = "+1\n+1\n-2\n";
    char const* sample_input_04 = "-1\n-2\n-3\n";

    SECTION("Parse Input")
    {
        CHECK(parseInput(sample_input_01) == std::vector<int>{1, -2, 3, 1});
        CHECK(parseInput(sample_input_02) == std::vector<int>{1, 1, 1});
        CHECK(parseInput(sample_input_03) == std::vector<int>{1, 1, -2});
        CHECK(parseInput(sample_input_04) == std::vector<int>{-1, -2, -3});
    }

    SECTION("Calculate Result Frequency")
    {
        CHECK(calculateResultFrequency({0}) == 0);
        CHECK(calculateResultFrequency({1}) == 1);
        CHECK(calculateResultFrequency({1, -2}) == -1);
        CHECK(calculateResultFrequency({1, -2, 3}) == 2);
        CHECK(calculateResultFrequency(parseInput(sample_input_01)) == 3);
        CHECK(calculateResultFrequency(parseInput(sample_input_02)) == 3);
        CHECK(calculateResultFrequency(parseInput(sample_input_03)) == 0);
        CHECK(calculateResultFrequency(parseInput(sample_input_04)) == -6);
    }

    SECTION("Find Recurring Frequency")
    {
        CHECK(findRecurringFrequency({1, -1}) == 0);
        CHECK(findRecurringFrequency(parseInput(sample_input_01)) == 2);
        CHECK(findRecurringFrequency({3, 3, 4, -2, -4}) == 10);
        CHECK(findRecurringFrequency({-6, 3, 8, 5, -6}) == 5);
        CHECK(findRecurringFrequency({7, 7, -2, -7, -4}) == 14);
    }
}
