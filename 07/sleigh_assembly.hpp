#ifndef ADVENT_OF_CODE_07_SLEIGH_ASSEMBLY_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_07_SLEIGH_ASSEMBLY_HPP_INCLUDE_GUARD

#include <array>
#include <bitset>
#include <string_view>
#include <utility>
#include <tuple>
#include <vector>

struct Edge {
    char from;
    char to;
};

inline bool operator==(Edge const& lhs, Edge const& rhs) {
    return std::tie(lhs.from, lhs.to) == std::tie(rhs.from, rhs.to);
}

std::vector<Edge> parseInput(std::string_view input);

class Graph {
private:
    std::array<std::bitset<26>, 26> m_adjacency;
    std::array<int, 26> m_counts;
public:
    Graph(std::vector<Edge> const& edges);

    bool hasEdge(char from, char to) const;

    int incomingEdgeCount(char node) const;

    std::string topologicalSort() const;
};

#endif
