#include <license_tree.hpp>

#include <range/v3/core.hpp>

#include <algorithm>
#include <cassert>
#include <iterator>
#include <limits>
#include <numeric>
#include <regex>
#include <string>

std::vector<int> parseInput(std::string_view input)
{
    std::regex rx_line(R"((\d+))");

    using regex_it = std::regex_iterator<std::string_view::iterator>;
    auto const it_begin = regex_it(begin(input), end(input), rx_line);
    auto const it_end = regex_it();

    std::vector<int> ret;
    std::transform(it_begin, it_end, std::back_inserter(ret),
        [](std::match_results<std::string_view::iterator> const& match) -> int {
            return std::stoi(match[0]);
        });
    return ret;
}

namespace {
Node buildTree_rec(std::vector<int>::const_iterator& it, std::vector<int>::const_iterator it_end)
{
    int const nChildren = *it;
    ++it;
    int const nMetadata = *it;
    ++it;

    Node ret;
    ret.children.reserve(nChildren);
    ret.metadata.reserve(nMetadata);
    for(int i = 0; i < nChildren; ++i) {
        ret.children.emplace_back(buildTree_rec(it, it_end));
    }
    for(int i=0; i  < nMetadata; ++i) {
        ret.metadata.push_back(*it);
        ++it;
    }
    return ret;
}
}

Node buildTree(std::vector<int> const& serialized_tree)
{
    auto it = begin(serialized_tree);
    return buildTree_rec(it, end(serialized_tree));
}

int metadataSum(Node const& tree)
{
    std::vector<Node const*> stack;
    stack.push_back(&tree);
    int acc = 0;
    while(!stack.empty()) {
        Node const* current_node = stack.back();
        stack.pop_back();
        acc += std::accumulate(begin(current_node->metadata), end(current_node->metadata), 0);
        for(auto const& c : current_node->children) { stack.push_back(&c); }
    }
    return acc;
}

int nodeValue(Node const& tree)
{
    std::vector<Node const*> stack;
    stack.push_back(&tree);
    int acc = 0;
    while(!stack.empty()) {
        Node const* current_node = stack.back();
        assert(current_node->children.size() < std::numeric_limits<int>::max());
        stack.pop_back();
        if(current_node->children.empty()) {
            acc += std::accumulate(begin(current_node->metadata), end(current_node->metadata), 0);
        } else {
            for(auto const& m : current_node->metadata) {
                if((m > 0) && (m <= static_cast<int>(current_node->children.size()))) {
                    stack.push_back(&(current_node->children[m-1]));
                }
            }
        }
        
    }
    return acc;
}
