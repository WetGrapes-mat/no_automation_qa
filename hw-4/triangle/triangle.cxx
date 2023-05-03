#include "triangle.hxx"
#include <iostream>
#include <utility>

triangle_render::triangle_render(canvas& buffer) : line_render(buffer) {}

pixels triangle_render::pixel_position_triangle(position& v1, position& v2, position& v3) {
  pixels pixel_pos;
  for (auto [start, end] : {
         std::pair {v1, v2},
          std::pair {v2, v3},
          std::pair {v3, v1}
  }) {
    pixels p = pixels_positions(start, end);
    pixel_pos.insert(pixel_pos.end(), p.begin(), p.end());
  }
  return pixel_pos;
}

void triangle_render::draw_triangles(pixels& vertexes, size_t num_vertices, rgb_color col) {
  for (size_t i = 0; i < num_vertices / 3; i++) {
    position v1 = vertexes.at(i * 3 + 0);
    position v2 = vertexes.at(i * 3 + 1);
    position v3 = vertexes.at(i * 3 + 2);
    draw_triangle(v1, v2, v3, col);
  }
};

void triangle_render::draw_triangle(position v1, position v2, position v3, rgb_color color) {
  pixels lines_pixels = pixel_position_triangle(v1, v2, v3);

  for (auto pixel : lines_pixels) {
    set_pixel(pixel, color);
  }
}
