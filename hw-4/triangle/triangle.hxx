#pragma once

#include "../line/line.hxx"

class triangle_render : public line_render {
  public:
    triangle_render(canvas& buffer);

    pixels pixel_position_triangle(position& v1, position& v2, position& v3);
    void draw_triangle(position v1, position v2, position v3, rgb_color col);
    void draw_triangles(pixels& vertexes, size_t num_vertices, rgb_color col);
};