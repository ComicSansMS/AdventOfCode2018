#include <hot_chocolate.hpp>

#include <catch.hpp>

TEST_CASE("Hot Chocolate")
{

    SECTION("Parse Input")
    {
        CHECK(parseInput("0123456789") == std::vector<int>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
    }

    SECTION("Recipe Combiner Construction")
    {
        RecipeCombiner rc;
        CHECK(rc.scores == std::vector<int>{3, 7});
        CHECK(rc.elves == std::vector<size_t>{0, 1});
    }

    SECTION("Combine Recipes")
    {
        RecipeCombiner rc;

        // (3)[7] 1  0
        rc.combine();
        CHECK(rc.scores == std::vector<int>{3, 7, 1, 0});
        CHECK(rc.elves == std::vector<size_t>{0, 1});

        // 3  7  1 [0](1) 0
        rc.combine();
        CHECK(rc.scores == std::vector<int>{3, 7, 1, 0, 1, 0});
        CHECK(rc.elves == std::vector<size_t>{4, 3});

        // 3  7  1  0 [1] 0 (1)
        rc.combine();
        CHECK(rc.scores == std::vector<int>{3, 7, 1, 0, 1, 0, 1});
        CHECK(rc.elves == std::vector<size_t>{6, 4});

        for(int i=0; i<10; ++i) { rc.combine(); }

        // 3  7  1  0 (1) 0  1  2  4  5  1  5 [8] 9  1  6  7  7
        CHECK(rc.scores == std::vector<int>{3, 7, 1, 0, 1, 0, 1, 2, 4, 5, 1, 5, 8, 9, 1, 6, 7, 7});
        CHECK(rc.elves == std::vector<size_t>{4, 12});

        // 3  7 [1] 0  1  0 (1) 2  4  5  1  5  8  9  1  6  7  7  9
        rc.combine();
        CHECK(rc.scores == std::vector<int>{3, 7, 1, 0, 1, 0, 1, 2, 4, 5, 1, 5, 8, 9, 1, 6, 7, 7, 9});
        CHECK(rc.elves == std::vector<size_t>{6, 2});

        // 3  7  1  0 [1] 0  1  2 (4) 5  1  5  8  9  1  6  7  7  9  2
        rc.combine();
        CHECK(rc.scores == std::vector<int>{3, 7, 1, 0, 1, 0, 1, 2, 4, 5, 1, 5, 8, 9, 1, 6, 7, 7, 9, 2});
        CHECK(rc.elves == std::vector<size_t>{8, 4});
    }

    SECTION("Cook Until")
    {
        CHECK(RecipeCombiner().cook_until(9) == "5158916779");
        CHECK(RecipeCombiner().cook_until(5) == "0124515891");
        CHECK(RecipeCombiner().cook_until(18) == "9251071085");
        CHECK(RecipeCombiner().cook_until(2018) == "5941429882");
    }

    SECTION("Cook Until2")
    {
        CHECK(RecipeCombiner().cook_until2({5,1,5,8,9}) == 9);
        CHECK(RecipeCombiner().cook_until2({0,1,2,4,5}) == 5);
        CHECK(RecipeCombiner().cook_until2({9,2,5,1,0}) == 18);
        CHECK(RecipeCombiner().cook_until2({5,9,4,1,4}) == 2018);
    }
}
