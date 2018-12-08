#include <license_tree.hpp>

#include <catch.hpp>

TEST_CASE("License Tree")
{
    char const* sample_input = "2 3 0 3 10 11 12 1 1 0 1 99 2 1 1 2";
    //                          A----------------------------------
    //                              B----------- C-----------
    //                                               D-----

    SECTION("Parse Input")
    {
        CHECK(parseInput(sample_input) == std::vector<int>{2, 3, 0, 3, 10, 11, 12, 1, 1, 0, 1, 99, 2, 1, 1, 2});
    }

    SECTION("Build Tree")
    {
        auto tree = buildTree(parseInput(sample_input));
        // Node A
        REQUIRE(tree.children.size() == 2);
        CHECK(tree.metadata == std::vector<int>{1, 1, 2});
        // Node B
        REQUIRE(tree.children[0].children.size() == 0);
        CHECK(tree.children[0].metadata == std::vector<int>{10, 11, 12});
        // Node C
        REQUIRE(tree.children[1].children.size() == 1);
        CHECK(tree.children[1].metadata == std::vector<int>{2});
        // Node D
        REQUIRE(tree.children[1].children[0].children.size() == 0);
        CHECK(tree.children[1].children[0].metadata == std::vector<int>{99});
    }

    SECTION("Metadata Sum")
    {
        auto tree = buildTree(parseInput(sample_input));
        CHECK(metadataSum(tree) == 138);
    }
}
