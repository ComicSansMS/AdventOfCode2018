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
#include <vector>

int calculatePowerLevel(int x, int y, int grid_serial_number)
{
    int const rack_id = x + 10;
    int const power_level = (((((rack_id * y) + grid_serial_number) * rack_id) % 1000) / 100) - 5;
    return power_level;
}

namespace {
std::vector<int> precomputeGrid(int grid_serial_number)
{
    std::vector<int> v;
    v.reserve(300 * 300);
    for(int y = 0; y < 300; ++y) {
        for(int x = 0; x < 300; ++x) {
            v.push_back(calculatePowerLevel(x + 1, y + 1, grid_serial_number));
        }
    }
    return v;
}

// here we pass an additional grid that contains the partial sum for each row
// this allows direct calculation of the row sums of the examined squares without iterating
// this is quite fast already; could be made faster by caching sums of whole squares as well,
// but this is good enough for now
std::tuple<int, int, int> getLargestSquare_impl_row_sums(int size, std::vector<int> const& row_sums)
{
    int max_power = std::numeric_limits<int>::min();
    int max_x = -1;
    int max_y = -1;
    int const limit_outer_loop = (300 - size + 1);
    for(int x = 1; x < limit_outer_loop; ++x) {
        int power = 0;
        for(int y = 1; y < limit_outer_loop; ++y) {
            if(y == 1) {
                int const limit_inner_y = y+size;
                for(int inner_y = y; inner_y < limit_inner_y; ++inner_y) {
                    power += row_sums[(inner_y - 1)* 300 + (x + size - 2)] -
                             ((x == 1) ? 0 : row_sums[(inner_y - 1)* 300 + (x - 2)]);
                }
            } else {
                // remove upper row
                power -= row_sums[(y - 2)* 300 + (x + size - 2)] -
                         ((x == 1) ? 0 : row_sums[(y - 2)* 300 + (x - 2)]);
                // add lower row
                power += row_sums[((y + size) - 2)* 300 + (x + size - 2)] -
                         ((x==1) ? 0 : row_sums[((y + size) - 2)* 300 + (x - 2)]);
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

std::vector<int> calculateRowSums(std::vector<int> const& grid)
{
    std::vector<int> row_sums;
    row_sums.reserve(300 * 300);
    for(int y : ranges::view::iota(0, 300)) {
        std::partial_sum(begin(grid) + y*300, begin(grid) + (y+1)*300, std::back_inserter(row_sums));
    }
    return row_sums;
}
}

std::tuple<int, int, int> getLargestSquare(int grid_serial_number, int size)
{
    std::vector<int> const grid = precomputeGrid(grid_serial_number);
    return getLargestSquare_impl_row_sums(size, calculateRowSums(grid));
}

std::tuple<int, int, int, int> getLargestOfAllSquares(int grid_serial_number)
{
    std::vector<int> const grid = precomputeGrid(grid_serial_number);
    int max_power = 0;
    int max_x = -1;
    int max_y = -1;
    int max_size = -1;
    std::vector<int> const row_sums = calculateRowSums(grid);
    for(int sq_size : ranges::iota_view(1, 301)) {
        auto const [power, x, y] = getLargestSquare_impl_row_sums(sq_size, row_sums);
        if(power > max_power) {
            max_power = power;
            max_x = x;
            max_y = y;
            max_size = sq_size;
        }
    }
    return std::make_tuple(max_power, max_x, max_y, max_size);
}
