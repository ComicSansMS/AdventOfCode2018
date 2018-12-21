#include <regex_labyrinth.hpp>

#include <catch.hpp>

#include <sstream>

TEST_CASE("Game of Woods")
{
    SECTION("Simple Labyrinth")
    {
        auto lmap = constructMap("^WNE$");
        CHECK(lmap.rooms[Vec2{0, 0}].position == Vec2{0, 0});          // N      S      W     E
        CHECK(lmap.rooms[Vec2{0, 0}].adjacencies == std::array<bool, 4>{ false, false, true, false });

        CHECK(lmap.rooms[Vec2{-1, 0}].position == Vec2{-1, 0});
        CHECK(lmap.rooms[Vec2{-1, 0}].adjacencies == std::array<bool, 4>{ true, false, false, true });

        CHECK(lmap.rooms[Vec2{-1, -1}].position == Vec2{-1, -1});
        CHECK(lmap.rooms[Vec2{-1, -1}].adjacencies == std::array<bool, 4>{ false, true, false, true });

        CHECK(lmap.rooms[Vec2{0, -1}].position == Vec2{0, -1});
        CHECK(lmap.rooms[Vec2{0, -1}].adjacencies == std::array<bool, 4>{ false, false, true, false });

        CHECK(lmap.min == Vec2{-1, -1});
        CHECK(lmap.max == Vec2{0, 0});

        INFO(lmap);
        std::stringstream sstr;
        sstr << lmap;
        CHECK(sstr.str() == "#####" "\n"
                            "#.|.#" "\n"
                            "#-###" "\n"
                            "#.|X#" "\n"
                            "#####" "\n");
        CHECK(shortestPaths(lmap) == std::make_tuple(3, 0));
    }

    SECTION("Labyrinth with Branches")
    {
        auto const lmap = constructMap("^ENWWW(NEEE|SSE(EE|N))$");

        INFO(lmap);
        std::stringstream sstr;
        sstr << lmap;
        CHECK(sstr.str() == "#########" "\n"
                            "#.|.|.|.#" "\n"
                            "#-#######" "\n"
                            "#.|.|.|.#" "\n"
                            "#-#####-#" "\n"
                            "#.#.#X|.#" "\n"
                            "#-#-#####" "\n"
                            "#.|.|.|.#" "\n"
                            "#########" "\n");
        CHECK(shortestPaths(lmap) == std::make_tuple(10, 0));
    }

    SECTION("Sample Labyrinth 3")
    {
        auto const lmap = constructMap("^ENNWSWW(NEWS|)SSSEEN(WNSE|)EE(SWEN|)NNN$");

        INFO(lmap);
        std::stringstream sstr;
        sstr << lmap;
        CHECK(sstr.str() == "###########" "\n"
                            "#.|.#.|.#.#" "\n"
                            "#-###-#-#-#" "\n"
                            "#.|.|.#.#.#" "\n"
                            "#-#####-#-#" "\n"
                            "#.#.#X|.#.#" "\n"
                            "#-#-#####-#" "\n"
                            "#.#.|.|.|.#" "\n"
                            "#-###-###-#" "\n"
                            "#.|.|.#.|.#" "\n"
                            "###########" "\n");
        CHECK(shortestPaths(lmap) == std::make_tuple(18, 0));
    }

    SECTION("Sample Labyrinth 4")
    {
        auto const lmap = constructMap("^ESSWWN(E|NNENN(EESS(WNSE|)SSS|WWWSSSSE(SW|NNNE)))$");

        INFO(lmap);
        std::stringstream sstr;
        sstr << lmap;
        CHECK(sstr.str() == "#############" "\n"
                            "#.|.|.|.|.|.#" "\n"
                            "#-#####-###-#" "\n"
                            "#.#.|.#.#.#.#" "\n"
                            "#-#-###-#-#-#" "\n"
                            "#.#.#.|.#.|.#" "\n"
                            "#-#-#-#####-#" "\n"
                            "#.#.#.#X|.#.#" "\n"
                            "#-#-#-###-#-#" "\n"
                            "#.|.#.|.#.#.#" "\n"
                            "###-#-###-#-#" "\n"
                            "#.|.#.|.|.#.#" "\n"
                            "#############" "\n");
        CHECK(shortestPaths(lmap) == std::make_tuple(23, 0));
    }

    SECTION("Sample Labyrinth 5")
    {
        auto const lmap = constructMap("^WSSEESWWWNW(S|NENNEEEENN(ESSSSW(NWSW|SSEN)|WSWWN(E|WWS(E|SS))))$");

        INFO(lmap);
        std::stringstream sstr;
        sstr << lmap;
        CHECK(sstr.str() == "###############" "\n"
                            "#.|.|.|.#.|.|.#" "\n"
                            "#-###-###-#-#-#" "\n"
                            "#.|.#.|.|.#.#.#" "\n"
                            "#-#########-#-#" "\n"
                            "#.#.|.|.|.|.#.#" "\n"
                            "#-#-#########-#" "\n"
                            "#.#.#.|X#.|.#.#" "\n"
                            "###-#-###-#-#-#" "\n"
                            "#.|.#.#.|.#.|.#" "\n"
                            "#-###-#####-###" "\n"
                            "#.|.#.|.|.#.#.#" "\n"
                            "#-#-#####-#-#-#" "\n"
                            "#.#.|.|.|.#.|.#" "\n"
                            "###############" "\n");
        CHECK(shortestPaths(lmap) == std::make_tuple(31, 0));
    }
}
