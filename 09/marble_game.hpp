#ifndef ADVENT_OF_CODE_09_MARBLE_GAME_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_09_MARBLE_GAME_HPP_INCLUDE_GUARD

#include <list>
#include <string_view>
#include <vector>

struct GameParameters{
    int64_t nPlayers;
    int64_t lastMarble;
};
GameParameters parseInput(std::string_view input);

class Game {
private:
    std::list<int64_t> m_field;
    GameParameters m_parameters;
    int64_t m_currentPlayer;
    std::vector<int64_t> m_playerScores;
    int64_t m_currentTurn;
    std::list<int64_t>::iterator m_currentMarble;
public:
    Game(GameParameters const& parameters);

    std::vector<int64_t> getField() const;

    std::vector<int64_t> getPlayerScores() const;

    int64_t getCurrentPlayer() const;

    void playTurn();

    int64_t getWinningScore() const;
};

int64_t playGame(GameParameters const& parameters);
#endif
