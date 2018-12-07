#include <sleigh_assembly.hpp>

#include <range/v3/core.hpp>

#include <algorithm>
#include <cassert>
#include <iterator>
#include <numeric>
#include <ostream>
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

std::string Graph::topologicalSort() const
{
    auto adj = m_adjacency;
    auto counts = m_counts;
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
