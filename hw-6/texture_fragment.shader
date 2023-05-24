#version 410 core
precision mediump float;

in vec4 v_color;
in vec2 v_texture;
out vec4 frag_color;

uniform sampler2D s_texture;

void main() {
    frag_color = texture(s_texture, v_texture) ;
}
