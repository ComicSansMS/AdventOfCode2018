#include <hot_chocolate.hpp>

#include <catch.hpp>

TEST_CASE("Hot Chocolate")
{
    char const* sample_input = "37";

    SECTION("Parse Input")
    {
        CHECK(parseInput("0123456789") == std::vector<int>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
        CHECK(parseInput(sample_input) == std::vector<int>{3, 7});
    }

    SECTION("Recipe Combiner Construction")
    {
        auto const initial_scores = parseInput(sample_input);
        RecipeCombiner rc(initial_scores);
        CHECK(rc.scores == initial_scores);
        CHECK(rc.elves == std::vector<int>{0, 1});
    }

    SECTION("Combine Recipes")
    {
        RecipeCombiner rc(parseInput(sample_input));

        // (3)[7] 1  0
        rc.combine();
        CHECK(rc.scores == std::vector<int>{3, 7, 1, 0});
        CHECK(rc.elves == std::vector<int>{0, 1});

        // 3  7  1 [0](1) 0
        rc.combine();
        CHECK(rc.scores == std::vector<int>{3, 7, 1, 0, 1, 0});
        CHECK(rc.elves == std::vector<int>{4, 3});

        // 3  7  1  0 [1] 0 (1)
        rc.combine();
        CHECK(rc.scores == std::vector<int>{3, 7, 1, 0, 1, 0, 1});
        CHECK(rc.elves == std::vector<int>{6, 4});

        for(int i=0; i<10; ++i) { rc.combine(); }

        // 3  7  1  0 (1) 0  1  2  4  5  1  5 [8] 9  1  6  7  7
        CHECK(rc.scores == std::vector<int>{3, 7, 1, 0, 1, 0, 1, 2, 4, 5, 1, 5, 8, 9, 1, 6, 7, 7});
        CHECK(rc.elves == std::vector<int>{4, 12});

        // 3  7 [1] 0  1  0 (1) 2  4  5  1  5  8  9  1  6  7  7  9
        rc.combine();
        CHECK(rc.scores == std::vector<int>{3, 7, 1, 0, 1, 0, 1, 2, 4, 5, 1, 5, 8, 9, 1, 6, 7, 7, 9});
        CHECK(rc.elves == std::vector<int>{6, 2});

        // 3  7  1  0 [1] 0  1  2 (4) 5  1  5  8  9  1  6  7  7  9  2
        rc.combine();
        CHECK(rc.scores == std::vector<int>{3, 7, 1, 0, 1, 0, 1, 2, 4, 5, 1, 5, 8, 9, 1, 6, 7, 7, 9, 2});
        CHECK(rc.elves == std::vector<int>{8, 4});
    }

    SECTION("Cook Until")
    {
        RecipeCombiner rc(parseInput(sample_input));

        CHECK(RecipeCombiner(rc).cook_until(9) == "5158916779");
        CHECK(RecipeCombiner(rc).cook_until(5) == "0124515891");
        CHECK(RecipeCombiner(rc).cook_until(18) == "9251071085");
        CHECK(RecipeCombiner(rc).cook_until(2018) == "5941429882");
    }
}
