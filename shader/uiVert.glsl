#version 440

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aUv;

out vec2 uv;

uniform mat4 projection;


void main() {
    uv = aUv;
    gl_Position = projection * vec4(aPos, 0, 1);
}
