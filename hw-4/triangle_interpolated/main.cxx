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
        out.red = static_cast<uint8_t>(v_in.r);
        out.green = static_cast<uint8_t>(v_in.g);
        out.blue = static_cast<uint8_t>(v_in.b);
        return out;
      }
  } program01;

  interpolated_render.clear(black);
  interpolated_render.set_gfx_program(program01);

  std::vector<vertex> triangle_v {
    {260, 240, 255,   0, 0},
    {380, 171, 255,   0, 0},
    {380, 309, 255,   0, 0},
    {260, 102, 255,   0, 0},
    {380,  33, 255,   0, 0},
    {380, 171, 255,   0, 0},
    {260, 378, 255,   0, 0},
    {380, 309, 255,   0, 0},
    {380, 447, 255,   0, 0},
    {500, 240, 255,   0, 0},
    {380, 171, 255,   0, 0},
    {380, 309, 255,   0, 0},
    {260, 240,   0, 255, 0},
    {140, 171,   0, 255, 0},
    {140, 309,   0, 255, 0}
  };

  interpolated_render.draw_triangles(triangle_v, 15);

  image.save_image("../interpolated.ppm");
}