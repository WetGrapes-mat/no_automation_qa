#include "engine.hxx"

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_video.h>
#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <cmath>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <vector>

#include "SDL3/SDL_keycode.h"
#include "glad/include/glad.h"
#include <SDL3/SDL.h>

#include "picopng.hxx"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
//+++++++++++++++++++++++++++++++EVENT+++++++++++++++++++++++++++++++

static std::array<std::string_view, 17> event_names = {
  {/// input events
"left_pressed", "left_released",
   "right_pressed", "right_released",
   "up_pressed", "up_released",
   "down_pressed", "down_released",
   "rotatel_pressed", "rotatel_released",
   "rotater_pressed", "rotater_released",
   "scalep_pressed", "scalep_released",
   "scalem_pressed", "scalem_released",
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
  {
   {SDLK_w, "up", event::up_pressed, event::up_released},
   {SDLK_a, "left", event::left_pressed, event::left_released},
   {SDLK_s, "down", event::down_pressed, event::down_released},
   {SDLK_d, "right", event::right_pressed, event::right_released},
   {SDLK_q, "right_rotate", event::rotater_pressed, event::rotater_released},
   {SDLK_e, "left_rotete", event::rotatel_pressed, event::rotatel_released},
   {SDLK_z, "scale-", event::scalem_pressed, event::scalem_released},
   {SDLK_c, "scale+", event::scalep_pressed, event::scalep_released},

   }
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

//+++++++++++++++++++++++++++++++TEXTURE+++++++++++++++++++++++++++++++
class opengl_texture : public texture {
  public:
    void bind() {
      glBindTexture(GL_TEXTURE_2D, gl_texture_id);
      CHECK_OPENGL()
    }

    void load(const std::string_view path) {
      std::vector<unsigned char> png_file_in_memory;
      std::ifstream ifs(path.data(), std::ios_base::binary);
      if (!ifs) {
        throw std::runtime_error("can't load texture");
      }
      ifs.seekg(0, std::ios_base::end);
      std::streamoff pos_in_file = ifs.tellg();
      png_file_in_memory.resize(static_cast<size_t>(pos_in_file));
      ifs.seekg(0, std::ios_base::beg);
      if (!ifs) {
        throw std::runtime_error("can't load texture");
      }
      ifs.read(reinterpret_cast<char*>(png_file_in_memory.data()), pos_in_file);

      if (!ifs.good()) {
        throw std::runtime_error("can't load texture");
      }

      std::vector<unsigned char> image;

      int error = decodePNG(image,
                            gl_texture_width,
                            gl_texture_height,
                            &png_file_in_memory[0],
                            png_file_in_memory.size(),
                            false);
      if (error != 0) {
        std::cerr << "error: " << error << std::endl;
        throw std::runtime_error("can't load texture");
      }
      std::cout << sizeof(image) << std::endl;

      glGenTextures(1, &gl_texture_id);
      CHECK_OPENGL()
      glBindTexture(GL_TEXTURE_2D, gl_texture_id);
      CHECK_OPENGL();

      GLint mipmap_level = 0;
      GLint border = 0;
      GLsizei width_ = static_cast<GLsizei>(gl_texture_width);
      GLsizei height_ = static_cast<GLsizei>(gl_texture_height);
      glTexImage2D(GL_TEXTURE_2D,
                   mipmap_level,
                   GL_RGBA,
                   width_,
                   height_,
                   border,
                   GL_RGBA,
                   GL_UNSIGNED_BYTE,
                   &image[0]);

      CHECK_OPENGL()

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      CHECK_OPENGL()
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      CHECK_OPENGL()
    }

  private:
    GLuint gl_texture_id = {};
    unsigned long gl_texture_width = 0;
    unsigned long gl_texture_height = 0;
};

//+++++++++++++++++++++++++++++++ENGINE+++++++++++++++++++++++++++++++

class engine_impl final : public engine {
  public:
    glm::mat3 matrix {};
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
      glGenBuffers(1, &vbo);
      CHECK_OPENGL()

      glBindBuffer(GL_ARRAY_BUFFER, vbo);
      CHECK_OPENGL()

      glGenVertexArrays(1, &vao);
      CHECK_OPENGL()
      glBindVertexArray(vao);
      CHECK_OPENGL()

      triangle_program.load_shader(GL_VERTEX_SHADER, "./triangle_vertex.shader");
      triangle_program.load_shader(GL_FRAGMENT_SHADER, "./triangle_fragment.shader");
      triangle_program.prepare_program();
      texture_program.load_shader(GL_VERTEX_SHADER, "./texture_vertex.shader");
      texture_program.load_shader(GL_FRAGMENT_SHADER, "./texture_fragment.shader");
      texture_program.prepare_program();

      glEnable(GL_BLEND);
      CHECK_OPENGL()
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      CHECK_OPENGL()

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

    bool is_key_down(const enum event key) final {
      const auto it =
        std::find_if(begin(keys), end(keys), [&](const bind& b) { return b.event_pressed == key; });

      if (it != end(keys)) {
        const std::uint8_t* state = SDL_GetKeyboardState(nullptr);
        int sdl_scan_code = SDL_GetScancodeFromKey(it->key);
        return state[sdl_scan_code];
      }
      return false;
    }
    void render(const triangle& t) final {
      glEnableVertexAttribArray(0);
      CHECK_OPENGL()

      glEnableVertexAttribArray(1);
      CHECK_OPENGL()

      triangle_program.apply_shader_program();

      glBufferData(GL_ARRAY_BUFFER, sizeof(t.vertices), t.vertices.data(), GL_STATIC_DRAW);
      CHECK_OPENGL()

      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*>(0));
      CHECK_OPENGL()

      glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*>(3 * sizeof(float)));

      CHECK_OPENGL()

      glDrawArrays(GL_TRIANGLES, 0, 3);
      CHECK_OPENGL()

      glDisableVertexAttribArray(0);
      CHECK_OPENGL()

      glDisableVertexAttribArray(1);
      CHECK_OPENGL()
    }

    void render(const triangle& triangle, texture* const texture) {
      glEnableVertexAttribArray(0);
      CHECK_OPENGL()

      glEnableVertexAttribArray(1);
      CHECK_OPENGL()

      glEnableVertexAttribArray(2);
      CHECK_OPENGL()

      texture_program.apply_shader_program();
      texture->bind();

      glBufferData(
        GL_ARRAY_BUFFER, sizeof(triangle.vertices), triangle.vertices.data(), GL_STATIC_DRAW);
      CHECK_OPENGL()

      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*>(0));
      CHECK_OPENGL()

      glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*>(3 * sizeof(float)));
      CHECK_OPENGL()

      glVertexAttribPointer(
        2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*>(6 * sizeof(float)));
      CHECK_OPENGL()

      glDrawArrays(GL_TRIANGLES, 0, 3);
      CHECK_OPENGL()

      glDisableVertexAttribArray(0);
      CHECK_OPENGL()

      glDisableVertexAttribArray(1);
      CHECK_OPENGL()

      glDisableVertexAttribArray(2);
      CHECK_OPENGL()
    }

    void swap_buffers() final {
      SDL_GL_SwapWindow(window);
      glClearColor(1.f, 1.f, 1.f, 0.0f);
      CHECK_OPENGL()
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      CHECK_OPENGL()
    }
    void uninitialize() final {}

    texture* create_texture(std::string_view path) {
      texture* r = new opengl_texture();
      r->load(path);
      std::cout << r << std::endl;
      return new opengl_texture();
    }

  private:
    SDL_Window* window = nullptr;
    SDL_GLContext gl_context = nullptr;
    GLuint vbo {};
    GLuint vao {};
    opengl_shader_program triangle_program {};
    opengl_shader_program texture_program {};
};

//+++++++++++++++++++++++++++++++SHADER_PROGRAM+++++++++++++++++++++++++++++++

opengl_shader_program::~opengl_shader_program() {
  std::for_each(m_shaders.begin(), m_shaders.end(), [](const GLuint shader_id) {
    glDeleteShader(shader_id);
    CHECK_OPENGL();
  });

  glDeleteProgram(m_program);
  CHECK_OPENGL();
}

void opengl_shader_program::load_shader(const GLenum shader_type,
                                        const std::string_view shader_path) {
  GLuint shader_id = glCreateShader(shader_type);
  CHECK_OPENGL();

  const std::string shader_code_string = get_shader_code_from_file(shader_path);
  const char* shader_code = shader_code_string.data();

  glShaderSource(shader_id, 1, &shader_code, nullptr);
  CHECK_OPENGL();

  glCompileShader(shader_id);
  CHECK_OPENGL();

  GLint shader_compiled {};
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &shader_compiled);
  CHECK_OPENGL();
  if (!shader_compiled) {
    GLint log_length {};
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);
    CHECK_OPENGL();

    if (log_length > 1) {
      std::string log {};
      log.resize(log_length);
      glGetShaderInfoLog(shader_id, log_length, nullptr, log.data());
      CHECK_OPENGL();
    }

    glDeleteShader(shader_id);
    CHECK_OPENGL();
    throw std::runtime_error {"Error on compiling shader"};
  }

  m_shaders.push_back(shader_id);
}

void opengl_shader_program::apply_shader_program() {
  glUseProgram(m_program);
  CHECK_OPENGL();
}

void opengl_shader_program::prepare_program() {
  attach_shaders();
  link_program();
  validate_program();
}

void opengl_shader_program::attach_shaders() {
  m_program = glCreateProgram();
  CHECK_OPENGL();

  std::for_each(m_shaders.begin(), m_shaders.end(), [this](const GLuint shader_id) {
    glAttachShader(m_program, shader_id);
    CHECK_OPENGL();
  });
}

void opengl_shader_program::link_program() const {
  glLinkProgram(m_program);
  CHECK_OPENGL();

  GLint linked {};
  glGetProgramiv(m_program, GL_LINK_STATUS, &linked);
  CHECK_OPENGL();

  if (!linked) {
    GLint log_length {};
    glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &log_length);
    CHECK_OPENGL();

    if (log_length > 1) {
      std::string log {};
      log.resize(log_length);
      glGetProgramInfoLog(m_program, log_length, nullptr, log.data());
      CHECK_OPENGL();
    }

    glDeleteProgram(m_program);
    CHECK_OPENGL();
    throw std::runtime_error {"Error on linking shader program"};
  }
}

void opengl_shader_program::validate_program() const {
  glValidateProgram(m_program);
  CHECK_OPENGL();

  GLint is_validated {};
  glGetProgramiv(m_program, GL_VALIDATE_STATUS, &is_validated);
  CHECK_OPENGL();
  std::cout << is_validated << std::endl;

  if (!is_validated) {
    GLint log_length {};
    glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &log_length);
    CHECK_OPENGL();

    if (log_length > 1) {
      std::string log {};
      log.resize(log_length);
      glGetProgramInfoLog(m_program, log_length, nullptr, log.data());
      CHECK_OPENGL();
    }

    glDeleteProgram(m_program);
    throw std::runtime_error {"Error on validating shader program"};
  }
}

std::string opengl_shader_program::get_shader_code_from_file(const std::string_view path) const {
  std::string shader_code {};

  std::ifstream file {path.data()};

  const std::filesystem::path fs_path {path.data()};

  std::streamsize bytes_to_read = std::filesystem::file_size(fs_path);

  shader_code.resize(bytes_to_read);

  file.read(shader_code.data(), bytes_to_read);

  file.close();
  return shader_code;
}

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