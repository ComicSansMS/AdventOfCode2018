#ifndef ADVENT_OF_CODE_03_RECTANGLE_COVER_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_03_RECTANGLE_COVER_HPP_INCLUDE_GUARD

#include <ostream>
#include <string_view>
#include <vector>

struct Rectangle {
    int left;
    int top;
    int width;
    int height;

    Rectangle(int l, int t, int w, int h);
};

bool operator==(Rectangle const& lhs, Rectangle const& rhs);

std::ostream& operator<<(std::ostream& os, Rectangle const& r);

std::vector<Rectangle> parseInput(std::string_view input);

int overlap_1d(int start1, int extent1, int start2, int extent2);

int overlap(Rectangle const& lhs, Rectangle const& rhs);

int total_overlap(std::vector<Rectangle> const& rects);

Rectangle getFieldDimensions(std::vector<Rectangle> const& rects);

class Field {
    friend std::ostream& operator<<(std::ostream& os, Field const& f);
private:
    std::vector<int> m_field;
    Rectangle m_dimensions;
public:
    explicit Field(Rectangle const& dimensions);

    Rectangle getDimensions() const;

    int getCell(int x, int y) const;

    void placeRectangle(Rectangle const& r);

    int getOverlap() const;

private:
    int getCellIndex(int x, int y) const;
};

std::ostream& operator<<(std::ostream& os, Field const& f);

#endif
