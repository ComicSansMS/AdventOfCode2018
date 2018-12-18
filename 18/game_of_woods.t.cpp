#include <game_of_woods.hpp>

#include <catch.hpp>

#include <sstream>

TEST_CASE("Game of Woods")
{
    char const sample_input[] = ".#.#...|#." "\n"
                                ".....#|##|" "\n"
                                ".|..|...#." "\n"
                                "..|#.....#" "\n"
                                "#.#|||#|#|" "\n"
                                "...#.||..." "\n"
                                ".|....|..." "\n"
                                "||...#|.#|" "\n"
                                "|.||||..|." "\n"
                                "...#.|..|." "\n";

    SECTION("Parse Input")
    {
        auto acre = parseInput(sample_input);
        CHECK(acre.m_width == 10);
        CHECK(acre.m_height == 10);
        CHECK(acre.m_field.size() == 10*10);

        std::stringstream sstr;
        sstr << acre;
        CHECK(sstr.str() == ".#.#...|#." "\n"
                            ".....#|##|" "\n"
                            ".|..|...#." "\n"
                            "..|#.....#" "\n"
                            "#.#|||#|#|" "\n"
                            "...#.||..." "\n"
                            ".|....|..." "\n"
                            "||...#|.#|" "\n"
                            "|.||||..|." "\n"
                            "...#.|..|." "\n");
    }

    SECTION("Condition Open Acre")
    {
        // An open acre will become filled with trees if three or more adjacent acres contained trees.
        // Otherwise, nothing happens.
        auto a1 = parseInput("|.|" "\n"
                             "..." "\n"
                             ".|." "\n");
        CHECK(a1.openCondition(1, 1));

        auto a2 = parseInput("|#|" "\n"
                             "#.#" "\n"
                             "#|#" "\n");
        CHECK(a2.openCondition(1, 1));

        auto a3 = parseInput("#.|" "\n"
                             "..." "\n"
                             ".|#" "\n");
        CHECK_FALSE(a3.openCondition(1, 1));
    }

    SECTION("Condition Tree Acre")
    {
        // An acre filled with trees will become a lumberyard if three or more adjacent acres were lumberyards.
        // Otherwise, nothing happens.
        auto a1 = parseInput("|.|" "\n"
                             ".|." "\n"
                             ".|." "\n");
        CHECK_FALSE(a1.treeCondition(1, 1));

        auto a2 = parseInput("|#|" "\n"
                             ".|#" "\n"
                             "#|." "\n");
        CHECK(a2.treeCondition(1, 1));

        auto a3 = parseInput("#.|" "\n"
                             "||#" "\n"
                             ".##" "\n");
        CHECK(a3.treeCondition(1, 1));
    }

    SECTION("Condition Tree Acre")
    {
        // An acre containing a lumberyard will remain a lumberyard if it was adjacent to at least one other
        // lumberyard and at least one acre containing trees. Otherwise, it becomes open.
        auto a1 = parseInput("|.|" "\n"
                             ".#." "\n"
                             ".|." "\n");
        CHECK(a1.yardCondition(1, 1));

        auto a2 = parseInput("|#|" "\n"
                             ".##" "\n"
                             "#|." "\n");
        CHECK_FALSE(a2.yardCondition(1, 1));

        auto a3 = parseInput("#.." "\n"
                             "###" "\n"
                             ".##" "\n");
        CHECK(a3.yardCondition(1, 1));

        auto a4 = parseInput("..." "\n"
                             ".#|" "\n"
                             "..#" "\n");
        CHECK_FALSE(a4.yardCondition(1, 1));
    }

    SECTION("Step")
    {
        auto acre = parseInput(sample_input);
        std::stringstream sstr;

        sstr << acre;
        INFO(acre << '\n');
        CHECK(sstr.str() == ".#.#...|#." "\n"
                            ".....#|##|" "\n"
                            ".|..|...#." "\n"
                            "..|#.....#" "\n"
                            "#.#|||#|#|" "\n"
                            "...#.||..." "\n"
                            ".|....|..." "\n"
                            "||...#|.#|" "\n"
                            "|.||||..|." "\n"
                            "...#.|..|." "\n");
        sstr.str(std::string{});

        // 1 minute
        acre = step(acre);
        sstr << acre;
        INFO(acre << '\n');
        CHECK(sstr.str() == ".......##." "\n"
                            "......|###" "\n"
                            ".|..|...#." "\n"
                            "..|#||...#" "\n"
                            "..##||.|#|" "\n"
                            "...#||||.." "\n"
                            "||...|||.." "\n"
                            "|||||.||.|" "\n"
                            "||||||||||" "\n"
                            "....||..|." "\n");
        sstr.str(std::string{});

        // 2 minutes
        acre = step(acre);
        sstr << acre;
        INFO(acre << '\n');
        CHECK(sstr.str() == ".......#.." "\n"
                            "......|#.." "\n"
                            ".|.|||...." "\n"
                            "..##|||..#" "\n"
                            "..###|||#|" "\n"
                            "...#|||||." "\n"
                            "|||||||||." "\n"
                            "||||||||||" "\n"
                            "||||||||||" "\n"
                            ".|||||||||" "\n");
        sstr.str(std::string{});

        // 3 minutes
        acre = step(acre);
        sstr << acre;
        INFO(acre << '\n');
        CHECK(sstr.str() == ".......#.." "\n"
                            "....|||#.." "\n"
                            ".|.||||..." "\n"
                            "..###|||.#" "\n"
                            "...##|||#|" "\n"
                            ".||##|||||" "\n"
                            "||||||||||" "\n"
                            "||||||||||" "\n"
                            "||||||||||" "\n"
                            "||||||||||" "\n");
        sstr.str(std::string{});

        // 4 minutes
        acre = step(acre);
        sstr << acre;
        INFO(acre << '\n');
        CHECK(sstr.str() == ".....|.#.." "\n"
                            "...||||#.." "\n"
                            ".|.#||||.." "\n"
                            "..###||||#" "\n"
                            "...###||#|" "\n"
                            "|||##|||||" "\n"
                            "||||||||||" "\n"
                            "||||||||||" "\n"
                            "||||||||||" "\n"
                            "||||||||||" "\n");
        sstr.str(std::string{});

        // 5 minutes
        acre = step(acre);
        sstr << acre;
        INFO(acre << '\n');
        CHECK(sstr.str() == "....|||#.." "\n"
                            "...||||#.." "\n"
                            ".|.##||||." "\n"
                            "..####|||#" "\n"
                            ".|.###||#|" "\n"
                            "|||###||||" "\n"
                            "||||||||||" "\n"
                            "||||||||||" "\n"
                            "||||||||||" "\n"
                            "||||||||||" "\n");
        sstr.str(std::string{});

        // 6 minutes
        acre = step(acre);
        sstr << acre;
        INFO(acre << '\n');
        CHECK(sstr.str() == "...||||#.." "\n"
                            "...||||#.." "\n"
                            ".|.###|||." "\n"
                            "..#.##|||#" "\n"
                            "|||#.##|#|" "\n"
                            "|||###||||" "\n"
                            "||||#|||||" "\n"
                            "||||||||||" "\n"
                            "||||||||||" "\n"
                            "||||||||||" "\n");
        sstr.str(std::string{});

        // 7 minutes
        acre = step(acre);
        sstr << acre;
        INFO(acre << '\n');
        CHECK(sstr.str() == "...||||#.." "\n"
                            "..||#|##.." "\n"
                            ".|.####||." "\n"
                            "||#..##||#" "\n"
                            "||##.##|#|" "\n"
                            "|||####|||" "\n"
                            "|||###||||" "\n"
                            "||||||||||" "\n"
                            "||||||||||" "\n"
                            "||||||||||" "\n");
        sstr.str(std::string{});

        // 8 minutes
        acre = step(acre);
        sstr << acre;
        INFO(acre << '\n');
        CHECK(sstr.str() == "..||||##.." "\n"
                            "..|#####.." "\n"
                            "|||#####|." "\n"
                            "||#...##|#" "\n"
                            "||##..###|" "\n"
                            "||##.###||" "\n"
                            "|||####|||" "\n"
                            "||||#|||||" "\n"
                            "||||||||||" "\n"
                            "||||||||||" "\n");
        sstr.str(std::string{});

        // 9 minutes
        acre = step(acre);
        sstr << acre;
        INFO(acre << '\n');
        CHECK(sstr.str() == "..||###..." "\n"
                            ".||#####.." "\n"
                            "||##...##." "\n"
                            "||#....###" "\n"
                            "|##....##|" "\n"
                            "||##..###|" "\n"
                            "||######||" "\n"
                            "|||###||||" "\n"
                            "||||||||||" "\n"
                            "||||||||||" "\n");
        sstr.str(std::string{});

        // 10 minutes
        acre = step(acre);
        sstr << acre;
        INFO(acre << '\n');
        CHECK(sstr.str() == ".||##....." "\n"
                            "||###....." "\n"
                            "||##......" "\n"
                            "|##.....##" "\n"
                            "|##.....##" "\n"
                            "|##....##|" "\n"
                            "||##.####|" "\n"
                            "||#####|||" "\n"
                            "||||#|||||" "\n"
                            "||||||||||" "\n");
        sstr.str(std::string{});
    }

    SECTION("Total Resource Value After 10")
    {
        using namespace std::chrono_literals;
        auto const a = parseInput(sample_input);
        CHECK(getTotalResourceValueAfter(a, 10min) == 1147);
    }

    SECTION("Find Cycle")
    {
        auto const a = parseInput(sample_input);
        auto const [acres, cycle_start] = findCycle(a);
        CHECK(acres.size() == 20);
        CHECK(cycle_start == 18);
    }
}
