#pragma once

#include "../line/line.hxx"

class triangle_render : public line_render {
  public:
    triangle_render(canvas& buffer);

    pixels pixel_position_triangle(position& v1, position& v2, position& v3);
    void draw_triangle(pixels& vertex, rgb_color color);
};