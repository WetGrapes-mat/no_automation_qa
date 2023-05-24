#version 410 core
// precision mediump float;
// in vec4 v_position;
// out vec4 frag_color;
// void main()
// {
//     frag_color = vec4(0.8, 0.0, 0.8, 1.0);

// }

precision mediump float;

in vec4 v_color;
out vec4 frag_color;

void main() {
    frag_color = v_color;
}