#version 440

uniform sampler2D fontAtlas;

in vec2 uv;

out vec4 out_Color;

void main() {
    out_Color = vec4(1, 1, 1, texture(fontAtlas, uv).r);
    // out_Color = vec4(uv, 0, 1);
}
