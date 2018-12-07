#include <sleigh_assembly.hpp>

#include <catch.hpp>

TEST_CASE("Sleigh Assembly")
{
    char const* sample_input = R"(Step C must be finished before step A can begin.
Step C must be finished before step F can begin.
Step A must be finished before step B can begin.
Step A must be finished before step D can begin.
Step B must be finished before step E can begin.
Step D must be finished before step E can begin.
Step F must be finished before step E can begin.)";

    SECTION("Parse Input")
    {
        CHECK(parseInput(sample_input) == std::vector<Edge>{{'C', 'A'}, {'C', 'F'}, {'A', 'B'}, {'A', 'D'},
                                                            {'B', 'E'}, {'D', 'E'}, {'F', 'E'}});
    }

    SECTION("Graph from edges") {
        auto const edges = parseInput(sample_input);
        Graph g(edges);
        CHECK(g.hasEdge('C', 'A'));
        CHECK(g.hasEdge('C', 'F'));
        CHECK(g.hasEdge('A', 'B'));
        CHECK(g.hasEdge('A', 'D'));
        CHECK(g.hasEdge('B', 'E'));
        CHECK(g.hasEdge('D', 'E'));
        CHECK(g.hasEdge('F', 'E'));
        CHECK(!g.hasEdge('A', 'F'));
        CHECK(!g.hasEdge('B', 'A'));
        CHECK(!g.hasEdge('A', 'A'));

        CHECK(g.incomingEdgeCount('A') == 1);
        CHECK(g.incomingEdgeCount('B') == 1);
        CHECK(g.incomingEdgeCount('C') == 0);
        CHECK(g.incomingEdgeCount('D') == 1);
        CHECK(g.incomingEdgeCount('E') == 3);
        CHECK(g.incomingEdgeCount('F') == 1);
    }

    SECTION("Topological Sort")
    {
        Graph g1(parseInput(sample_input));
        CHECK(g1.topologicalSort() == "CABDFEGHIJKLMNOPQRSTUVWXYZ");

        Graph g2(std::vector<Edge>{{'E', 'B'}, {'E', 'C'}, {'B', 'D'}, {'B', 'A'}, {'C', 'D'}, {'D', 'A'}});
        CHECK(g2.topologicalSort() == "EBCDAFGHIJKLMNOPQRSTUVWXYZ");
    }
}
