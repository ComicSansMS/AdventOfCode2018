#include <marble_game.hpp>

#include <range/v3/core.hpp>

#include <algorithm>
#include <cassert>
#include <iterator>
#include <limits>
#include <numeric>
#include <regex>
#include <string>

GameParameters parseInput(std::string_view input)
{
    std::regex rx_line(R"(^(\d+) players; last marble is worth (\d+) points[\r\n]+$)");
    std::match_results<std::string_view::iterator> matches;
    std::regex_match(begin(input), end(input), matches, rx_line);

    GameParameters ret;
    ret.nPlayers = std::stoi(matches[1]);
    ret.lastMarble = std::stoi(matches[2]);
    return ret;
}

Game::Game(GameParameters const& parameters)
    :m_field(), m_parameters(parameters), m_currentPlayer(1), m_playerScores(parameters.nPlayers, 0), m_currentTurn(1)
{
    assert(m_parameters.nPlayers > 0);
    assert(m_parameters.lastMarble > 0);
    m_field.push_back(0);
    m_currentMarble = begin(m_field);
}

std::vector<int64_t> Game::getField() const
{
    std::vector<int64_t> ret;
    ret.reserve(m_field.size());
    ret.assign(begin(m_field), end(m_field));
    return ret;
}

std::vector<int64_t> Game::getPlayerScores() const
{
    return m_playerScores;
}

int64_t Game::getCurrentPlayer() const
{
    return m_currentPlayer;
}

void Game::playTurn()
{
    auto const insertion_point = [this](std::list<int64_t>::iterator it) {
        for(int i = 0; i < 2; ++i) {
            if(it == end(m_field)) { it = begin(m_field); }
            ++it;
        }
        return it;
    };
    auto const removal_point = [this](std::list<int64_t>::iterator it) {
        for(int i = 0; i < 7; ++i) {
            if(it == begin(m_field)) { it = end(m_field); }
            --it;
        }
        return it;
    };
    if(m_currentTurn % 23 != 0) {
        m_currentMarble = m_field.insert(insertion_point(m_currentMarble), m_currentTurn);
    } else {
        auto to_remove = removal_point(m_currentMarble);
        int64_t score = m_currentTurn + (*to_remove);
        m_currentMarble = m_field.erase(to_remove);
        if(m_currentMarble == end(m_field)) { m_currentMarble = begin(m_field); }
        m_playerScores[m_currentPlayer - 1] += score;
    }
    ++m_currentTurn;
    ++m_currentPlayer;
    if(m_currentPlayer > m_parameters.nPlayers) { m_currentPlayer = 1; }
}

int64_t Game::getWinningScore() const
{
    return *std::max_element(begin(m_playerScores), end(m_playerScores));
}

int64_t playGame(GameParameters const& parameters)
{
    Game g(parameters);
    for(int64_t i = 0; i < parameters.lastMarble; ++i) {
        g.playTurn();
    }
    return g.getWinningScore();
}
