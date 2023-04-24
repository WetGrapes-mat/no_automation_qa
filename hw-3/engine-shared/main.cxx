#include <iostream>

#include "engine.hxx"

int main() {
  using namespace grp;
  engine* eng = create_engine();
  eng->init();
  bool main_loop = true;
  while (main_loop) {
    event event;
    while (eng->input_event(event)) {
      std::cout << event << std::endl;
      if (event == event::turn_off) {
        main_loop = false;
        break;
      }
    }
  }
  eng->uninit();
  destroy_engine(eng);
  return EXIT_SUCCESS;
}