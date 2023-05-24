#include "shader_program.hxx"

//
//
#include <algorithm>
#include <fstream>
#include <stdexcept>

///////////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////

opengl_shader_program::~opengl_shader_program() {
  std::for_each(m_shaders.begin(), m_shaders.end(), [](const GLuint shader_id) {
    glDeleteShader(shader_id);
    CHECK_OPENGL();
  });
}

void opengl_shader_program::load_shader(const GLenum shader_type,
                                        const std::string_view shader_path) {
  GLuint shader_id = glCreateShader(shader_type);
  CHECK_OPENGL();

  CHECK(shader_id) << "Error on creating " << shader_type << " shader";

  const std::string shader_code_string = get_shader_code_from_file(shader_path);
  const char* shader_code = shader_code_string.data();

  glShaderSource(shader_id, 1, &shader_code, nullptr);
  opengl_check();

  glCompileShader(shader_id);
  opengl_check();

  GLint shader_compiled {};
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &shader_compiled);
  opengl_check();

  if (!shader_compiled) {
    GLint log_length {};
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);
    opengl_check();

    if (log_length > 1) {
      std::string log {};
      log.resize(log_length);
      glGetShaderInfoLog(shader_id, log_length, nullptr, log.data());
      opengl_check();
      LOG(ERROR) << log;
    }

    glDeleteShader(shader_id);
    opengl_check();
    throw std::runtime_error {"Error on compiling shader"};
  }

  m_shaders.push_back(shader_id);
}

GLuint opengl_shader_program::get_prepared_program() {
  attach_shaders();
  link_program();
  validate_program();
  return m_program;
}

void opengl_shader_program::attach_shaders() {
  m_program = glCreateProgram();
  opengl_check();
  CHECK(m_program);

  std::for_each(m_shaders.begin(), m_shaders.end(), [this](const GLuint shader_id) {
    glAttachShader(m_program, shader_id);
    opengl_check();
  });
}

void opengl_shader_program::link_program() const {
  glLinkProgram(m_program);
  opengl_check();

  GLint linked {};
  glGetProgramiv(m_program, GL_LINK_STATUS, &linked);
  opengl_check();

  if (!linked) {
    GLint log_length {};
    glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &log_length);
    opengl_check();

    if (log_length > 1) {
      std::string log {};
      log.resize(log_length);
      glGetProgramInfoLog(m_program, log_length, nullptr, log.data());
      opengl_check();
      LOG(ERROR) << log;
    }

    glDeleteProgram(m_program);
    opengl_check();
    throw std::runtime_error {"Error on linking shader program"};
  }
}

void opengl_shader_program::validate_program() const {
  glValidateProgram(m_program);
  opengl_check();

  GLint is_validated {};
  glGetProgramiv(m_program, GL_VALIDATE_STATUS, &is_validated);
  opengl_check();

  if (!is_validated) {
    GLint log_length {};
    glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &log_length);
    opengl_check();

    if (log_length > 1) {
      std::string log {};
      log.resize(log_length);
      glGetProgramInfoLog(m_program, log_length, nullptr, log.data());
      opengl_check();
      LOG(ERROR) << log;
    }

    glDeleteProgram(m_program);
    throw std::runtime_error {"Error on validating shader program"};
  }
}

std::string opengl_shader_program::get_shader_code_from_file(const std::string_view path) const {
  std::string shader_code {};

  std::ifstream file {path.data()};

  CHECK(file.is_open()) << "Error on opening " << path;

  file.seekg(0, std::ios_base::end);
  CHECK(file.good()) << "Error on setting read position to the end of " << path << " file";

  std::streamsize bytes_to_read = file.tellg();
  CHECK(file.good()) << "tellg() function failed in " << path;
  CHECK(bytes_to_read);
  shader_code.resize(bytes_to_read);

  file.seekg(0, std::ios_base::beg);
  CHECK(file.good()) << "Error on setting read position to the begin of " << path << " file";

  file.read(shader_code.data(), bytes_to_read);
  CHECK(file.good()) << "Error on reading " << path << " file";

  file.close();
  CHECK(file.good()) << "Error on closing " << path << " file";

  return shader_code;
}

///////////////////////////////////////////////////////////////////////////////

} // namespace grp

  ///////////////////////////////////////////////////////////////////////////////