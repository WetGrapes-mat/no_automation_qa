#include "triangle_interpolated.hxx"
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>

int main(int, char**) {
  constexpr size_t width = 640;
  constexpr size_t height = 480;

  canvas image(width, height);

  triangle_interpolated interpolated_render(image);

  struct program : gfx_program {
      uniforms uniform;
      void set_uniforms(const uniforms& u) override {
        uniform = u;
      }
      vertex vertex_shader(const vertex& v_in) override {
        vertex out = v_in;

        double delta_x = std::cos(uniform.t) * std::sin(uniform.t + out.x) * 20;
        double delta_y = std::sin(uniform.t) * std::sin(uniform.t + out.y) * 20;

        out.x += delta_x;
        out.y += delta_y;

        size_t c_x = 320;
        size_t c_y = 240;
        double angle = std::atan2(out.y - c_y, out.x - c_x);

        double distance = std::sqrt(std::pow(out.x - c_x, 2) + std::pow(out.y - c_y, 2));
        double a = angle + 2 * M_PI * 0.05 * uniform.t;

        out.x = c_x + distance * cos(a);
        out.y = c_y + distance * sin(a);

        double d_x = c_x - out.x;
        double d_y = c_y - out.y;

        d_x *= uniform.s;
        d_y *= uniform.s;

        out.x = c_x - d_x;
        out.y = c_y - d_y;

        out.r *= cos(uniform.c);
        out.g *= cos(uniform.c);
        out.b *= sin(uniform.c);

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

  void* pixels = image.get_pixels().data();
  const int depth = sizeof(rgb_color) * 8;
  const int pitch = width * sizeof(rgb_color);
  interpolated_render.clear({0, 0, 0});
  interpolated_render.set_gfx_program(program01);

  double cx = 320.0;
  double cy = 240.0;
  double r = 100.0;
  int n = 21;
  double step = 2.0 * M_PI / n;
  std::vector<vertex> triangle_v;

  for (int i = 0; i < n; ++i) {
    double angle = i * step;
    double x = cx + r * std::cos(angle);
    double y = cy + r * std::sin(angle);
    triangle_v.push_back({
      x,
      y,
      static_cast<double>(std::rand() % 256),
      static_cast<double>(std::rand() % 256),
      static_cast<double>(std::rand() % 256),
    });
  }

  // std::vector<vertex> triangle_v {
  //  центр
  // {260, 240, 223, 240,  38},
  // {380, 171,  55, 240,  38},
  // {380, 309, 218,  36, 224},
  // верх
  //     {260, 102, 223, 240,  38},
  //     {380,  33, 223, 240,  38},
  //     {380, 171, 223, 240,  38},
  //  //  низ
  //     {260, 378,  55, 240,  38},
  //     {380, 309,  55, 240,  38},
  //     {380, 447,  55, 240,  38},
  //  //  правый
  //     {500, 240, 240, 134,  36},
  //     {380, 171, 240, 134,  36},
  //     {380, 309, 240, 134,  36},
  //  //  левый
  //     {260, 240, 218,  36, 224},
  //     {140, 171, 218,  36, 224},
  //     {140, 309, 218,  36, 224}
  // };

  // interpolated_render.draw_triangles(triangle_v, n);

  // image.save_image("../interpolated.ppm");

  SDL_Window* window = SDL_CreateWindow("triangle", width, height, SDL_WINDOW_OPENGL);
  if (window == nullptr) {
    std::cerr << SDL_GetError() << std::endl;
    return EXIT_FAILURE;
  }
  SDL_Renderer* renderer = SDL_CreateRenderer(window, "opengl", SDL_RENDERER_ACCELERATED);
  if (renderer == nullptr) {
    std::cerr << SDL_GetError() << std::endl;
    return EXIT_FAILURE;
  }

  interpolated_render.set_gfx_program(reinterpret_cast<gfx_program&>(program01));
  bool continue_loop = true;
  double size = 0;
  double color = 0;
  SDL_Event e;
  while (continue_loop) {
    double time_from_start = SDL_GetTicks() / 1000.0;

    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_EVENT_QUIT) {
        continue_loop = false;
        break;
      }
      if (e.type == SDL_EVENT_KEY_DOWN) {
        if (e.key.keysym.sym == SDLK_w) {
          size += 0.05;
        }
        if (e.key.keysym.sym == SDLK_s) {
          size -= 0.05;
        }
        if (e.key.keysym.sym == SDLK_d) {
          color += 1;
        }
        if (e.key.keysym.sym == SDLK_a) {
          color -= 1;
        }
      }
    }
    interpolated_render.clear({0, 0, 0});
    program01.set_uniforms(
      uniforms {time_from_start, size, color, triangle_v[0], triangle_v[1], triangle_v[2]});
    interpolated_render.draw_triangles(triangle_v, n);

    SDL_Surface* bitmapSurface =
      SDL_CreateSurfaceFrom(pixels, width, height, pitch, SDL_PIXELFORMAT_RGB24);

    SDL_Texture* bitmapTex = SDL_CreateTextureFromSurface(renderer, bitmapSurface);

    SDL_DestroySurface(bitmapSurface);

    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, bitmapTex, nullptr, nullptr);
    SDL_RenderPresent(renderer);

    SDL_DestroyTexture(bitmapTex);
  }
}