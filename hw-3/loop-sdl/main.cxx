#include <iostream>
#include <vector>

#include <SDL3/SDL.h>

int main() {
  using namespace std;
  const vector<pair<SDL_Keycode, string>> keys {
    {     SDLK_w,      "W"},
    {     SDLK_s,      "S"},
    {     SDLK_a,      "A"},
    {     SDLK_d,      "D"},
    { SDLK_SPACE,  "Space"},
    {SDLK_ESCAPE, "Escape"}
  };

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
    SDL_Log("SDL initialize error: %s", SDL_GetError());
    return EXIT_FAILURE;
  }

  SDL_Window* window = SDL_CreateWindow("My_SDL", 640, 480, ::SDL_WINDOW_OPENGL);

  if (!window) {
    SDL_Log("SDL Create window error: %s", SDL_GetError());
    SDL_Quit();
    return EXIT_FAILURE;
  }

  bool main_loop = true;
  while (main_loop) {
    SDL_Event sdl_event;
    while (SDL_PollEvent(&sdl_event)) {
      switch (sdl_event.type) {
        case SDL_EVENT_KEY_DOWN:
          for (int i = 0; i < keys.size(); i++) {
            if (keys[i].first == sdl_event.key.keysym.sym) {
              std::cout << keys[i].second << " is pressed\n";
              break;
            }
          }
          break;
        case SDL_EVENT_KEY_UP:
          for (int i = 0; i < keys.size(); i++) {
            if (keys[i].first == sdl_event.key.keysym.sym) {
              std::cout << keys[i].second << " is released\n";
              break;
            }
          }
          break;
        case SDL_EVENT_QUIT:
          main_loop = false;
          break;
        default:
          break;
      }
    }
  }
  SDL_DestroyWindow(window);
  SDL_Quit();
  return EXIT_SUCCESS;
}