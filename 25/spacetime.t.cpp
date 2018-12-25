#include <spacetime.hpp>

#include <catch.hpp>

#include <algorithm>

namespace {
bool contains(std::vector<Vec4> const& v, Vec4 x) {
    return std::find(begin(v), end(v), x) != end(v);
}
}

TEST_CASE("Adventures in Spacetime")
{
    SECTION("Vec4 Construction")
    {
        Vec4 v(1, 2, 3, 4);
        CHECK(v.x == 1);
        CHECK(v.y == 2);
        CHECK(v.z == 3);
        CHECK(v.t == 4);

        CHECK(v == Vec4(1, 2, 3, 4));
        CHECK_FALSE(v == Vec4(0, 2, 3, 4));
        CHECK_FALSE(v == Vec4(1, 0, 3, 4));
        CHECK_FALSE(v == Vec4(1, 2, 0, 4));
        CHECK_FALSE(v == Vec4(1, 2, 3, 0));
    }

    SECTION("Input Parsing")
    {
        auto const vecs = parseInput("-1,2,2,0"          "\n"
                                     "12,55,42,-1221332" "\n"
                                     "0,0,0,-2"          "\n"
                                     "-1,2,0,0"          "\n"
                                     "-2,-2,-2,2"        "\n");
        REQUIRE(vecs.size() == 5);
        CHECK(vecs[0] == Vec4(-1, 2, 2, 0));
        CHECK(vecs[1] == Vec4(12, 55, 42, -1221332));
        CHECK(vecs[2] == Vec4(0, 0, 0, -2));
        CHECK(vecs[3] == Vec4(-1, 2, 0, 0));
        CHECK(vecs[4] == Vec4(-2, -2, -2, 2));
    }

    SECTION("Manhattan Distance")
    {
        CHECK(distance(Vec4(1,2,3,4), Vec4(5, -2, 0, 8)) == 15);
    }

    SECTION("Cluster #1")
    {
        auto vecs = parseInput("0,0,0,0"  "\n"
                               "3,0,0,0"  "\n"
                               "0,3,0,0"  "\n"
                               "0,0,3,0"  "\n"
                               "0,0,0,3"  "\n"
                               "0,0,0,6"  "\n"
                               "9,0,0,0"  "\n"
                               "12,0,0,0" "\n");

        auto const clusters = cluster(vecs);
        REQUIRE(clusters.size() == 2);
        CHECK(clusters[0].size() == 2);
        CHECK(clusters[1].size() == 6);
        CHECK(contains(clusters[1], Vec4(0,0,0,0)));
        CHECK(contains(clusters[1], Vec4(3,0,0,0)));
        CHECK(contains(clusters[1], Vec4(0,3,0,0)));
        CHECK(contains(clusters[1], Vec4(0,0,3,0)));
        CHECK(contains(clusters[1], Vec4(0,0,0,3)));
        CHECK(contains(clusters[1], Vec4(0,0,0,6)));
        CHECK(contains(clusters[0], Vec4(9,0,0,0)));
        CHECK(contains(clusters[0], Vec4(12,0,0,0)));

        vecs.push_back(Vec4(6,0,0,0));
        auto const clusters2 = cluster(vecs);
        REQUIRE(clusters2.size() == 1);
        CHECK(clusters2[0].size() == 9);
    }

    SECTION("Cluster #2")
    {
        auto const vecs = parseInput("-1,2,2,0"   "\n"
                                     "0,0,2,-2"   "\n"
                                     "0,0,0,-2"   "\n"
                                     "-1,2,0,0"   "\n"
                                     "-2,-2,-2,2" "\n"
                                     "3,0,2,-1"   "\n"
                                     "-1,3,2,2"   "\n"
                                     "-1,0,-1,0"  "\n"
                                     "0,2,1,-2"   "\n"
                                     "3,0,0,0"    "\n");

        auto const clusters = cluster(vecs);
        CHECK(clusters.size() == 4);
    }

    SECTION("Cluster #3")
    {
        auto const vecs = parseInput("1,-1,0,1"  "\n"
                                     "2,0,-1,0"  "\n"
                                     "3,2,-1,0"  "\n"
                                     "0,0,3,1"   "\n"
                                     "0,0,-1,-1" "\n"
                                     "2,3,-2,0"  "\n"
                                     "-2,2,0,0"  "\n"
                                     "2,-2,0,-1" "\n"
                                     "1,-1,0,-1" "\n"
                                     "3,2,0,2"   "\n");

        auto const clusters = cluster(vecs);
        CHECK(clusters.size() == 3);
    }

    SECTION("Cluster #4")
    {
        auto const vecs = parseInput("1,-1,-1,-2" "\n"
                                     "-2,-2,0,1"  "\n"
                                     "0,2,1,3"    "\n"
                                     "-2,3,-2,1"  "\n"
                                     "0,2,3,-2"   "\n"
                                     "-1,-1,1,-2" "\n"
                                     "0,-2,-1,0"  "\n"
                                     "-2,2,3,-1"  "\n"
                                     "1,2,2,0"    "\n"
                                     "-1,-2,0,-2" "\n");

        auto const clusters = cluster(vecs);
        CHECK(clusters.size() == 8);
    }
}
