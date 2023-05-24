#version 410 core
// in vec3 a_position;
// out vec4 v_position;

// void main()
// {
//     v_position = vec4(a_position, 1.0);
//     gl_Position = v_position;
// }

layout(location = 0) in vec4 a_position;
layout(location = 1) in vec4 a_color;
out vec4 v_color;

void main() {
    v_color = a_color;
    gl_Position = a_position;
}