#version 410 core
layout(location = 0) in vec4 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_texture;
out vec4 v_color;
out vec2 v_texture;

void main() {
    v_color = a_color;
    v_texture = a_texture;
    gl_Position = a_position;
}
