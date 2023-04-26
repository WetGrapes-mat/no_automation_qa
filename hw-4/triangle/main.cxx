#pragma once
#include "triangle.hxx"

int main() {
  int32_t width = 640;
  int32_t height = 480;

  canvas image(width, height);

  triangle_render render(image);

  pixels vertexes;
  vertexes.push_back(position::generate(width, height));
  vertexes.push_back(position::generate(width, height));
  vertexes.push_back(position::generate(width, height));

  pixels vertexes1;
  vertexes1.push_back(position::generate(width, height));
  vertexes1.push_back(position::generate(width, height));
  vertexes1.push_back(position::generate(width, height));

  pixels vertexes2;
  vertexes2.push_back(position::generate(width, height));
  vertexes2.push_back(position::generate(width, height));
  vertexes2.push_back(position::generate(width, height));

  render.draw_triangle(vertexes, color_green);
  render.draw_triangle(vertexes1, color_blue);
  render.draw_triangle(vertexes2, color_red);

  image.save_image("../triangle.ppm");

  return EXIT_SUCCESS;
}