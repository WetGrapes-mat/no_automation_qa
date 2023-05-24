#include <algorithm>
#include <array>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <string_view>

#include "engine.hxx"

#include "glm/glm.hpp"

grp::triangle get_transformed_triangle(const grp::triangle& t,
                                       const glm::mediump_mat3& result_matrix) {
  grp::triangle result {t};

  std::for_each(result.vertices.begin(), result.vertices.end(), [&](grp::vertex& v) {
    glm::vec3 v_pos_source {v.x, v.y, 1.f};
    glm::vec3 v_pos_result = result_matrix * v_pos_source;

    v.x = v_pos_result[0];
    v.y = v_pos_result[1];
  });

  return result;
}

int main(int /*argc*/, char* /*argv*/[]) {
  std::unique_ptr<grp::engine, void (*)(grp::engine*)> engine(grp::create_engine(),
                                                              grp::destroy_engine);
  int screen_width = 640;
  int screen_height = 480;

  engine->initialize("");

  std::unique_ptr<grp::texture> tank {engine->create_texture("./fly.png")};

  grp::triangle triangle_low {
    {-0.3f, -0.3f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.3f, -0.3f, 0.f, 0.f,
     0.f, 0.f, 1.f, 1.f, -0.3f, 0.3f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f},
  };

  grp::triangle triangle_high {
    {-0.3f, 0.3f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.3f, 0.3f, 0.f, 0.f,
     0.f, 0.f, 1.f, 0.f, 0.3f, -0.3f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f},
  };

  glm::vec3 worm_pos {0.f, 0.f, 1.f};
  glm::vec2 worm_scale {1.f, 1.f};

  constexpr float pi {3.14159};
  float worm_direction {0.f};
  float speed_x {0.01f}, speed_y {0.01f};
  //++++++++++++++++++main loop++++++++++++++++++++
  bool continue_loop = true;
  while (continue_loop) {
    grp::event event;

    while (engine->read_input(event)) {
      std::cout << event << std::endl;
      if (event == grp::event::left_pressed) {
        worm_direction = 0.f;
        worm_pos[0] -= speed_x;
      } else if (event == grp::event::right_pressed) {
        worm_direction = 0.f;
        worm_pos[0] += speed_x;
      } else if (event == grp::event::scalep_pressed) {
        worm_scale[0] += 0.1;
        worm_scale[1] += 0.1;
      } else if (event == grp::event::scalem_pressed) {
        worm_scale[0] -= 0.1;
        worm_scale[1] -= 0.1;
      } else if (event == grp::event::up_pressed) {
        worm_direction = 0.f;
        worm_pos[1] += speed_y;
      } else if (event == grp::event::down_pressed) {
        worm_direction = pi / 2.f;
        worm_pos[1] -= speed_y;
      } else if (event == grp::event::rotatel_pressed) {
        worm_direction += 0.1f;
      } else if (event == grp::event::rotater_pressed) {
        worm_direction -= 0.1f;
      } else if (event == grp::event::turn_off) {
        continue_loop = false;
        break;
      }
    }

    const glm::mediump_mat3x3 aspect_matrix {
      1.f, 0.f, 0.f, 0.f, static_cast<float>(screen_width) / screen_height, 0.f, 0.f, 0.f, 1.f};

    const glm::mediump_mat3x3 scale_matrix {
      worm_scale[0], 0.f, 0.f, 0.f, worm_scale[1], 0.f, 0.f, 0.f, 1.f};

    const glm::mediump_mat3 rotation_matrix {std::cos(worm_direction),
                                             std::sin(worm_direction),
                                             0.f,
                                             -std::sin(worm_direction),
                                             std::cos(worm_direction),
                                             0.f,
                                             0.f,
                                             0.f,
                                             1.f};

    const glm::mediump_mat3x3 move_matrix {
      1.f, 0.f, 0.f, 0.f, 1.f, 0.f, worm_pos[0], worm_pos[1], 1.f};

    const glm::mediump_mat3 result_matrix =
      aspect_matrix * move_matrix * scale_matrix * rotation_matrix;

    grp::triangle triangle_low_transformed = get_transformed_triangle(triangle_low, result_matrix);
    grp::triangle triangle_high_transformed =
      get_transformed_triangle(triangle_high, result_matrix);

    engine->render(triangle_low_transformed, tank.get());
    engine->render(triangle_high_transformed, tank.get());

    engine->swap_buffers();
  }

  engine->uninitialize();

  return EXIT_SUCCESS;
}
