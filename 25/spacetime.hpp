#ifndef ADVENT_OF_CODE_25_SPACETIME_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_25_SPACETIME_HPP_INCLUDE_GUARD

#include <string_view>
#include <vector>

struct Vec4 {
    int x;
    int y;
    int z;
    int t;

    Vec4() = default;
    Vec4(int xx, int yy, int zz, int tt);
};

bool operator==(Vec4 const& lhs, Vec4 const& rhs);

std::vector<Vec4> parseInput(std::string_view input);

int distance(Vec4 const& lhs, Vec4 const& rhs);

std::vector<std::vector<Vec4>> cluster(std::vector<Vec4> const& vs);

#endif
