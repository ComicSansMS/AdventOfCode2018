#include <marble_game.hpp>

#include <catch.hpp>

TEST_CASE("License Tree")
{
    SECTION("Parse Input")
    {
        auto const game_parameters = parseInput("470 players; last marble is worth 72170 points");
        CHECK(game_parameters.nPlayers == 470);
        CHECK(game_parameters.lastMarble == 72170);
    }

    SECTION("Game Construction")
    {
        Game g({9, 25});

        CHECK(g.getField() == std::vector<int64_t>{0});
        CHECK(g.getPlayerScores() == std::vector<int64_t>(9));
        CHECK(g.getCurrentPlayer() == 1);
    }

    SECTION("Play Game Turn")
    {
        Game g({9, 25});

        g.playTurn();
        CHECK(g.getField() == std::vector<int64_t>{0, 1});
        CHECK(g.getPlayerScores() == std::vector<int64_t>(9));
        CHECK(g.getCurrentPlayer() == 2);

        g.playTurn();
        CHECK(g.getField() == std::vector<int64_t>{0, 2, 1});
        CHECK(g.getPlayerScores() == std::vector<int64_t>(9));
        CHECK(g.getCurrentPlayer() == 3);

        g.playTurn();
        CHECK(g.getField() == std::vector<int64_t>{0, 2, 1, 3});
        CHECK(g.getPlayerScores() == std::vector<int64_t>(9));
        CHECK(g.getCurrentPlayer() == 4);

        g.playTurn();
        CHECK(g.getField() == std::vector<int64_t>{0, 4, 2, 1, 3});
        CHECK(g.getPlayerScores() == std::vector<int64_t>(9));
        CHECK(g.getCurrentPlayer() == 5);

        g.playTurn();
        CHECK(g.getField() == std::vector<int64_t>{0, 4, 2, 5, 1, 3});
        CHECK(g.getPlayerScores() == std::vector<int64_t>(9));
        CHECK(g.getCurrentPlayer() == 6);
    }

    SECTION("Player wrap-around")
    {
        Game g({9, 25});

        for(int i = 0; i < 8; ++i) {
            g.playTurn();
        }
        CHECK(g.getField() == std::vector<int64_t>{0, 8, 4, 2, 5, 1, 6, 3, 7});
        CHECK(g.getPlayerScores() == std::vector<int64_t>(9));
        CHECK(g.getCurrentPlayer() == 9);

        g.playTurn();
        CHECK(g.getField() == std::vector<int64_t>{0, 8, 4, 9, 2, 5, 1, 6, 3, 7});
        CHECK(g.getPlayerScores() == std::vector<int64_t>(9));
        CHECK(g.getCurrentPlayer() == 1);
    }

    SECTION("Turn 23")
    {
        Game g({9, 25});

        for(int i = 0; i < 22; ++i) {
            g.playTurn();
        }
        CHECK(g.getField() == std::vector<int64_t>{0, 16, 8, 17, 4, 18, 9, 19, 2, 20, 10, 21,
                                                   5, 22, 11, 1, 12, 6, 13, 3, 14, 7, 15});
        CHECK(g.getPlayerScores() == std::vector<int64_t>(9));
        CHECK(g.getWinningScore() == 0);
        CHECK(g.getCurrentPlayer() == 5);

        g.playTurn();
        CHECK(g.getField() == std::vector<int64_t>{0, 16, 8, 17, 4, 18, 19, 2, 20, 10, 21,
                                                   5, 22, 11, 1, 12, 6, 13, 3, 14, 7, 15});
        CHECK(g.getPlayerScores() == std::vector<int64_t>{0, 0, 0, 0, 32, 0, 0, 0, 0});
        CHECK(g.getWinningScore() == 32);
        CHECK(g.getCurrentPlayer() == 6);

        g.playTurn();
        CHECK(g.getField() == std::vector<int64_t>{0, 16, 8, 17, 4, 18, 19, 2, 24, 20, 10, 21,
                                                   5, 22, 11, 1, 12, 6, 13, 3, 14, 7, 15});
        CHECK(g.getPlayerScores() == std::vector<int64_t>{0, 0, 0, 0, 32, 0, 0, 0, 0});
        CHECK(g.getCurrentPlayer() == 7);
    }

    SECTION("Play full game")
    {
        CHECK(playGame({9, 25}) == 32);
        CHECK(playGame({10, 1618}) == 8317);
        CHECK(playGame({13, 7999}) == 146373);
        CHECK(playGame({17, 1104}) == 2764);
        CHECK(playGame({21, 6111}) == 54718);
        CHECK(playGame({30, 5807}) == 37305);
    }
}
