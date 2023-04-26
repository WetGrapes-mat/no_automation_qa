#pragma once
#include "../canvas/canvas.hxx"

#include <cmath>
#include <cstdint>
#include <tuple>

struct position {
    double length();
    friend position operator-(const position& left, const position& right);
    friend bool operator==(const position& left, const position& right);
    int x = 0;
    int y = 0;

    static position generate(const int width, const int height);
    static std::tuple<position, position> generate(const int width,
                                                   const int height,
                                                   const int amount,
                                                   const int serial);
};

using pixels = std::vector<position>;

class irender {
  public:
    virtual void clear(rgb_color) = 0;
    virtual void set_pixel(position, rgb_color) = 0;
    virtual pixels pixels_positions(position start, position end) = 0;

    virtual ~irender();
};