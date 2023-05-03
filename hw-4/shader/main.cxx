#include "triangle_interpolated.hxx"
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <algorithm>
#include <cmath>
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
        // double distanceA =
        //   std::sqrt(std::pow(v_in.x - uniform.v1.x, 2) + std::pow(v_in.y - uniform.v1.y, 2));
        // double distanceB =
        //   std::sqrt(std::pow(v_in.x - uniform.v2.x, 2) + std::pow(v_in.y - uniform.v2.y, 2));
        // double distanceC =
        //   std::sqrt(std::pow(v_in.x - uniform.v3.x, 2) + std::pow(v_in.y - uniform.v3.y, 2));
        // double n_x, n_y;
        // if (distanceA <= distanceB && distanceA <= distanceC) {
        //   n_x = uniform.v1.x;
        //   n_y = uniform.v1.y;
        // } else if (distanceB <= distanceA && distanceB <= distanceC) {
        //   n_x = uniform.v2.x;
        //   n_y = uniform.v2.y;
        // } else {
        //   n_x = uniform.v3.x;
        //   n_y = uniform.v3.y;
        // }
        // vertex nn;
        // if (n_x == uniform.v1.x && n_y == uniform.v1.y) {
        //   nn = (uniform.v2.x < uniform.v3.x) ? uniform.v2 : uniform.v3;
        // } else if (n_x == uniform.v2.x && n_y == uniform.v2.y) {
        //   nn = (uniform.v1.x < uniform.v3.x) ? uniform.v1 : uniform.v3;
        // } else {
        //   nn = (uniform.v1.x < uniform.v2.x) ? uniform.v1 : uniform.v2;
        // }
        // double direction = std::atan2(nn.y - out.y, nn.x - out.x);
        // double distance = std::sqrt(std::pow(nn.x - out.x, 2) + std::pow(nn.y - out.y, 2));
        // if (distance <= 0.05) {
        //   out.x = nn.x;
        //   out.y = nn.y;
        //   return out;
        // }
        // out.x = out.x + 0.05 * uniform.t * std::cos(direction);
        // out.y = out.y + 0.05 * uniform.t * std::sin(direction);
        // return out;

        double delta_x = std::cos(uniform.t) * std::sin(uniform.t + out.x);
        double delta_y = std::sin(uniform.t) * std::sin(uniform.t + out.y);

        out.x += delta_x * 30;
        out.y += delta_y * 30;

        size_t c_x = 320;
        size_t c_y = 240;
        double angle = std::atan2(out.y - c_y, out.x - c_x);

        double distance = std::sqrt(std::pow(out.x - c_x, 2) + std::pow(out.y - c_y, 2));
        double a = angle + 2 * M_PI * 0.05 * uniform.t;

        out.x = c_x + distance * cos(a);
        out.y = c_y + distance * sin(a);

        auto d_x {640 / 2 - out.x};
        auto d_y {480 / 2 - out.y};

        d_x *= uniform.s;
        d_y *= uniform.s;

        out.x = 640 / 2 - d_x;
        out.y = 480 / 2 - d_y;

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

  std::vector<vertex> triangle_v {
  //  центр
    {260, 240, 223, 240,  38},
    {380, 171,  55, 240,  38},
    {380, 309, 218,  36, 224},
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
  };

  interpolated_render.draw_triangles(triangle_v, 3);

  image.save_image("../interpolated.ppm");

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
    interpolated_render.draw_triangles(triangle_v, 3);

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