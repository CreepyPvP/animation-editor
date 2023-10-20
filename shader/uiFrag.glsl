#version 440

uniform sampler2D tileset;

in vec2 uv;

out vec4 out_Color;

void main() {
    out_Color = vec4(uv, 0, 1);
}
