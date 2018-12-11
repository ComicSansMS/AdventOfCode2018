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
        CHECK(getLargestSquare(18) == std::make_tuple(33, 45));
        CHECK(getLargestSquare(42) == std::make_tuple(21, 61));
    }
}
