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

// cache the row sums for each examined square to avoid reiterating rows all the times
// turns out this doesn't pay off and is actually slower than recomputing the row sums
std::tuple<int, int, int> getLargestSquare_impl_row_lookup(int size, std::vector<int> const& grid, std::vector<int>& rows)
{
    int max_power = std::numeric_limits<int>::min();
    int max_x = -1;
    int max_y = -1;
    int current_row_index = 0;
    int const limit_outer_loop = (300 - size + 1);
    for(int x = 1; x < limit_outer_loop; ++x) {
        int power = 0;
        for(int y = 1; y < limit_outer_loop; ++y) {
            if(y == 1) {
                current_row_index = 0;
                int const limit_inner_x = x+size;
                for(int inner_x = x; inner_x < limit_inner_x; ++inner_x) {
                    rows[current_row_index] = 0;
                    int const limit_inner_y = y+size;
                    for(int inner_y = y; inner_y < limit_inner_y; ++inner_y) {
                        assert((inner_x >= 1) && (inner_x <= 300));
                        assert((inner_y >= 1) && (inner_y <= 300));
                        rows[current_row_index] += grid[(inner_y - 1)* 300 + (inner_x - 1)];
                    }
                    power += rows[current_row_index];
                    ++current_row_index;
                }
                current_row_index = 0;
            } else {
                // remove upper row
                power -= rows[current_row_index];
                rows[current_row_index] = 0;
                int const limit_xx = x + size;
                for(int xx = x; xx < limit_xx; ++xx) {
                    // add lower row
                    rows[current_row_index] += grid[((y + size) - 2) * 300 + (xx - 1)];
                }
                power += rows[current_row_index];
                ++current_row_index;
                if(current_row_index == size) { current_row_index = 0; }
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

std::tuple<int, int, int> getLargestSquare_impl(int size, std::vector<int> const& grid)
{
    int max_power = std::numeric_limits<int>::min();
    int max_x = -1;
    int max_y = -1;
    for(int x : ranges::iota_view(1, (300 - size) + 1)) {
        int power = 0;
        for(int y : ranges::iota_view(1, (300 - size) + 1)) {
            if(y == 1) {
                for(int inner_y : ranges::iota_view(y, y + size)) {
                    for(int inner_x : ranges::iota_view(x, x + size)) {
                        assert((inner_x >= 1) && (inner_x <= 300));
                        assert((inner_y >= 1) && (inner_y <= 300));
                        power += grid[(inner_y - 1)* 300 + (inner_x - 1)];
                    }
                }
            } else {
                for(int xx : ranges::iota_view(x, x+size)) {
                    // remove upper row
                    power -= grid[(y - 2) * 300 + (xx - 1)];
                    // add lower row
                    power += grid[((y + size) - 2) * 300 + (xx - 1)];
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

// here we pass an additional grid that contains the partial sum for each row
// this allows direct calculation of the row sums of the examined squares without iterating
// this is quite fast already; could be made faster by caching sums of whole squares as well,
// but this is good enough for now
std::tuple<int, int, int> getLargestSquare_impl_row_sums(int size, std::vector<int> const& grid,
                                                         std::vector<int> const& row_sums)
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
    return getLargestSquare_impl_row_sums(size, grid, calculateRowSums(grid));
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
        auto const [power, x, y] = getLargestSquare_impl_row_sums(sq_size, grid, row_sums);
        if(power > max_power) {
            max_power = power;
            max_x = x;
            max_y = y;
            max_size = sq_size;
        }
    }
    return std::make_tuple(max_power, max_x, max_y, max_size);
}
