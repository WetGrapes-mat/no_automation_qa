#include "engine.hxx"

#include <SDL3/SDL.h>
#include <iostream>
#include <map>
#include <tuple>
#include <vector>

namespace grp {
using namespace std;
SDL_Window* window = nullptr;
const vector<tuple<SDL_Keycode, string, event, event>> keys {
  {     SDLK_w,      "w",      event::w_pressed,      event::w_released},
  {     SDLK_s,      "s",      event::s_pressed,      event::s_released},
  {     SDLK_a,      "a",      event::a_pressed,      event::a_released},
  {     SDLK_d,      "d",      event::d_pressed,      event::d_released},
  { SDLK_SPACE,  "space",  event::space_pressed,  event::space_released},
  {SDLK_ESCAPE, "escape", event::escape_pressed, event::escape_released}
};
const map<event, std::string> event_names {
  {      event::w_pressed,       "w_pressed"},
  {     event::w_released,      "w_released"},
  {      event::s_pressed,       "s_pressed"},
  {     event::s_released,      "s_released"},
  {      event::d_pressed,       "d_pressed"},
  {     event::d_released,      "d_released"},
  {      event::a_pressed,       "a_pressed"},
  {     event::a_released,      "a_released"},
  {  event::space_pressed,   "space_pressed"},
  { event::space_released,  "space_released"},
  { event::escape_pressed,  "escape_pressed"},
  {event::escape_released, "escape_released"},
  {       event::turn_off,           "close"}
};

std::ostream& operator<<(std::ostream& out, event e) {
  out << event_names.at(e);
  return out;
}

void engine::init() {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
    SDL_Log("SDL initialize error: %s", SDL_GetError());
  }

  window = SDL_CreateWindow("My_SDL", 640, 480, ::SDL_WINDOW_OPENGL);

  if (!window) {
    SDL_Log("SDL Create window error: %s", SDL_GetError());
    SDL_Quit();
  }
}
void engine::uninit() {
  if (window) {
    SDL_DestroyWindow(window);
    SDL_Quit();
  }
}

bool engine::input_event(event& e) {
  SDL_Event event;
  if (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_EVENT_QUIT:
        e = event::turn_off;
        return true;
        break;
      case SDL_EVENT_KEY_DOWN:
        for (int i = 0; i < keys.size(); i++) {
          if (std::get<0>(keys[i]) == event.key.keysym.sym) {
            e = std::get<2>(keys[i]);
            return true;
          }
        }
        break;
      case SDL_EVENT_KEY_UP:
        for (int i = 0; i < keys.size(); i++) {
          if (std::get<0>(keys[i]) == event.key.keysym.sym) {
            e = std::get<3>(keys[i]);
            return true;
          }
        }
        break;
    }
  }
  return false;
}
void engine::render() {}

static bool already_init = false;

engine* create_engine() {
  if (already_init) {
    throw std::runtime_error("engine already exist");
  }
  engine* result = new engine();
  already_init = true;
  return result;
}

void destroy_engine(engine* e) {
  if (!already_init) {
    throw std::runtime_error("engine not created");
  }
  if (e == nullptr) {
    throw std::runtime_error("e is nullptr");
  }
  delete e;
}

} // namespace grp