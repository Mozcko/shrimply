#version 300 es
precision mediump float;
in vec2 v_uv;
uniform sampler2D u_rgba_texture;
out vec4 out_color;
void main() {
    out_color = texture(u_rgba_texture, v_uv);
}
