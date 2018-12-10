#include <moving_stars.hpp>

#include <sstream>

#include <catch.hpp>

TEST_CASE("Moving Stars")
{
    SECTION("Vec 2")
    {
        Vec2 v(22, 33);
        CHECK(v.x == 22);
        CHECK(v.y == 33);

        CHECK(v == Vec2(22, 33));
        CHECK_FALSE(v == Vec2(22, 44));
        CHECK_FALSE(v == Vec2(11, 33));
        CHECK_FALSE(v == Vec2(44, 11));

        CHECK(std::hash<Vec2>{}(v) == 0x160021);
    }

    SECTION("Star")
    {
        CHECK(Star{ Vec2(1, 2), Vec2(3, 4) } == Star{ Vec2(1, 2), Vec2(3, 4) });
        CHECK_FALSE(Star{ Vec2(1, 2), Vec2(3, 4) } == Star{ Vec2(2, 1), Vec2(3, 4) });
        CHECK_FALSE(Star{ Vec2(1, 2), Vec2(3, 4) } == Star{ Vec2(1, 2), Vec2(4, 3) });
        CHECK_FALSE(Star{ Vec2(1, 2), Vec2(3, 4) } == Star{ Vec2(0, 0), Vec2(0, 0) });
    }


    char const* sample_input = R"(position=< 9,  1> velocity=< 0,  2>
position=< 7,  0> velocity=<-1,  0>
position=< 3, -2> velocity=<-1,  1>
position=< 6, 10> velocity=<-2, -1>
position=< 2, -4> velocity=< 2,  2>
position=<-6, 10> velocity=< 2, -2>
position=< 1,  8> velocity=< 1, -1>
position=< 1,  7> velocity=< 1,  0>
position=<-3, 11> velocity=< 1, -2>
position=< 7,  6> velocity=<-1, -1>
position=<-2,  3> velocity=< 1,  0>
position=<-4,  3> velocity=< 2,  0>
position=<10, -3> velocity=<-1,  1>
position=< 5, 11> velocity=< 1, -2>
position=< 4,  7> velocity=< 0, -1>
position=< 8, -2> velocity=< 0,  1>
position=<15,  0> velocity=<-2,  0>
position=< 1,  6> velocity=< 1,  0>
position=< 8,  9> velocity=< 0, -1>
position=< 3,  3> velocity=<-1,  1>
position=< 0,  5> velocity=< 0, -1>
position=<-2,  2> velocity=< 2,  0>
position=< 5, -2> velocity=< 1,  2>
position=< 1,  4> velocity=< 2,  1>
position=<-2,  7> velocity=< 2, -2>
position=< 3,  6> velocity=<-1, -1>
position=< 5,  0> velocity=< 1,  0>
position=<-6,  0> velocity=< 2,  0>
position=< 5,  9> velocity=< 1, -2>
position=<14,  7> velocity=<-2,  0>
position=<-3,  6> velocity=< 2, -1>
)";

    SECTION("Parse Input")
    {
        auto stars = parseInput(sample_input);
        REQUIRE(stars.size() == 31);
        stars.erase(stars.begin() + 15, stars.end());
        CHECK(stars == std::vector<Star>{
            Star{ Vec2(9, 1), Vec2(0, 2) },
            Star{ Vec2(7, 0), Vec2(-1, 0) },
            Star{ Vec2(3, -2), Vec2(-1, 1) },
            Star{ Vec2(6, 10), Vec2(-2, -1) },
            Star{ Vec2(2, -4), Vec2(2, 2) },
            Star{ Vec2(-6, 10), Vec2(2, -2) },
            Star{ Vec2(1, 8), Vec2(1, -1) },
            Star{ Vec2(1, 7), Vec2(1, 0) },
            Star{ Vec2(-3, 11), Vec2(1, -2) },
            Star{ Vec2(7, 6), Vec2(-1, -1) },
            Star{ Vec2(-2, 3), Vec2(1, 0) },
            Star{ Vec2(-4, 3), Vec2(2, 0) },
            Star{ Vec2(10, -3), Vec2(-1, 1) },
            Star{ Vec2(5, 11), Vec2(1, -2) },
            Star{ Vec2(4, 7), Vec2(0, -1) },
        });
    }

    SECTION("Simulate Step")
    {
        auto stars = parseInput(sample_input);
        CHECK(stars[0] == Star{ Vec2(9, 1), Vec2(0, 2) });
        CHECK(stars[1] == Star{ Vec2(7, 0), Vec2(-1, 0) });
        CHECK(stars[3] == Star{ Vec2(6, 10), Vec2(-2, -1) });
        CHECK(stars[30] == Star{ Vec2(-3, 6), Vec2(2, -1) });

        simulateStep(stars);
        CHECK(stars[0] == Star{ Vec2(9, 3), Vec2(0, 2) });
        CHECK(stars[1] == Star{ Vec2(6, 0), Vec2(-1, 0) });
        CHECK(stars[3] == Star{ Vec2(4, 9), Vec2(-2, -1) });
        CHECK(stars[30] == Star{ Vec2(-1, 5), Vec2(2, -1) });

        simulateStep(stars);
        CHECK(stars[0] == Star{ Vec2(9, 5), Vec2(0, 2) });
        CHECK(stars[1] == Star{ Vec2(5, 0), Vec2(-1, 0) });
        CHECK(stars[3] == Star{ Vec2(2, 8), Vec2(-2, -1) });
        CHECK(stars[30] == Star{ Vec2(1, 4), Vec2(2, -1) });
    }

    SECTION("Clustering")
    {
        auto stars = parseInput(sample_input);

        CHECK(cluster(stars, 1000).size() == 28);
        CHECK(cluster(stars, 13).size() == 13);
        simulateStep(stars);
        simulateStep(stars);
        simulateStep(stars);
        CHECK(cluster(stars, 1000).size() == 2);
    }

    SECTION("Find and Print")
    {
        auto stars = parseInput(sample_input);
        std::stringstream sstr;
        int const seconds_elapsed = findAndPrintMessage(stars, sstr);
        CHECK(sstr.str() == R"(#   #  ###
#   #   # 
#   #   # 
#####   # 
#   #   # 
#   #   # 
#   #   # 
#   #  ###
)");
        CHECK(seconds_elapsed == 3);
    }
}
