#include <plant_spreading.hpp>

#include <catch.hpp>

TEST_CASE("Plant Spreading")
{
    char const* sample_input = R"(initial state: #..#.#..##......###...###

...## => #
..#.. => #
.#... => #
.#.#. => #
.#.## => #
.##.. => #
.#### => #
#.#.# => #
#.### => #
##.#. => #
##.## => #
###.. => #
###.# => #
####. => #
)";

    SECTION("Input Parsing")
    {
        auto const plants = Plants::parseFromInput(sample_input);

        CHECK(plants.potHasPlant(0));
        CHECK(!plants.potHasPlant(1));
        CHECK(!plants.potHasPlant(2));
        CHECK(plants.potHasPlant(3));
        CHECK(!plants.potHasPlant(4));
        CHECK(plants.potHasPlant(5));
        CHECK(!plants.potHasPlant(6));
        CHECK(!plants.potHasPlant(7));

        CHECK(!plants.potHasPlant(20));
        CHECK(!plants.potHasPlant(21));
        CHECK(plants.potHasPlant(22));
        CHECK(plants.potHasPlant(23));
        CHECK(plants.potHasPlant(24));
        CHECK(!plants.potHasPlant(25));

        CHECK(plants.countPlants() == 11);

        auto const rules = plants.getRules();
        REQUIRE(rules.size() == 14);
        CHECK(rules[0].before == std::array<bool, 5>{false, false, false, true, true});
        CHECK(rules[0].after == true);

        CHECK(rules[1].before == std::array<bool, 5>{false, false, true, false, false});
        CHECK(rules[1].after == true);

        CHECK(rules[2].before == std::array<bool, 5>{false, true, false, false, false});
        CHECK(rules[2].after == true);

        CHECK(rules[12].before == std::array<bool, 5>{true, true, true, false, true});
        CHECK(rules[12].after == true);

        CHECK(rules[13].before == std::array<bool, 5>{true, true, true, true, false});
        CHECK(rules[13].after == true);
    }

    SECTION("Simulate step")
    {
        auto plants = Plants::parseFromInput(sample_input);
        plants.simulateStep();

        CHECK(plants.countPlants() == 7);
        std::array<bool, 50> pots;
        int i = -10;
        std::generate(begin(pots), end(pots), [&plants, &i]() { return plants.potHasPlant(i++); });
        CHECK(pots == std::array<bool, 50>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
             1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0,
             0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        });
         
        for(int i=0; i<19; ++i) { plants.simulateStep(); }
        CHECK(plants.countPlants() == 19);
        CHECK(plants.accumulate() == 325);
    }
}
