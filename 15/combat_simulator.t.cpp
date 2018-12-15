#include <combat_simulator.hpp>

#include <catch.hpp>

#include <sstream>

TEST_CASE("Combat Simulator")
{
    SECTION("Position Equality")
    {
        CHECK(Position{1, 2} == Position{1, 2});
        CHECK_FALSE(Position{1, 2} == Position{2, 2});
        CHECK_FALSE(Position{1, 2} == Position{1, 3});
        CHECK_FALSE(Position{1, 2} == Position{2, 1});
        CHECK(Position{2, 1} == Position{2, 1});
    }

    SECTION("Position Less Than Is Reading Order")
    {
        CHECK_FALSE(Position{1, 2} < Position{1, 2});
        CHECK(Position{2, 1} < Position{4, 1});
        CHECK(Position{4, 1} < Position{1, 2});
        CHECK(Position{2, 1} < Position{1, 2});
        CHECK(Position{2, 1} < Position{2, 3});
        CHECK(Position{2, 1} < Position{1, 2});
    }

    SECTION("Faction Negation")
    {
        CHECK(!Faction::Elf == Faction::Goblin);
        CHECK(!Faction::Goblin == Faction::Elf);
    }

    char const* sample_input_parse = "#######" "\n"
                                     "#.G.E.#" "\n"
                                     "#E.G.E#" "\n"
                                     "#.G.E.#" "\n"
                                     "#######" "\n";
    SECTION("Parse Input")
    {
        auto const b = parseInput(sample_input_parse);
        CHECK(b.fieldWidth == 7);
        CHECK(b.fieldHeight == 5);
        REQUIRE(b.field.size() == 35);
        CHECK(b.nGoblins == 3);
        CHECK(b.nElves == 4);

        CHECK(std::string_view(&b.field[0], 7) == "#######");
        CHECK(std::string_view(&b.field[7], 7) == "#.....#");
        CHECK(std::string_view(&b.field[14], 7) == "#.....#");
        CHECK(std::string_view(&b.field[21], 7) == "#.....#");
        CHECK(std::string_view(&b.field[28], 7) == "#######");

        CHECK(b.units[0].id == 1);
        CHECK(b.units[0].faction == Faction::Goblin);
        CHECK(b.units[0].hitpoints == 200);
        CHECK(b.units[0].position == Position{2, 1});

        CHECK(b.units[1].id == 2);
        CHECK(b.units[1].faction == Faction::Elf);
        CHECK(b.units[1].hitpoints == 200);
        CHECK(b.units[1].position == Position{4, 1});

        CHECK(b.units[2].id == 3);
        CHECK(b.units[2].faction == Faction::Elf);
        CHECK(b.units[2].hitpoints == 200);
        CHECK(b.units[2].position == Position{1, 2});

        CHECK(b.units[3].id == 4);
        CHECK(b.units[3].faction == Faction::Goblin);
        CHECK(b.units[3].hitpoints == 200);
        CHECK(b.units[3].position == Position{3, 2});

        CHECK(b.units[4].id == 5);
        CHECK(b.units[4].faction == Faction::Elf);
        CHECK(b.units[4].hitpoints == 200);
        CHECK(b.units[4].position == Position{5, 2});

        CHECK(b.units[5].id == 6);
        CHECK(b.units[5].faction == Faction::Goblin);
        CHECK(b.units[5].hitpoints == 200);
        CHECK(b.units[5].position == Position{2, 3});

        CHECK(b.units[6].id == 7);
        CHECK(b.units[6].faction == Faction::Elf);
        CHECK(b.units[6].hitpoints == 200);
        CHECK(b.units[6].position == Position{4, 3});
    }

    SECTION("Battlefield Printing")
    {
        auto const b = parseInput(sample_input_parse);
        std::stringstream sstr;
        sstr << b;
        CHECK(sstr.str() == sample_input_parse);
    }

    char const* sample_input2 = "#######" "\n"
                                "#E..G.#" "\n"
                                "#...#.#" "\n"
                                "#.G.#G#" "\n"
                                "#######" "\n";

    SECTION("Is In Range")
    {
        auto b = parseInput(sample_input2);

        CHECK(!b.isInRangeFor(Faction::Elf, 2, 1));
        CHECK(b.isInRangeFor(Faction::Elf, 3, 1));
        CHECK(b.isInRangeFor(Faction::Elf, 5, 1));
        CHECK(!b.isInRangeFor(Faction::Elf, 1, 2));
        CHECK(b.isInRangeFor(Faction::Elf, 2, 2));
        CHECK(!b.isInRangeFor(Faction::Elf, 3, 2));
        CHECK(b.isInRangeFor(Faction::Elf, 5, 2));
        CHECK(b.isInRangeFor(Faction::Elf, 1, 3));
        CHECK(b.isInRangeFor(Faction::Elf, 3, 3));

        CHECK(b.isInRangeFor(Faction::Goblin, 2, 1));
        CHECK(b.isInRangeFor(Faction::Goblin, 1, 2));
    }

    SECTION("Unit Shortest Paths")
    {
        char const* sample_input_shortest_paths = "#######" "\n"
                                                  "#.E...#" "\n"
                                                  "#.....#" "\n"
                                                  "#...G.#" "\n"
                                                  "#######" "\n";
        auto const b1 = parseInput(sample_input2);
        CHECK(b1.shortestPaths(b1.units[0])[1] == Position{2, 1});
        CHECK(b1.shortestPaths(b1.units[0])[0] == Position{3, 1});

        auto b2 = parseInput(sample_input_shortest_paths);
        CHECK(b2.shortestPaths(b2.units[0]).back() == Position{3, 1});

        auto const b3 = parseInput("######\n#....#\n#.EG.#\n#....#\n######\n");
        CHECK(b3.shortestPaths(b3.units[0]).empty());
    }

    SECTION("Play Turn (Movement Only)")
    {
        char const* sample_input_move = "#########" "\n"
                                        "#G..G..G#" "\n"
                                        "#.......#" "\n"
                                        "#.......#" "\n"
                                        "#G..E..G#" "\n"
                                        "#.......#" "\n"
                                        "#.......#" "\n"
                                        "#G..G..G#" "\n"
                                        "#########" "\n";
        auto b = parseInput(sample_input_move);

        // #########
        // #.G...G.#
        // #...G...#
        // #...E..G#
        // #.G.....#
        // #.......#
        // #G..G..G#
        // #.......#
        // #########
        b.playTurn();
        INFO(b);
        CHECK(b.units[0].position == Position{2, 1});
        CHECK(b.units[1].position == Position{4, 2});
        CHECK(b.units[2].position == Position{6, 1});
        CHECK(b.units[3].position == Position{2, 4});
        CHECK(b.units[4].faction == Faction::Elf);
        CHECK(b.units[4].position == Position{4, 3});
        CHECK(b.units[5].position == Position{7, 3});
        CHECK(b.units[6].position == Position{1, 6});
        CHECK(b.units[7].position == Position{4, 6});
        CHECK(b.units[8].position == Position{7, 6});

        // #########
        // #..G.G..#
        // #...G...#
        // #.G.E.G.#
        // #.......#
        // #G..G..G#
        // #.......#
        // #.......#
        // #########
        b.playTurn();
        INFO(b);
        CHECK(b.units[0].position == Position{3, 1});
        CHECK(b.units[1].position == Position{5, 1});
        CHECK(b.units[2].position == Position{4, 2});
        CHECK(b.units[3].faction == Faction::Elf);
        CHECK(b.units[3].position == Position{4, 3});
        CHECK(b.units[4].position == Position{6, 3});
        CHECK(b.units[5].position == Position{2, 3});
        CHECK(b.units[6].position == Position{1, 5});
        CHECK(b.units[7].position == Position{4, 5});
        CHECK(b.units[8].position == Position{7, 5});

        // #########
        // #.......#
        // #..GGG..#
        // #..GEG..#
        // #G..G...#
        // #......G#
        // #.......#
        // #.......#
        // #########
        b.playTurn();
        INFO(b);
        CHECK(b.units[0].position == Position{3, 2});
        CHECK(b.units[1].position == Position{5, 2});
        CHECK(b.units[2].position == Position{4, 2});
        CHECK(b.units[3].position == Position{3, 3});
        CHECK(b.units[4].faction == Faction::Elf);
        CHECK(b.units[4].position == Position{4, 3});
        CHECK(b.units[5].position == Position{5, 3});
        CHECK(b.units[6].position == Position{1, 4});
        CHECK(b.units[7].position == Position{4, 4});
        CHECK(b.units[8].position == Position{7, 5});

        // stalemate from here on
        for(int i=0; i<5; ++i) {
            b.playTurn();
            INFO(b);
            CHECK(b.units[0].position == Position{3, 2});
            CHECK(b.units[1].position == Position{4, 2});
            CHECK(b.units[2].position == Position{5, 2});
            CHECK(b.units[3].position == Position{3, 3});
            CHECK(b.units[4].faction == Faction::Elf);
            CHECK(b.units[4].position == Position{4, 3});
            CHECK(b.units[5].position == Position{5, 3});
            CHECK(b.units[6].position == Position{1, 4});
            CHECK(b.units[7].position == Position{4, 4});
            CHECK(b.units[8].position == Position{7, 5});
        }
    }

    SECTION("Attack")
    {
        char const* sample_input_attack = "#######" "\n"
                                          "#G....#" "\n"
                                          "#..G..#" "\n"
                                          "#..EG.#" "\n"
                                          "#..G..#" "\n"
                                          "#...G.#" "\n"
                                          "#######" "\n";
        auto b = parseInput(sample_input_attack);
        b.units[0].hitpoints = 9;
        b.units[1].hitpoints = 4;
        b.units[3].hitpoints = 2;
        b.units[4].hitpoints = 2;
        b.units[5].hitpoints = 1;

        // G....  9       G....  9
        // ..G..  4       ..G..  4
        // ..EG.  2  -->  ..E..
        // ..G..  2       ..G..  2
        // ...G.  1       ...G.  1
        REQUIRE(b.units[2].faction == Faction::Elf);
        bool const did_kill = b.attack(b.units[2]);
        CHECK(did_kill);
        CHECK(b.units[0].hitpoints == 9);
        CHECK(b.units[1].hitpoints == 4);
        CHECK(b.units[3].hitpoints == 2);
        CHECK(b.units[4].hitpoints == 1);

        REQUIRE(b.graveyard.size() == 1);
        CHECK(b.graveyard[0].position == Position{4, 3});
        CHECK(b.graveyard[0].hitpoints == -1);
    }

    char const* sample_input_battle = "#######" "\n"
                                      "#.G...#" "\n"
                                      "#...EG#" "\n"
                                      "#.#.#G#" "\n"
                                      "#..G#E#" "\n"
                                      "#.....#" "\n"
                                      "#######" "\n";

    SECTION("Battle rounds")
    {
        auto b = parseInput(sample_input_battle);

        // #######
        // #..G..#   G(200)
        // #...EG#   E(197), G(197)
        // #.#G#G#   G(200), G(197)
        // #...#E#   E(197)
        // #.....#
        // #######
        b.playTurn();
        {
            std::stringstream sstr;
            sstr << b;
            CHECK(sstr.str() == "#######" "\n"
                                "#..G..#" "\n"
                                "#...EG#" "\n"
                                "#.#G#G#" "\n"
                                "#...#E#" "\n"
                                "#.....#" "\n"
                                "#######" "\n");
        }
        CHECK(b.units[0].hitpoints == 200);
        CHECK(b.units[1].hitpoints == 197);
        CHECK(b.units[2].hitpoints == 197);
        CHECK(b.units[3].hitpoints == 197);
        CHECK(b.units[4].hitpoints == 200);
        CHECK(b.units[5].hitpoints == 197);

        // #######
        // #...G.#   G(200)
        // #..GEG#   G(200), E(188), G(194)
        // #.#.#G#   G(194)
        // #...#E#   E(194)
        // #.....#
        // #######
        b.playTurn();
        {
            std::stringstream sstr;
            sstr << b;
            CHECK(sstr.str() == "#######" "\n"
                                "#...G.#" "\n"
                                "#..GEG#" "\n"
                                "#.#.#G#" "\n"
                                "#...#E#" "\n"
                                "#.....#" "\n"
                                "#######" "\n");
        }
        CHECK(b.units[0].hitpoints == 200);
        CHECK(b.units[1].hitpoints == 188);
        CHECK(b.units[2].hitpoints == 194);
        CHECK(b.units[3].hitpoints == 200);
        CHECK(b.units[4].hitpoints == 194);
        CHECK(b.units[5].hitpoints == 194);

        // #######
        // #...G.#   G(200)
        // #..G.G#   G(200), G(131)
        // #.#.#G#   G(131)
        // #...#E#   E(131)
        // #.....#
        // #######
        for(int i = 0; i < 21; ++i) {
            b.playTurn();
        }
        {
            std::stringstream sstr;
            sstr << b;
            CHECK(sstr.str() == "#######" "\n"
                                "#...G.#" "\n"
                                "#..G.G#" "\n"
                                "#.#.#G#" "\n"
                                "#...#E#" "\n"
                                "#.....#" "\n"
                                "#######" "\n");
        }
        CHECK(b.graveyard[0].position == Position{4, 2});
        CHECK(b.units[0].hitpoints == 200);
        CHECK(b.units[1].hitpoints == 200);
        CHECK(b.units[2].hitpoints == 131);
        CHECK(b.units[3].hitpoints == 131);
        CHECK(b.units[4].hitpoints == 131);

        // #######
        // #G....#   G(200)
        // #.G...#   G(131)
        // #.#.#G#   G(116)
        // #...#E#   E(113)
        // #....G#   G(200)
        // #######
        for(int i = 0; i < 5; ++i) {
            b.playTurn();
        }
        {
            std::stringstream sstr;
            sstr << b;
            CHECK(sstr.str() == "#######" "\n"
                                "#G....#" "\n"
                                "#.G...#" "\n"
                                "#.#.#G#" "\n"
                                "#...#E#" "\n"
                                "#....G#" "\n"
                                "#######" "\n");
        }
        CHECK(b.units[0].hitpoints == 200);
        CHECK(b.units[1].hitpoints == 131);
        CHECK(b.units[2].hitpoints == 116);
        CHECK(b.units[3].hitpoints == 113);
        CHECK(b.units[4].hitpoints == 200);

        // #######
        // #G....#   G(200)
        // #.G...#   G(131)
        // #.#.#G#   G(59)
        // #...#.#
        // #....G#   G(200)
        // #######
        for(int i = 0; i < 19; ++i) {
            b.playTurn();
        }
        {
            std::stringstream sstr;
            sstr << b;
            CHECK(sstr.str() == "#######" "\n"
                                "#G....#" "\n"
                                "#.G...#" "\n"
                                "#.#.#G#" "\n"
                                "#...#.#" "\n"
                                "#....G#" "\n"
                                "#######" "\n");
        }
        CHECK(b.graveyard[0].position == Position{4, 2});
        CHECK(b.graveyard[1].position == Position{5, 4});
        CHECK(b.units[0].hitpoints == 200);
        CHECK(b.units[1].hitpoints == 131);
        CHECK(b.units[2].hitpoints == 59);
        CHECK(b.units[3].hitpoints == 200);
    }

    SECTION("Battle simulation #1")
    {
        auto b = parseInput(sample_input_battle);

        auto const stats = b.simulateBattle();
        CHECK(stats.turns_played == 47);
        CHECK(stats.hitpoints_remaining == 590);
        CHECK(stats.outcome == 27730);
    }

    SECTION("Battle simulation #2")
    {
        // #######       #######
        // #G..#E#       #...#E#   E(200)
        // #E#E.E#       #E#...#   E(197)
        // #G.##.#  -->  #.E##.#   E(185)
        // #...#E#       #E..#E#   E(200), E(200)
        // #...E.#       #.....#
        // #######       #######
        char const* sample_battle = "#######" "\n"
                                    "#G..#E#" "\n"
                                    "#E#E.E#" "\n"
                                    "#G.##.#" "\n"
                                    "#...#E#" "\n"
                                    "#...E.#" "\n"
                                    "#######" "\n";
        auto b = parseInput(sample_battle);
        auto const stats = b.simulateBattle();
        INFO(b);
        CHECK(stats.turns_played == 37);
        CHECK(stats.hitpoints_remaining == 982);
        CHECK(stats.outcome == 36334);
    }

    SECTION("Battle simulation #3")
    {
        // #######       #######
        // #E..EG#       #.E.E.#   E(164), E(197)
        // #.#G.E#       #.#E..#   E(200)
        // #E.##E#  -->  #E.##.#   E(98)
        // #G..#.#       #.E.#.#   E(200)
        // #..E#.#       #...#.#
        // #######       #######
        char const* sample_battle = "#######" "\n"
                                    "#E..EG#" "\n"
                                    "#.#G.E#" "\n"
                                    "#E.##E#" "\n"
                                    "#G..#.#" "\n"
                                    "#..E#.#" "\n"
                                    "#######" "\n";
        auto b = parseInput(sample_battle);
        auto const stats = b.simulateBattle();
        INFO(b);
        CHECK(stats.turns_played == 46);
        CHECK(stats.hitpoints_remaining == 859);
        CHECK(stats.outcome == 39514);
    }

    SECTION("Battle simulation #4")
    {
        // #######       #######
        // #E.G#.#       #G.G#.#   G(200), G(98)
        // #.#G..#       #.#G..#   G(200)
        // #G.#.G#  -->  #..#..#
        // #G..#.#       #...#G#   G(95)
        // #...E.#       #...G.#   G(200)
        // #######       #######
        char const* sample_battle = "#######" "\n"
                                    "#E.G#.#" "\n"
                                    "#.#G..#" "\n"
                                    "#G.#.G#" "\n"
                                    "#G..#.#" "\n"
                                    "#...E.#" "\n"
                                    "#######" "\n";
        auto b = parseInput(sample_battle);
        auto const stats = b.simulateBattle();
        INFO(b);
        CHECK(stats.turns_played == 35);
        CHECK(stats.hitpoints_remaining == 793);
        CHECK(stats.outcome == 27755);
    }

    SECTION("Battle simulation #5")
    {
        // #######       #######
        // #.E...#       #.....#
        // #.#..G#       #.#G..#   G(200)
        // #.###.#  -->  #.###.#
        // #E#G#G#       #.#.#.#
        // #...#G#       #G.G#G#   G(98), G(38), G(200)
        // #######       #######
        char const* sample_battle = "#######" "\n"
                                    "#.E...#" "\n"
                                    "#.#..G#" "\n"
                                    "#.###.#" "\n"
                                    "#E#G#G#" "\n"
                                    "#...#G#" "\n"
                                    "#######" "\n";
        auto b = parseInput(sample_battle);
        auto const stats = b.simulateBattle();
        INFO(b);
        CHECK(stats.turns_played == 54);
        CHECK(stats.hitpoints_remaining == 536);
        CHECK(stats.outcome == 28944);
    }

    SECTION("Battle simulation #6")
    {
        // #########       #########   
        // #G......#       #.G.....#   G(137)
        // #.E.#...#       #G.G#...#   G(200), G(200)
        // #..##..G#       #.G##...#   G(200)
        // #...##..#  -->  #...##..#   
        // #...#...#       #.G.#...#   G(200)
        // #.G...G.#       #.......#   
        // #.....G.#       #.......#   
        // #########       ######### 
        char const* sample_battle = "#########" "\n"
                                    "#G......#" "\n"
                                    "#.E.#...#" "\n"
                                    "#..##..G#" "\n"
                                    "#...##..#" "\n"
                                    "#...#...#" "\n"
                                    "#.G...G.#" "\n"
                                    "#.....G.#" "\n"
                                    "#########" "\n";
        auto b = parseInput(sample_battle);
        auto const stats = b.simulateBattle();
        INFO(b);
        CHECK(stats.turns_played == 20);
        CHECK(stats.hitpoints_remaining == 937);
        CHECK(stats.outcome == 18740);
    }
}
