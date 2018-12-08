#ifndef ADVENT_OF_CODE_08_LICENSE_TREE_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_08_LICENSE_TREE_HPP_INCLUDE_GUARD

#include <string_view>
#include <vector>

std::vector<int> parseInput(std::string_view input);

struct Node {
    std::vector<Node> children;
    std::vector<int> metadata;
};

Node buildTree(std::vector<int> const& serialized_tree);

int metadataSum(Node const& tree);

int nodeValue(Node const& tree);

#endif
