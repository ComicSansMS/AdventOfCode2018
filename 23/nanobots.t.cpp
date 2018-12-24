#include <nanobots.hpp>

#include <catch.hpp>

#include <sstream>

TEST_CASE("Nanobots")
{
    char const sample_input[] = "pos=<0,0,0>, r=4" "\n"
                                "pos=<1,0,0>, r=1" "\n"
                                "pos=<4,0,0>, r=3" "\n"
                                "pos=<0,2,0>, r=1" "\n"
                                "pos=<0,5,0>, r=3" "\n"
                                "pos=<0,0,3>, r=1" "\n"
                                "pos=<1,1,1>, r=1" "\n"
                                "pos=<1,1,2>, r=1" "\n"
                                "pos=<1,3,1>, r=1" "\n";

    SECTION("Parse Input")
    {
        auto const bots = parseInput(sample_input);
        REQUIRE(bots.size() == 9);
        CHECK(bots[0].position == Vec3(0, 0, 0));
        CHECK(bots[0].radius == 4);
        CHECK(bots[1].position == Vec3(1, 0, 0));
        CHECK(bots[1].radius == 1);
        CHECK(bots[2].position == Vec3(4, 0, 0));
        CHECK(bots[2].radius == 3);
        CHECK(bots[3].position == Vec3(0, 2, 0));
        CHECK(bots[3].radius == 1);
        CHECK(bots[4].position == Vec3(0, 5, 0));
        CHECK(bots[4].radius == 3);
        CHECK(bots[5].position == Vec3(0, 0, 3));
        CHECK(bots[5].radius == 1);
        CHECK(bots[6].position == Vec3(1, 1, 1));
        CHECK(bots[6].radius == 1);
        CHECK(bots[7].position == Vec3(1, 1, 2));
        CHECK(bots[7].radius == 1);
        CHECK(bots[8].position == Vec3(1, 3, 1));
        CHECK(bots[8].radius == 1);

        auto const bots2 = parseInput("pos=<-10478543,-10375568,71392729>, r=89396243");
        REQUIRE(bots2.size() == 1);
        CHECK(bots2[0].position.x == -10478543);
        CHECK(bots2[0].position.y == -10375568);
        CHECK(bots2[0].position.z == 71392729);
        CHECK(bots2[0].radius == 89396243);
    }

    SECTION("In Range Of Largest Signal")
    {
        auto const bots = parseInput(sample_input);
        CHECK(inRangeOfLargestSignal(bots) == 7);
    }

    SECTION("Overlaps")
    {
        char const overlap_input[] = "pos=<10,12,12>, r=2"   "\n"
                                     "pos=<12,14,12>, r=2"   "\n"
                                     "pos=<16,12,12>, r=4"   "\n"
                                     "pos=<14,14,14>, r=6"   "\n"
                                     "pos=<50,50,50>, r=200" "\n"
                                     "pos=<10,10,10>, r=5"   "\n";
        auto const bots = parseInput(overlap_input);
        REQUIRE(bots.size() == 6);

        Bounding brect = getBounding(bots);
        CHECK(brect.min == Vec3(10, 10, 10));
        CHECK(brect.max == Vec3(50, 50, 50));

        auto const ov = overlaps(brect, bots);
        CHECK(ov.width == 41);
        CHECK(ov.height == 41);
        CHECK(ov.depth == 41);

        CHECK(ov.max_coords == Vec3(12, 12, 12));
        CHECK(ov.max_overlap == 5);

        for(auto const& b : bots) {
            CHECK(intersects(b, brect));
        }
        Bounding bb2{ Vec3(0, 0, 0), Vec3(9, 9, 9) };
        CHECK(!intersects(bots[0], bb2));
        CHECK(!intersects(bots[1], bb2));
        CHECK(!intersects(bots[2], bb2));
        CHECK(!intersects(bots[3], bb2));
        CHECK(intersects(bots[4], bb2));
        CHECK(intersects(bots[5], bb2));
    }
}
