#include <SDL3/SDL.h>
#include <filesystem>
#include <iostream>
#include <map>
#include <thread>
#include <tuple>
#include <vector>

#include "engine.hxx"

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

grp::game* reload_game(grp::game* old,
                       const char* library_name,
                       const char* tmp_library_name,
                       grp::engine& engine,
                       void*& old_handle);

int main() {
  const char* library_name = "./libgame-lib.dylib";
  const char* tmp_library_file = "./temp.dylib";

  grp::engine* engin = grp::create_engine();

  void* game_lib_handle = nullptr;
  grp::game* current_game =
    reload_game(nullptr, library_name, tmp_library_file, *engin, game_lib_handle);
  auto time = std::filesystem::last_write_time(library_name);

  bool main_loop = true;
  while (main_loop) {
    auto last_changed = std::filesystem::last_write_time(library_name);
    if (last_changed != time) {
      current_game =
        reload_game(current_game, library_name, tmp_library_file, *engin, game_lib_handle);
      time = last_changed;
      if (!current_game) {
        std::cerr << "Unable to reload game!" << std::endl;
        using namespace std::chrono;
        std::this_thread::sleep_for(milliseconds(5000));
        continue;
      }
    }
    current_game->update();
    current_game->render();
  }

  grp::destroy_engine(engin);
  return EXIT_SUCCESS;
}

grp::game* reload_game(grp::game* old,
                       const char* library_name,
                       const char* tmp_library_name,
                       grp::engine& engine,
                       void*& old_handle) {
  using namespace std::filesystem;

  if (old) {
    SDL_UnloadObject(old_handle);
  }

  if (std::filesystem::exists(tmp_library_name)) {
    remove(tmp_library_name);
  }

  copy(library_name, tmp_library_name);

  void* game_handle = SDL_LoadObject(tmp_library_name);

  if (game_handle == nullptr) {
    SDL_Log("Error when load library handle: %s", SDL_GetError());
    return nullptr;
  }

  old_handle = game_handle;

  SDL_FunctionPointer create_game_func_ptr = SDL_LoadFunction(game_handle, "create_game");

  using create_game_ptr = decltype(&create_game);

  auto create_game_func = reinterpret_cast<create_game_ptr>(create_game_func_ptr);

  grp::game* game = create_game_func(&engine);

  if (game == nullptr) {
    SDL_Log("Error when create game: %s", SDL_GetError());
    return nullptr;
  }
  return game;
}
