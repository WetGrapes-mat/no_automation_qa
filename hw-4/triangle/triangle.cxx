#include "triangle.hxx"
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

void triangle_render::draw_triangle(pixels& vertexes, rgb_color color) {
  pixels lines_pixels = pixel_position_triangle(vertexes.at(0), vertexes.at(1), vertexes.at(2));

  for (auto pixel : lines_pixels) {
    set_pixel(pixel, color);
  }
}
