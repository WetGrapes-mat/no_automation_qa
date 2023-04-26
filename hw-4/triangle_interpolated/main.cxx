#include "triangle_interpolated.hxx"

int main(int, char**) {
  const rgb_color black = {0, 0, 0};

  constexpr size_t width = 640;
  constexpr size_t height = 480;

  canvas image(width, height);

  triangle_interpolated interpolated_render(image);

  struct program : gfx_program {
      vertex vertex_shader(const vertex& v_in) override {
        vertex out = v_in;

        double alpha = 3.14159 / 6;
        double x = out.x;
        double y = out.y;
        out.x = x * std::cos(alpha) - y * std::sin(alpha);
        out.y = x * std::sin(alpha) + y * std::cos(alpha);

        out.x *= 0.3;
        out.y *= 0.3;

        out.x += (width / 2.0);
        out.y += (height / 2.0);

        return out;
      }
      rgb_color fragment_shader(const vertex& v_in) override {
        rgb_color out;
        out.red = static_cast<uint8_t>(v_in.r * 255);
        out.green = static_cast<uint8_t>(v_in.g * 255);
        out.blue = static_cast<uint8_t>(v_in.b * 255);
        return out;
      }
  } program01;

  interpolated_render.clear(black);
  interpolated_render.set_gfx_program(program01);

  std::vector<vertex> triangle_v {
    {  0,   0, 20, 0, 0,   0,   0, 0},
    {  0, 239,  0, 1, 0,  13, 239, 0},
    {319, 239,  0, 0, 1, 319, 239, 0}
  };

  interpolated_render.draw_triangle(triangle_v);

  image.save_image("../interpolated.ppm");
}