#ifndef ADVENT_OF_CODE_09_MARBLE_GAME_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_09_MARBLE_GAME_HPP_INCLUDE_GUARD

#include <list>
#include <string_view>
#include <vector>

struct GameParameters{
    int nPlayers;
    int lastMarble;
};
GameParameters parseInput(std::string_view input);

class Game {
private:
    std::list<int> m_field;
    GameParameters m_parameters;
    int m_currentPlayer;
    std::vector<int> m_playerScores;
    int m_currentTurn;
    std::list<int>::iterator m_currentMarble;
public:
    Game(GameParameters const& parameters);

    std::vector<int> getField() const;

    std::vector<int> getPlayerScores() const;

    int getCurrentPlayer() const;

    void playTurn();

    int getWinningScore() const;
};

int playGame(GameParameters const& parameters);
#endif
