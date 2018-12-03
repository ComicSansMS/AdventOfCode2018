#include <rectangle_cover.hpp>

#include <catch.hpp>

#include <sstream>
#include <string>

TEST_CASE("Rectangle Cover")
{
    char const* sample_input = "#1 @ 1,3: 4x4\n#2 @ 3,1: 4x4\n#3 @ 5,5: 2x2\n";

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

    SECTION("Parse Input")
    {
        auto rects = parseInput(sample_input);
        REQUIRE(rects.size() == 3);
        CHECK(rects[0] == Rectangle{1, 3, 4, 4});
    }

    SECTION("Overlap 1D")
    {
        /// |XXXXX         |
        /// |         XXXXX|
        CHECK(overlap_1d(0, 5, 10, 5) == 0);
        /// |         XXXXX|
        /// |XXXXX         |
        CHECK(overlap_1d(10, 5, 0, 5) == 0);
        /// |XXXXX         |
        /// |    XXXXX     |
        CHECK(overlap_1d(0, 5, 4, 5) == 1);
        /// |    XXXXX     |
        /// |XXXXX         |
        CHECK(overlap_1d(4, 5, 0, 5) == 1);
        /// |  XXXXX       |
        /// |    XXXXX     |
        CHECK(overlap_1d(2, 5, 4, 5) == 3);
        /// |    XXXXX     |
        /// |  XXXXX       |
        CHECK(overlap_1d(4, 5, 2, 5) == 3);
        /// |  XXXXXXXX    |
        /// |    XXXXX     |
        CHECK(overlap_1d(2, 8, 4, 5) == 5);
        /// |    XXXXX     |
        /// |  XXXXXXXX    |
        CHECK(overlap_1d(4, 5, 2, 8) == 5);
        /// |      XXXXX   |
        /// |    XXXXX     |
        CHECK(overlap_1d(6, 5, 4, 5) == 3);
        /// |    XXXXX     |
        /// |      XXXXX   |
        CHECK(overlap_1d(4, 5, 6, 5) == 3);
        /// |      XX      |
        /// |      XX      |
        CHECK(overlap_1d(8, 2, 8, 2) == 2);
    }

    SECTION("Rectangle Overlap")
    {
        CHECK(overlap(Rectangle{1, 1, 4, 4}, Rectangle{10, 10, 4, 4}) == 0);
        CHECK(overlap(Rectangle{1, 3, 4, 4}, Rectangle{3, 1, 4, 4}) == 4);
        CHECK(overlap(Rectangle{1, 3, 4, 4}, Rectangle{5, 5, 2, 2}) == 0);
        CHECK(overlap(Rectangle{3, 1, 4, 4}, Rectangle{5, 5, 2, 2}) == 0);
    }

    SECTION("Total overlap")
    {
        auto rects = parseInput(sample_input);
        CHECK(total_overlap(rects) == 4);
    }

    SECTION("Field dimensions")
    {
        auto rects = parseInput(sample_input);
        CHECK(getFieldDimensions(rects) == Rectangle(1, 1, 7, 7));

        CHECK(getFieldDimensions(std::vector<Rectangle>{Rectangle{1, 2, 1, 1},
                                                        Rectangle{4, 6, 1, 1}}) == Rectangle{1, 2, 5, 6});
    }

    SECTION("Field Construction")
    {
        Field f(Rectangle{2, 4, 8, 7});
        CHECK(f.getDimensions() == Rectangle{2, 4, 8, 7});
        for(int x = 2; x < 10; ++x) {
            for(int y = 4; y < 11; ++y) {
                CHECK(f.getCell(x, y) == 0);
            }
        }
        std::stringstream sstr;
        sstr << f;
        CHECK(sstr.str() == " 0 0 0 0 0 0 0 0\n 0 0 0 0 0 0 0 0\n 0 0 0 0 0 0 0 0\n 0 0 0 0 0 0 0 0\n"
                            " 0 0 0 0 0 0 0 0\n 0 0 0 0 0 0 0 0\n 0 0 0 0 0 0 0 0\n\n");
    }

    SECTION("Field Rectangle Placing")
    {
        auto rects = parseInput(sample_input);
        Field f(getFieldDimensions(rects));
        f.placeRectangle(rects[0]);
        INFO(f);
        CHECK(f.getOverlap() == 0);
        f.placeRectangle(rects[1]);
        INFO(f);
        CHECK(f.getOverlap() == 4);
        f.placeRectangle(rects[2]);
        INFO(f);
        CHECK(f.getOverlap() == 4);
    }

    SECTION("Find Rectangle That Has No Overlap")
    {
        auto rects = parseInput(sample_input);
        Field f(getFieldDimensions(rects));
        for(auto const& r : rects) { f.placeRectangle(r); }
        CHECK(f.findNoneOverlap(rects) == 3);
        f.placeRectangle(Rectangle{4, 4, 2, 2});
        CHECK(f.findNoneOverlap(rects) == -1);
    }
}
