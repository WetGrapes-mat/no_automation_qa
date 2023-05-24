#include "glad/include/glad.h"
#include <array>
#include <iosfwd>
#include <string>
#include <string_view>
#include <vector>

namespace grp {
/// dendy gamepad emulation events
enum class event {
  /// input events
  left_pressed,
  left_released,
  right_pressed,
  right_released,
  up_pressed,
  up_released,
  down_pressed,
  down_released,
  rotatel_pressed,
  rotatel_released,
  rotater_pressed,
  rotater_released,
  scalep_pressed,
  scalep_released,
  scalem_pressed,
  scalem_released,

  /// virtual console events
  turn_off
};

std::ostream& operator<<(std::ostream& stream, const event e);

class engine;

/// return not null on success
engine* create_engine();
void destroy_engine(engine* e);

struct vertex {
    float x {};
    float y {};
    float z {};
    float r {};
    float g {};
    float b {};
    float tx {};
    float ty {};
};

struct triangle {
    // triangle(const vertex& v0, const vertex& v1, const vertex& v2);

    std::array<vertex, 3> vertices;
};

std::istream& operator>>(std::istream& is, vertex&);
std::istream& operator>>(std::istream& is, triangle&);

class texture {
  public:
    virtual ~texture() = default;
    virtual void load(const std::string_view path) = 0;
    virtual void bind() = 0;
};

class engine {
  public:
    virtual ~engine();

    virtual std::string initialize(std::string_view config) = 0;
    virtual bool read_input(event& e) = 0;
    virtual bool is_key_down(const enum event key) = 0;
    virtual void render(const triangle&) = 0;
    virtual void render(const triangle&, texture* const texture) = 0;
    virtual texture* create_texture(std::string_view path) = 0;

    virtual void swap_buffers() = 0;
    virtual void uninitialize() = 0;
};

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
    void prepare_program();

    // Use this shader for rendering objects.
    void apply_shader_program();

  private:
    // IMPORTANT NOTE: it's assumed that the user specifies attribute
    // location directly in the shader source file by using `location`
    // layout qualifier (opengl es 3.2). So, there is no need to call
    // glBindAttributeLocation().
    void attach_shaders();
    void link_program() const;
    void validate_program() const;

    std::string get_shader_code_from_file(const std::string_view path) const;

    // All shader ids.
    std::vector<GLuint> m_shaders {};
    GLuint m_program {};
};

} // end namespace grp
