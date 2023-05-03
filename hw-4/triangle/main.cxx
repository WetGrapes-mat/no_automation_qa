#pragma once
#include "triangle.hxx"

int main() {
  int32_t width = 640;
  int32_t height = 480;

  canvas image(width, height);

  triangle_render render(image);

  pixels vertexes;
  vertexes.reserve(9);
  vertexes.push_back(position(260, 240));
  vertexes.push_back(position(380, 171));
  vertexes.push_back(position(380, 309));

  vertexes.push_back(position(260, 102));
  vertexes.push_back(position(380, 33));
  vertexes.push_back(position(380, 171));

  vertexes.push_back(position(260, 378));
  vertexes.push_back(position(380, 309));
  vertexes.push_back(position(380, 447));

  vertexes.push_back(position(500, 240));
  vertexes.push_back(position(380, 171));
  vertexes.push_back(position(380, 309));

  vertexes.push_back(position(260, 240));
  vertexes.push_back(position(140, 171));
  vertexes.push_back(position(140, 309));

  render.draw_triangles(vertexes, 15, color_green);

  // render.draw_triangle(vertexes.at(0), vertexes.at(1), vertexes.at(2), color_green);
  // render.draw_triangle(vertexes1.at(0), vertexes1.at(1), vertexes1.at(2), color_blue);
  // render.draw_triangle(vertexes2.at(0), vertexes2.at(1), vertexes2.at(2), color_red);

  image.save_image("../triangle.ppm");

  return EXIT_SUCCESS;
}