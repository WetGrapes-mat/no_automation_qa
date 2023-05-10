#include <algorithm>
#include <array>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <string_view>

#include "engine.hxx"

int main(int /*argc*/, char* /*argv*/[]) {
  std::unique_ptr<grp::engine, void (*)(grp::engine*)> engine(grp::create_engine(),
                                                              grp::destroy_engine);

  engine->initialize("");

  bool continue_loop = true;
  while (continue_loop) {
    grp::event event;

    while (engine->read_input(event)) {
      std::cout << event << std::endl;
      switch (event) {
        case grp::event::turn_off:
          continue_loop = false;
          break;
        default:
          break;
      }
    }

    // std::ifstream file("/Users/matvejdubajlo/no_automation_qa/untitled folder/vertexes.txt");
    // assert(!!file);

    // grp::triangle tr;
    // file >> tr;
    grp::triangle t {
      {-0.5f, -0.5f, -0.8f, 0.5f, -0.5f, 0.5f, 0.0f, 0.8f, 1.f}
    };

    engine->render_triangle(t);

    engine->swap_buffers();
  }

  engine->uninitialize();

  return EXIT_SUCCESS;
}
