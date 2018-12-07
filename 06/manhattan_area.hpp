#ifndef ADVENT_OF_CODE_06_MANHATTAN_AREA_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_06_MANHATTAN_AREA_HPP_INCLUDE_GUARD

#include <iosfwd>
#include <string_view>
#include <vector>

struct Coordinate {
    int x;
    int y;
};

bool operator==(Coordinate const& lhs, Coordinate const& rhs);

std::vector<Coordinate> parseInput(std::string_view input);

struct Rectangle {
    int left;
    int top;
    int width;
    int height;

    Rectangle(int l, int t, int w, int h);
};
bool operator==(Rectangle const& lhs, Rectangle const& rhs);

std::ostream& operator<<(std::ostream& os, Rectangle const& r);

Rectangle getFieldDimensions(std::vector<Coordinate> const& coordinates);

int getCellIndex(Rectangle const& dimensions, int x, int y);

int manhattanDistance(Coordinate c1, Coordinate c2);

struct FieldCell {
    int closestPoint;
    int distance;
};
std::vector<FieldCell> fillField(std::vector<Coordinate> points);

int findLargestArea(std::vector<FieldCell> const& field, std::vector<Coordinate> points);

int findLargestAreaFloodFill_SinglePoint(std::vector<Coordinate> const& points, int limit, Coordinate const& start_point);

int findLargestAreaFloodFill(std::vector<Coordinate> const& points, int limit);

#endif
