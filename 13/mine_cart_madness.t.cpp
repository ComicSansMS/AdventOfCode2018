#include <mine_cart_madness.hpp>

#include <catch.hpp>

TEST_CASE("Mine Cart Madness")
{
    char const* sample_input = R"(/->-\        
|   |  /----\
| /-+--+-\  |
| | |  | v  |
\-+-/  \-+--/
  \------/   
)";
    SECTION("Parse Input")
    {
        Field const f = parseInput(sample_input);
        CHECK(f.track_width == 13);
        CHECK(f.track_height == 6);
        char const* track = R"(/---\        )"
                            R"(|   |  /----\)"
                            R"(| /-+--+-\  |)"
                            R"(| | |  | |  |)"
                            R"(\-+-/  \-+--/)"
                            R"(  \------/   )";
        CHECK(std::string_view(f.tracks.data(), f.tracks.size()) == track);

        CHECK(f.carts[0].coords == std::make_tuple(2, 0));
        CHECK(f.carts[0].direction == Cart::Right);

        CHECK(f.carts[1].coords == std::make_tuple(9, 3));
        CHECK(f.carts[1].direction == Cart::Down);
    }

    SECTION("Simulate Step")
    {
        Field f = parseInput(sample_input);
        // /->-\         //
        // |   |  /----\ //
        // | /-+--+-\  | //
        // | | |  | v  | //
        // \-+-/  \-+--/ //
        // \------/      //

        CHECK(simulateStep(f).empty());
        // /-->\         //
        // |   |  /----\ //
        // | /-+--+-\  | //
        // | | |  | |  | //
        // \-+-/  \->--/ //
        // \------/      //
        CHECK(f.carts[0].coords == std::make_tuple(3, 0));
        CHECK(f.carts[0].direction == Cart::Right);
        CHECK(f.carts[1].coords == std::make_tuple(9, 4));
        CHECK(f.carts[1].direction == Cart::Right);

        CHECK(simulateStep(f).empty());
        // /---v         //
        // |   |  /----\ //
        // | /-+--+-\  | //
        // | | |  | |  | //
        // \-+-/  \-+>-/ //
        // \------/      //
        CHECK(f.carts[0].coords == std::make_tuple(4, 0));
        CHECK(f.carts[0].direction == Cart::Down);
        CHECK(f.carts[1].coords == std::make_tuple(10, 4));
        CHECK(f.carts[1].direction == Cart::Right);

        CHECK(simulateStep(f).empty());
        // /---\         //
        // |   v  /----\ //
        // | /-+--+-\  | //
        // | | |  | |  | //
        // \-+-/  \-+->/ //
        // \------/      //
        CHECK(f.carts[0].coords == std::make_tuple(4, 1));
        CHECK(f.carts[0].direction == Cart::Down);
        CHECK(f.carts[1].coords == std::make_tuple(11, 4));
        CHECK(f.carts[1].direction == Cart::Right);


        CHECK(simulateStep(f).empty());
        // /---\         //
        // |   |  /----\ //
        // | /->--+-\  | //
        // | | |  | |  | //
        // \-+-/  \-+--^ //
        // \------/      //
        CHECK(f.carts[0].coords == std::make_tuple(4, 2));
        CHECK(f.carts[0].direction == Cart::Right);
        CHECK(f.carts[1].coords == std::make_tuple(12, 4));
        CHECK(f.carts[1].direction == Cart::Up);

        CHECK(simulateStep(f).empty());
        // /---\         //
        // |   |  /----\ //
        // | /-+>-+-\  | //
        // | | |  | |  ^ //
        // \-+-/  \-+--/ //
        // \------/      //
        CHECK(f.carts[0].coords == std::make_tuple(5, 2));
        CHECK(f.carts[0].direction == Cart::Right);
        CHECK(f.carts[1].coords == std::make_tuple(12, 3));
        CHECK(f.carts[1].direction == Cart::Up);

        CHECK(simulateStep(f).empty());
        // /---\         //
        // |   |  /----\ //
        // | /-+->+-\  ^ //
        // | | |  | |  | //
        // \-+-/  \-+--/ //
        // \------/      //
        CHECK(f.carts[0].coords == std::make_tuple(6, 2));
        CHECK(f.carts[0].direction == Cart::Right);
        CHECK(f.carts[1].coords == std::make_tuple(12, 2));
        CHECK(f.carts[1].direction == Cart::Up);

        CHECK(simulateStep(f).empty());
        // /---\         //
        // |   |  /----< //
        // | /-+-->-\  | //
        // | | |  | |  | //
        // \-+-/  \-+--/ //
        // \------/      //
        CHECK(f.carts[0].coords == std::make_tuple(7, 2));
        CHECK(f.carts[0].direction == Cart::Right);
        CHECK(f.carts[1].coords == std::make_tuple(12, 1));
        CHECK(f.carts[1].direction == Cart::Left);

        CHECK(simulateStep(f).empty());
        // /---\         //
        // |   |  /---<\ //
        // | /-+--+>\  | //
        // | | |  | |  | //
        // \-+-/  \-+--/ //
        // \------/      //
        CHECK(f.carts[0].cart_id == 2);  // carts will switch indices here due to different step eval order
        CHECK(f.carts[1].cart_id == 1);  // carts in upper rows are always processed before carts in lower rows
        CHECK(f.carts[1].coords == std::make_tuple(8, 2));
        CHECK(f.carts[1].direction == Cart::Right);
        CHECK(f.carts[0].coords == std::make_tuple(11, 1));
        CHECK(f.carts[0].direction == Cart::Left);

        CHECK(simulateStep(f).empty());
        // /---\         //
        // |   |  /--<-\ //
        // | /-+--+-v  | //
        // | | |  | |  | //
        // \-+-/  \-+--/ //
        // \------/      //
        CHECK(f.carts[1].coords == std::make_tuple(9, 2));
        CHECK(f.carts[1].direction == Cart::Down);
        CHECK(f.carts[0].coords == std::make_tuple(10, 1));
        CHECK(f.carts[0].direction == Cart::Left);

        CHECK(simulateStep(f).empty());
        // /---\         //
        // |   |  /-<--\ //
        // | /-+--+-\  | //
        // | | |  | v  | //
        // \-+-/  \-+--/ //
        // \------/      //
        CHECK(f.carts[1].coords == std::make_tuple(9, 3));
        CHECK(f.carts[1].direction == Cart::Down);
        CHECK(f.carts[0].coords == std::make_tuple(9, 1));
        CHECK(f.carts[0].direction == Cart::Left);

        CHECK(simulateStep(f).empty());
        // /---\         //
        // |   |  /<---\ //
        // | /-+--+-\  | //
        // | | |  | |  | //
        // \-+-/  \-<--/ //
        // \------/      //
        CHECK(f.carts[1].coords == std::make_tuple(9, 4));
        CHECK(f.carts[1].direction == Cart::Left);
        CHECK(f.carts[0].coords == std::make_tuple(8, 1));
        CHECK(f.carts[0].direction == Cart::Left);

        CHECK(simulateStep(f).empty());
        // /---\         //
        // |   |  v----\ //
        // | /-+--+-\  | //
        // | | |  | |  | //
        // \-+-/  \<+--/ //
        // \------/      //
        CHECK(f.carts[1].coords == std::make_tuple(8, 4));
        CHECK(f.carts[1].direction == Cart::Left);
        CHECK(f.carts[0].coords == std::make_tuple(7, 1));
        CHECK(f.carts[0].direction == Cart::Down);

        CHECK(simulateStep(f).empty());
        // /---\         //
        // |   |  /----\ //
        // | /-+--v-\  | //
        // | | |  | |  | //
        // \-+-/  ^-+--/ //
        // \------/      //
        CHECK(f.carts[1].coords == std::make_tuple(7, 4));
        CHECK(f.carts[1].direction == Cart::Up);
        CHECK(f.carts[0].coords == std::make_tuple(7, 2));
        CHECK(f.carts[0].direction == Cart::Down);

        auto crash = simulateStep(f);
        REQUIRE(crash.size() == 2);
        CHECK(crash[0].coords == crash[1].coords);
        CHECK(crash[0].coords == std::make_tuple(7, 3));
    }

    SECTION("Run to completion") {
        char const* input = R"(/>-<\  
|   |  
| /<+-\
| | | v
\>+</ |
  |   ^
  \<->/
)";
        auto field = parseInput(input);

        CHECK(runToCompletion(field).coords == std::make_tuple(6, 4));
    }
}
