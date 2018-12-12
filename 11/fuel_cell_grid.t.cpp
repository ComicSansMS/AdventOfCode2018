#include <fuel_cell_grid.hpp>

#include <catch.hpp>

TEST_CASE("Fuel Cell Grid")
{
    SECTION("Calculate Power Level")
    {
        CHECK(calculatePowerLevel(3, 5, 8) == 4);
        CHECK(calculatePowerLevel(122, 79, 57) == -5);
        CHECK(calculatePowerLevel(217, 196, 39) == 0);
        CHECK(calculatePowerLevel(101, 153, 71) == 4);
    }

    SECTION("Get Largest Square")
    {
        CHECK(getLargestSquare(18, 3) == std::make_tuple(29, 33, 45));
        CHECK(getLargestSquare(42, 3) == std::make_tuple(30, 21, 61));
    }

    SECTION("Get Largest of All Squares")
    {
        CHECK(getLargestOfAllSquares(18) == std::make_tuple(113, 90, 269, 16));
        //CHECK(getLargestOfAllSquares(42) == std::make_tuple(119, 232, 251, 12));
    }
}
