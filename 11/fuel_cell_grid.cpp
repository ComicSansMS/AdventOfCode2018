#include <fuel_cell_grid.hpp>

#include <range/v3/core.hpp>
#include <range/v3/view/iota.hpp>

#include <algorithm>
#include <cassert>
#include <iterator>
#include <limits>
#include <numeric>
#include <string>
#include <unordered_set>

int calculatePowerLevel(int x, int y, int grid_serial_number)
{
    int const rack_id = x + 10;
    int const power_level = (((((rack_id * y) + grid_serial_number) * rack_id) % 1000) / 100) - 5;
    return power_level;
}

std::tuple<int, int, int> getLargestSquare(int grid_serial_number, int size)
{
    int max_power = 0;
    int max_x = -1;
    int max_y = -1;
    for(int x : ranges::iota_view(1, (300 - size) + 1)) {
        for(int y : ranges::iota_view(1, (300 - size) + 1)) {
            int power = 0;
            for(int inner_x : ranges::iota_view(x, x + size)) {
                for(int inner_y : ranges::iota_view(y, y + size)) {
                    assert((inner_x >= 1) && (inner_x <= 300));
                    assert((inner_y >= 1) && (inner_y <= 300));
                    power += calculatePowerLevel(inner_x, inner_y, grid_serial_number);
                }
            }
            if(power > max_power) {
                max_power = power;
                max_x = x;
                max_y = y;
            }
        }
    }
    return std::make_tuple(max_power, max_x, max_y);
}

std::tuple<int, int, int, int> getLargestOfAllSquares(int grid_serial_number)
{
    int max_power = 0;
    int max_x = -1;
    int max_y = -1;
    int max_size = -1;
    for(int sq_size : ranges::iota_view(1, 301)) {
        auto const [power, x, y] = getLargestSquare(grid_serial_number, sq_size);
        if(power > max_power) {
            max_power = power;
            max_x = x;
            max_y = y;
            max_size = sq_size;
        }
    }
    return std::make_tuple(max_power, max_x, max_y, max_size);
}
