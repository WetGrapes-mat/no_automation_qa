#pragma once

#include "glad/glad.h"

//
#include <string>
#include <string_view>
#include <vector>

///////////////////////////////////////////////////////////////////////////////

namespace grp {

///////////////////////////////////////////////////////////////////////////////

class opengl_shader_program final {
  public:
    opengl_shader_program() = default;
    ~opengl_shader_program();
    opengl_shader_program(const opengl_shader_program&) = delete;
    opengl_shader_program(opengl_shader_program&&) = delete;
    opengl_shader_program& operator=(const opengl_shader_program&) = delete;
    opengl_shader_program& operator=(opengl_shader_program&&) = delete;

    void load_shader(const GLenum shader_type, const std::string_view shader_path);

    // Attach all shaders, compile, link and validate program.
    // Then return program id.
    GLuint get_prepared_program();

  private:
    // IMPORTANT NOTE: it's assumed that the user specifies attribute
    // location directly in the shader source file by using `location`
    // layout qualifier. Otherwise opengl app will not work.
    void attach_shaders();
    void link_program() const;
    void validate_program() const;

    std::string get_shader_code_from_file(const std::string_view path) const;

    // All shader ids.
    std::vector<GLuint> m_shaders {};
    GLuint m_program {};
};

///////////////////////////////////////////////////////////////////////////////

} // namespace grp

  ///////////////////////////////////////////////////////////////////////////////