#pragma once

#include "../triangle/triangle.hxx"

struct vertex {
    double x = 0;
    double y = 0;
    double r = 0;
    double g = 0;
    double b = 0;
};

struct uniforms {
    double t = 0;
    double s = 1;
    double c = 0;
    vertex v1;
    vertex v2;
    vertex v3;
};

double interpolate(const double f0, const double f1, const double t);

vertex interpolate(const vertex& v0, const vertex& v1, const double t);

struct gfx_program {
    virtual void set_uniforms(const uniforms&) = 0;
    virtual ~gfx_program() = default;
    virtual vertex vertex_shader(const vertex& v_in) = 0;
    virtual rgb_color fragment_shader(const vertex& v_in) = 0;
};

struct triangle_interpolated : triangle_render {
    triangle_interpolated(canvas& buffer);
    void set_gfx_program(gfx_program& program) {
      program_ = &program;
    }
    void draw_triangles(std::vector<vertex>& vertexes, size_t num_vertices);
    void draw_triangle(std::vector<vertex>& vertexes);

  private:
    std::vector<vertex> rasterize_triangle(const vertex& v0, const vertex& v1, const vertex& v2);
    std::vector<vertex> raster_horizontal_triangle(const vertex& single,
                                                   const vertex& left,
                                                   const vertex& right);

    void raster_one_horizontal_line(const vertex& left_vertex,
                                    const vertex& right_vertex,
                                    std::vector<vertex>& out);

    gfx_program* program_ = nullptr;
};
