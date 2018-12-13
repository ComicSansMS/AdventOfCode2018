#ifndef ADVENT_OF_CODE_13_MINE_CART_MADNESS_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_13_MINE_CART_MADNESS_HPP_INCLUDE_GUARD

#include <string_view>
#include <tuple>
#include <vector>

struct Cart {
    std::tuple<int, int> coords;
    enum Direction_T { Up, Down, Left, Right } direction;
    int turn_counter;
    int cart_id;
};

struct Field {
    std::vector<char> tracks;
    int track_width;
    int track_height;
    std::vector<Cart> carts;
};

Field parseInput(std::string_view input);

std::vector<Cart> simulateStep(Field& f);

Cart runToCompletion(Field& f);

#endif
