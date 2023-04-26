#include "line.hxx"

int main(int, char**) {
  const rgb_color black = {0, 0, 0};
  const rgb_color white = {255, 255, 255};
  const rgb_color gray = {117, 117, 117};

  int32_t width = 640;
  int32_t height = 480;

  canvas image(width, height);

  line_render render(image);

  render.clear(gray);

  int amount_lines = 200;

  for (size_t i = 0; i < amount_lines; ++i) {
    // position start = position::generate(width, height);
    // position end = position::generate(width, height);
    // render.draw_line(start, end, color);
    auto result = position::generate(width, height, amount_lines, i);
    rgb_color color = rgb_color ::generate();

    render.draw_line(std::get<0>(result), std::get<1>(result), color);
  }
  image.save_image("../lines.ppm");
  return 0;
}
