#include <sleigh_assembly.hpp>

#include <range/v3/core.hpp>

#include <algorithm>
#include <cassert>
#include <iterator>
#include <numeric>
#include <optional>
#include <regex>
#include <string>
#include <unordered_set>

std::vector<Edge> parseInput(std::string_view input)
{
    std::regex rx_line(R"(Step (\w) must be finished before step (\w) can begin.)");

    using regex_it = std::regex_iterator<std::string_view::iterator>;
    auto const it_begin = regex_it(begin(input), end(input), rx_line);
    auto const it_end = regex_it();

    std::vector<Edge> ret;
    std::transform(it_begin, it_end, std::back_inserter(ret),
        [](std::match_results<std::string_view::iterator> const& match) -> Edge {
        assert((match[1].first[0] >= 'A') && (match[1].first[0] <= 'Z'));
        assert((match[2].first[0] >= 'A') && (match[2].first[0] <= 'Z'));
            return Edge{match[1].first[0], match[2].first[0]};
        });
    return ret;
}

Graph::Graph(std::vector<Edge> const& edges)
{
    m_counts.fill(-1);
    for(auto const& e : edges) {
        assert(!m_adjacency[e.to - 'A'][e.from - 'A']);
        m_adjacency[e.to - 'A'][e.from - 'A'].flip();
    }
    for(int i=0; i<26; ++i) {
        m_counts[i] = static_cast<int>(m_adjacency[i].count());
    }
}

bool Graph::hasEdge(char from, char to) const
{
    assert((from >= 'A') && (from <= 'Z'));
    assert((to >= 'A') && (to <= 'Z'));

    return m_adjacency[to - 'A'][from - 'A'];
}

int Graph::incomingEdgeCount(char node) const
{
    assert((node >= 'A') && (node <= 'Z'));
    return m_counts[node - 'A'];
}

namespace
{
std::string topologicalSortImpl(std::array<std::bitset<26>, 26> adj, std::array<int, 26> counts)
{
    std::string ret;
    ret.reserve(26);
    while(ret.size() < 26) {
        for(int from = 0; from < 26; ++from) {
            if(counts[from] == 0) {
                ret.push_back(static_cast<char>('A' + from));
                counts[from] = -1;
                for(int to = 0; to < 26; ++to) {
                    if(adj[to].test(from)) {
                        assert(counts[to] > 0);
                        --counts[to];
                        adj[to].reset(from);
                    }
                }
                break;
            }
        }
    }
    return ret;
}
}

std::string Graph::topologicalSort() const
{
    return topologicalSortImpl(m_adjacency, m_counts);
}


namespace
{
std::tuple<int, std::string> assembleImpl(std::array<std::bitset<26>, 26> adj, std::array<int, 26> counts,
                                          int nWorkers, int timeBaseline, char finalLetter)
{
    std::string ret;
    ret.reserve(26);
    int timecount = 0;
    struct Worker {
        int timeLeft;
        int letter;
    };
    std::vector<Worker> workers(nWorkers, Worker{-1, -1});
    std::unordered_set<int> letters_worked_on;

    auto pick_letter = [&]() -> std::optional<int> {
        for(int from = 0; from < 26; ++from) {
            if((counts[from] == 0) && (letters_worked_on.find(from) == end(letters_worked_on))) {
                // found a candidate
                counts[from] = -1;
                letters_worked_on.insert(from);
                return from;
            }
        }
        return std::nullopt;
    };
    auto complete_letter = [&](int from) {
        assert(letters_worked_on.find(from) != end(letters_worked_on));
        for(int to = 0; to < 26; ++to) {
            if(adj[to].test(from)) {
                assert(counts[to] > 0);
                --counts[to];
                adj[to].reset(from);
            }
        }
        ret.push_back(static_cast<char>('A' + from));
        letters_worked_on.erase(from);
    };

    int const limit = (finalLetter - 'A') + 1;
    for(; ret.size() < limit; ++timecount) {
        for(auto& w : workers) {
            if(w.timeLeft > 0) {
                --w.timeLeft;
                if(w.timeLeft == 0) {
                    complete_letter(w.letter);
                    w.letter = -1;
                    w.timeLeft = -1;
                }
            }
            if(w.letter == -1) {
                if(auto next_letter = pick_letter(); next_letter && (*next_letter < limit)) {
                    w.letter = *next_letter;
                    w.timeLeft = *next_letter + timeBaseline + 1;
                }
            }
        }
    }
    assert(std::all_of(begin(workers), end(workers),
                       [](Worker const& w) { return (w.letter == -1) && (w.timeLeft == -1); }));
    return std::make_tuple(timecount - 1, ret);
}
}

std::tuple<int, std::string> Graph::assemble(int nWorkers, int timeBaseline, char finalLetter) const
{
    return assembleImpl(m_adjacency, m_counts, nWorkers, timeBaseline, finalLetter);
}
