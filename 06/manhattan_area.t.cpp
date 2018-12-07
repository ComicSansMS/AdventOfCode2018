#include <manhattan_area.hpp>

#include <range/v3/view/slice.hpp>
#include <range/v3/view/transform.hpp>

#include <catch.hpp>

#include <sstream>

TEST_CASE("Manhattan Area")
{
    SECTION("Coordinate Equality")
    {
        CHECK(Coordinate{1, 2} == Coordinate{1, 2});
        CHECK_FALSE(Coordinate{1, 0} == Coordinate{1, 2});
        CHECK_FALSE(Coordinate{0, 2} == Coordinate{1, 2});
        CHECK_FALSE(Coordinate{0, 0} == Coordinate{1, 2});
    }

    SECTION("Rectangle")
    {
        Rectangle r(1, 2, 3, 4);
        CHECK(r.left == 1);
        CHECK(r.top == 2);
        CHECK(r.width == 3);
        CHECK(r.height == 4);
    }

    SECTION("Rectangle Equality")
    {
        CHECK(Rectangle{1,2,3,4} == Rectangle{1,2,3,4});
        CHECK_FALSE(Rectangle{1,2,3,4} == Rectangle{0,0,0,0});
        CHECK_FALSE(Rectangle{1,2,3,4} == Rectangle{0,2,3,4});
        CHECK_FALSE(Rectangle{1,2,3,4} == Rectangle{1,0,3,4});
        CHECK_FALSE(Rectangle{1,2,3,4} == Rectangle{1,2,0,4});
        CHECK_FALSE(Rectangle{1,2,3,4} == Rectangle{1,2,3,0});
    }

    SECTION("Rectangle Printing")
    {
        std::stringstream sstr;
        sstr << Rectangle{1, 2, 3, 4};
        CHECK(sstr.str() == "{ l: 1 t: 2 w: 3 h: 4 }");
    }

    char const* sample_input = "1, 1\n1, 6\n8, 3\n3, 4\n5, 5\n8, 9\n";

    SECTION("Parse Input")
    {
        auto const coordinates = parseInput(sample_input);
        CHECK(coordinates == std::vector<Coordinate>{ {1, 1}, {1, 6}, {8, 3}, {3, 4}, {5, 5}, {8, 9} });
    }

    SECTION("Field Dimension Calculation")
    {
        CHECK(getFieldDimensions(std::vector<Coordinate>{ {1, 1} }) == Rectangle{0, 0, 3, 3});
        CHECK(getFieldDimensions(std::vector<Coordinate>{ {1, 1}, {4, 5} }) == Rectangle{0, 0, 6, 7});
        CHECK(getFieldDimensions(std::vector<Coordinate>{ {3, 7}, {4, 5} }) == Rectangle{2, 4, 4, 5});
        CHECK(getFieldDimensions(parseInput(sample_input)) == Rectangle{0, 0, 10, 11});
    }

    SECTION("Manhattan Distance")
    {
        CHECK(manhattanDistance(Coordinate{5, 3}, Coordinate{5, 3}) == 0);
        CHECK(manhattanDistance(Coordinate{0, 0}, Coordinate{10, 7}) == 17);
        CHECK(manhattanDistance(Coordinate{5, 3}, Coordinate{10, 7}) == 9);
        CHECK(manhattanDistance(Coordinate{5, 3}, Coordinate{-4, 4}) == 10);
    }

    SECTION("Iterate Rectangle")
    {
        auto const coordinates = parseInput(sample_input);
        auto const field = fillField(coordinates);
        REQUIRE(field.size() == 12*11);
        auto row_point = [&](int idx) {
            auto pointIndexToChar = [](int i) -> char { return (i == -1) ? '.' : static_cast<char>('a' + i); };
            auto rng = field | ranges::view::slice(idx*11, ((idx+1)*11)) |
                       ranges::view::transform(&FieldCell::closestPoint) |
                       ranges::view::transform(pointIndexToChar);
            return rng;
        };
        CHECK(std::string(row_point(0))  == "aaaaa.ccccc");
        CHECK(std::string(row_point(1))  == "aaaaa.ccccc");
        CHECK(std::string(row_point(2))  == "aaaddeccccc");
        CHECK(std::string(row_point(3))  == "aadddeccccc");
        CHECK(std::string(row_point(4))  == "..dddeecccc");
        CHECK(std::string(row_point(5))  == "bb.deeeeccc");
        CHECK(std::string(row_point(6))  == "bbb.eeee...");
        CHECK(std::string(row_point(7))  == "bbb.eeeffff");
        CHECK(std::string(row_point(8))  == "bbb.eefffff");
        CHECK(std::string(row_point(9))  == "bbb.fffffff");
        CHECK(std::string(row_point(10)) == "bbb.fffffff");

        auto row_distance = [&](int idx) {
            auto pointDistanceToChar = [](int i) -> char { return (i == -1) ? '.' : static_cast<char>('0' + i); };
            auto rng = field | ranges::view::slice(idx*11, ((idx+1)*11)) |
                       ranges::view::transform(&FieldCell::distance) |
                       ranges::view::transform(pointDistanceToChar);
            return rng;
        };
        CHECK(std::string(row_distance(0))  == "21234554345");
        CHECK(std::string(row_distance(1))  == "10123443234");
        CHECK(std::string(row_distance(2))  == "21223332123");
        CHECK(std::string(row_distance(3))  == "32212221012");
        CHECK(std::string(row_distance(4))  == "32101122123");
        CHECK(std::string(row_distance(5))  == "21211012234");
        CHECK(std::string(row_distance(6))  == "10122123345");
        CHECK(std::string(row_distance(7))  == "21233233234");
        CHECK(std::string(row_distance(8))  == "32344332123");
        CHECK(std::string(row_distance(9))  == "43454321012");
        CHECK(std::string(row_distance(10)) == "54565432123");
    }

    SECTION("Find Largest Area")
    {
        auto const coordinates = parseInput(sample_input);
        auto const field = fillField(coordinates);
        CHECK(findLargestArea(field, coordinates) == 17);
    }

    SECTION("Find Largest Area Flood Fill")
    {
        auto const coordinates = parseInput(sample_input);
        CHECK(findLargestAreaFloodFill_SinglePoint(coordinates, 32, coordinates[3]) == 16);
        CHECK(findLargestAreaFloodFill_SinglePoint(coordinates, 32, coordinates[0]) == 1);

        CHECK(findLargestAreaFloodFill(coordinates, 32) == 16);
    }
}
