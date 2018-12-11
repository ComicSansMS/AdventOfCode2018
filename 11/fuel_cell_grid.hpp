#ifndef ADVENT_OF_CODE_11_FUEL_CELL_GRID_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_11_FUEL_CELL_GRID_HPP_INCLUDE_GUARD

#include <tuple>

int calculatePowerLevel(int x, int y, int grid_serial_number);

std::tuple<int, int, int> getLargestSquare(int grid_serial_number, int size);

std::tuple<int, int, int, int> getLargestOfAllSquares(int grid_serial_number);

#endif
