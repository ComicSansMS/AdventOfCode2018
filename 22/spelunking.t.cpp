#include <spelunking.hpp>

#include <catch.hpp>

#include <sstream>

TEST_CASE("Spelunking")
{
    char const sample_input[] = "depth: 510\ntarget: 10,10\n";

    SECTION("Parse Input")
    {
        auto const scan = parseInput(sample_input);
        CHECK(scan.depth == 510);
        CHECK(scan.target_coords.x == 10);
        CHECK(scan.target_coords.y == 10);
    }

    SECTION("Cave Generation")
    {
        auto const scan = parseInput(sample_input);
        Cave c(scan, 16, 16);
        CHECK(c.width == 16);
        CHECK(c.height == 16);
        REQUIRE(c.regions.size() == 16*16);

        std::stringstream sstr;
        sstr << c;
        CHECK(sstr.str() == "M=.|=.|.|=.|=|=." "\n"
                            ".|=|=|||..|.=..." "\n"
                            ".==|....||=..|==" "\n"
                            "=.|....|.==.|==." "\n"
                            "=|..==...=.|==.." "\n"
                            "=||.=.=||=|=..|=" "\n"
                            "|.=.===|||..=..|" "\n"
                            "|..==||=.|==|===" "\n"
                            ".=..===..=|.|||." "\n"
                            ".======|||=|=.|=" "\n"
                            ".===|=|===T===||" "\n"
                            "=|||...|==..|=.|" "\n"
                            "=.=|=.=..=.||==|" "\n"
                            "||=|=...|==.=|==" "\n"
                            "|=.=||===.|||===" "\n"
                            "||.|==.|.|.||=||" "\n");
    }

    SECTION("Risk Level Calculation")
    {
        auto const scan = parseInput(sample_input);
        Cave c(scan, 16, 16);

        CHECK(c.riskLevelToTarget() == 114);
    }
}
