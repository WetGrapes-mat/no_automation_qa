#include "canvas.hxx"

#include <iostream>

int main() {
  int width = 640;
  int height = 480;
  canvas image_generate(width, height);
  for (int i = 0; i < width; i++)
    for (int j = 0; j < height; j++)
      image_generate.set_pixel(i,
                               j,
                               {static_cast<uint8_t>(170 + (247 - 170) * i / width),
                                static_cast<uint8_t>(247 + (2 - 247) * i / width),
                                static_cast<uint8_t>(2 + (247 - 2) * i / width)});

  image_generate.save_image("../canvas.ppm");

  canvas image_load(0, 0);
  image_load.load_image("../canvas.ppm");

  return EXIT_SUCCESS;
}