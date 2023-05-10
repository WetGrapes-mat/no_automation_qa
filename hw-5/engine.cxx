#include "engine.hxx"

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_video.h>
#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <cmath>
#include <exception>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <vector>

#include <SDL3/SDL.h>

#include "glad/glad.h"

#define CHECK_OPENGL()                                                  \
  {                                                                     \
    const int err = static_cast<int>(glGetError());                     \
    if (err != GL_NO_ERROR) {                                           \
      switch (err) {                                                    \
        case GL_INVALID_ENUM:                                           \
          std::cerr << "GL_INVALID_ENUM" << std::endl;                  \
          break;                                                        \
        case GL_INVALID_VALUE:                                          \
          std::cerr << "GL_INVALID_VALUE" << std::endl;                 \
          break;                                                        \
        case GL_INVALID_OPERATION:                                      \
          std::cerr << "GL_INVALID_OPERATION" << std::endl;             \
          break;                                                        \
        case GL_INVALID_FRAMEBUFFER_OPERATION:                          \
          std::cerr << "GL_INVALID_FRAMEBUFFER_OPERATION" << std::endl; \
          break;                                                        \
        case GL_OUT_OF_MEMORY:                                          \
          std::cerr << "GL_OUT_OF_MEMORY" << std::endl;                 \
          break;                                                        \
      }                                                                 \
      assert(false);                                                    \
    }                                                                   \
  }

namespace grp {

static std::array<std::string_view, 17> event_names = {
  {/// input events
"left_pressed", "left_released",
   "right_pressed", "right_released",
   "up_pressed", "up_released",
   "down_pressed", "down_released",
   "select_pressed", "select_released",
   "start_pressed", "start_released",
   "button1_pressed", "button1_released",
   "button2_pressed", "button2_released",
   /// virtual console events
   "turn_off"}
};

std::ostream& operator<<(std::ostream& stream, const event e) {
  std::uint32_t value = static_cast<std::uint32_t>(e);
  std::uint32_t minimal = static_cast<std::uint32_t>(event::left_pressed);
  std::uint32_t maximal = static_cast<std::uint32_t>(event::turn_off);
  if (value >= minimal && value <= maximal) {
    stream << event_names[value];
    return stream;
  } else {
    throw std::runtime_error("too big event value");
  }
}

static std::ostream& operator<<(std::ostream& out, const SDL_version& v) {
  out << static_cast<int>(v.major) << '.';
  out << static_cast<int>(v.minor) << '.';
  out << static_cast<int>(v.patch);
  return out;
}

std::istream& operator>>(std::istream& is, vertex& v) {
  is >> v.x;
  is >> v.y;
  is >> v.z;
  return is;
}

struct bind {
    SDL_Keycode key;
    std::string_view name;
    event event_pressed;
    event event_released;
};

const std::array<bind, 8> keys {
  {{SDLK_w, "up", event::up_pressed, event::up_released},
   {SDLK_a, "left", event::left_pressed, event::left_released},
   {SDLK_s, "down", event::down_pressed, event::down_released},
   {SDLK_d, "right", event::right_pressed, event::right_released},
   {SDLK_LCTRL, "button1", event::button1_pressed, event::button1_released},
   {SDLK_SPACE, "button2", event::button2_pressed, event::button2_released},
   {SDLK_ESCAPE, "select", event::select_pressed, event::select_released},
   {SDLK_RETURN, "start", event::start_pressed, event::start_released}}
};

static bool check_input(const SDL_Event& e, const bind*& result) {
  using namespace std;

  const auto it =
    find_if(begin(keys), end(keys), [&](const bind& b) { return b.key == e.key.keysym.sym; });

  if (it != end(keys)) {
    result = &(*it);
    return true;
  }
  return false;
}

class engine_impl final : public engine {
  public:
    std::string initialize(std::string_view /*config*/) final {
      using namespace std;

      stringstream serr;

      const int init_result = SDL_Init(SDL_INIT_EVERYTHING);
      if (init_result != 0) {
        const char* err_message = SDL_GetError();
        serr << "error: failed call SDL_Init: " << err_message << endl;
        return serr.str();
      }

      SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

      window = SDL_CreateWindow("title", 640, 480, ::SDL_WINDOW_OPENGL);

      if (window == nullptr) {
        const char* err_message = SDL_GetError();
        serr << "error: failed call SDL_CreateWindow: " << err_message << endl;
        SDL_Quit();
        return serr.str();
      }
      int gl_major_ver = 4;
      int gl_minor_ver = 1;
      int gl_context_profile = SDL_GL_CONTEXT_PROFILE_CORE;

      SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, gl_context_profile);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, gl_major_ver);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, gl_minor_ver);

      SDL_GLContext gl_context = SDL_GL_CreateContext(window);

      assert(gl_context != nullptr);

      std::clog << "OpenGl " << gl_major_ver << '.' << gl_minor_ver << '\n';

      auto load_gl_pointer = [](const char* function_name) {
        SDL_FunctionPointer function_ptr = SDL_GL_GetProcAddress(function_name);
        return reinterpret_cast<void*>(function_ptr);
      };
      if (gladLoadGLLoader(load_gl_pointer) == 0) {
        std::clog << "error: failed to initialize glad" << std::endl;
      }

      // create vertex shader

      GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
      CHECK_OPENGL()
      string_view vertex_shader_src = R"(
                                #version 410 core
                                in vec3 a_position;
                                out vec4 v_position;

                                void main()
                                {
                                    v_position = vec4(a_position, 1.0);
                                    gl_Position = v_position;
                                }
                                )";
      // string_view vertex_shader_src = R"(
      //                               #version 410 core
      //                               layout(location = 0) in vec4 a_position;
      //                               layout(location = 1) in vec4 a_color;
      //                               out vec4 v_color;

      //                               void main() {
      //                                   v_color = a_color;
      //                                   gl_Position = a_position;
      //                               }
      //                               )";
      const char* source = vertex_shader_src.data();
      glShaderSource(vert_shader, 1, &source, nullptr);
      CHECK_OPENGL()

      glCompileShader(vert_shader);
      CHECK_OPENGL()

      // create fragment shader

      GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
      CHECK_OPENGL()
      // string_view fragment_shader_src = R"(
      //             #version 410 core
      //             precision mediump float;

      //             in vec4 v_position;

      //             out vec4 frag_color;

      //             // try main_one function name on linux mesa drivers
      //             void main()
      //             {
      //                 if (v_position.z >= 0.0)
      //                 {
      //                     float light_green = 0.5 + v_position.z / 2.0;
      //                     frag_color = vec4(0.0, light_green, 0.0, 1.0);
      //                 } else
      //                 {
      //                     float color = 0.5 - (v_position.z / -2.0);
      //                     frag_color = vec4(color, 0.0, 0.0, 1.0);
      //                 }
      //             }
      //             )";
      string_view fragment_shader_src = R"(
                      #version 410 core
                  precision mediump float;

                  in vec4 v_position;

                  out vec4 frag_color;


                      void main() {
                          frag_color = v_position;
                      }
                      )";
      source = fragment_shader_src.data();
      glShaderSource(fragment_shader, 1, &source, nullptr);
      CHECK_OPENGL()

      glCompileShader(fragment_shader);
      CHECK_OPENGL()

      // now create program and attach vertex and fragment shaders

      program_id_ = glCreateProgram();
      CHECK_OPENGL()
      if (0 == program_id_) {
        serr << "failed to create gl program";
        return serr.str();
      }

      glAttachShader(program_id_, vert_shader);
      CHECK_OPENGL()
      glAttachShader(program_id_, fragment_shader);
      CHECK_OPENGL()

      // bind attribute location
      glBindAttribLocation(program_id_, 0, "a_position");
      CHECK_OPENGL()

      // glBindAttribLocation(program_id_, 1, "a_color");
      // CHECK_OPENGL()
      // link program after binding attribute locations
      glLinkProgram(program_id_);
      CHECK_OPENGL()

      glGenBuffers(1, &m_vbo);
      CHECK_OPENGL()

      glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
      CHECK_OPENGL()

      glGenVertexArrays(1, &m_vao);
      CHECK_OPENGL()
      glBindVertexArray(m_vao);
      CHECK_OPENGL()

      // turn on rendering with just created shader program
      glUseProgram(program_id_);
      CHECK_OPENGL()

      glEnableVertexAttribArray(0);
      CHECK_OPENGL()

      glEnableVertexAttribArray(1);
      CHECK_OPENGL()

      glEnable(GL_DEPTH_TEST);
      // glDisable(GL_DEPTH_TEST);

      return "";
    }
    /// pool event from input queue
    /// return true if more events in queue
    bool read_input(event& e) final {
      using namespace std;
      // collect all events from SDL
      SDL_Event sdl_event;
      if (SDL_PollEvent(&sdl_event)) {
        const bind* binding = nullptr;

        if (sdl_event.type == SDL_EVENT_QUIT) {
          e = event::turn_off;
          return true;
        } else if (sdl_event.type == SDL_EVENT_KEY_DOWN) {
          if (check_input(sdl_event, binding)) {
            e = binding->event_pressed;
            return true;
          }
        } else if (sdl_event.type == SDL_EVENT_KEY_UP) {
          if (check_input(sdl_event, binding)) {
            e = binding->event_released;
            return true;
          }
        }
      }
      return false;
    }
    void render_triangle(const triangle& t) final {
      triangle out = t;
      using namespace std;
      chrono::steady_clock clock;
      auto time_point = clock.now();
      auto time_since_epoch = time_point.time_since_epoch();
      auto ns = time_since_epoch.count();
      auto seconds = ns / 1000'000'000.f;
      double alpha = std::sin(seconds) * 0.5;
      out.vertices[0].x = out.vertices[0].x * std::cos(alpha) - out.vertices[0].y * std::sin(alpha);
      out.vertices[0].y = out.vertices[0].x * std::sin(alpha) + out.vertices[0].y * std::cos(alpha);
      out.vertices[1].x = out.vertices[1].x * std::cos(alpha) - out.vertices[1].y * std::sin(alpha);
      out.vertices[1].y = out.vertices[1].x * std::sin(alpha) + out.vertices[1].y * std::cos(alpha);
      out.vertices[2].x = out.vertices[2].x * std::cos(alpha) - out.vertices[2].y * std::sin(alpha);
      out.vertices[2].y = out.vertices[2].x * std::sin(alpha) + out.vertices[2].y * std::cos(alpha);

      glBufferData(GL_ARRAY_BUFFER, sizeof(out.vertices), out.vertices.data(), GL_STATIC_DRAW);
      CHECK_OPENGL()
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*>(0));
      CHECK_OPENGL()

      glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*>(3 * sizeof(float)));

      CHECK_OPENGL()

      glDrawArrays(GL_TRIANGLES, 0, 3);
      CHECK_OPENGL()
    }
    void swap_buffers() final {
      SDL_GL_SwapWindow(window);
      using namespace std;
      chrono::steady_clock clock;
      auto time_point = clock.now();
      auto time_since_epoch = time_point.time_since_epoch();
      auto ns = time_since_epoch.count();
      auto seconds = ns / 1000'000'000.0f;
      auto current_color = 0.5f * (std::sin(seconds) + 1.0f);
      glClearColor(current_color, 0.f, 1.f - current_color, 0.0f);
      CHECK_OPENGL()
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      CHECK_OPENGL()
    }
    void uninitialize() final {}

  private:
    SDL_Window* window = nullptr;
    SDL_GLContext gl_context = nullptr;
    GLuint program_id_ = 0;
    GLuint m_vbo {};
    GLuint m_vao {};
};

static bool already_exist = false;

engine* create_engine() {
  if (already_exist) {
    throw std::runtime_error("engine already exist");
  }
  engine* result = new engine_impl();
  already_exist = true;
  return result;
}

void destroy_engine(engine* e) {
  if (already_exist == false) {
    throw std::runtime_error("engine not created");
  }
  if (nullptr == e) {
    throw std::runtime_error("e is nullptr");
  }
  delete e;
}

engine::~engine() {}
} // end namespace grp
