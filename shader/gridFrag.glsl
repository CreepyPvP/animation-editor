#version 440

in vec2 pos;

out vec4 out_Color;

vec4 maxAlpha(vec4 col1, vec4 col2) {
    return col1.a > col2.a ? col1 : col2;
}

vec4 grid(vec2 worldPos, float scale, vec3 baseColor, float thickness) {

    // Inverse scale => higher scale == smaller grid
    vec2 coord = worldPos * scale;
    vec2 derivative = fwidth(coord);
    vec2 grid = abs((fract(coord - 0.5) - 0.5) / thickness) / derivative;
    float line = min(grid.x, grid.y); // 0 = visible, 1 = invisible

    return vec4(baseColor, 1.0 - min(line, 1.0));
}

void main() {

    out_Color = maxAlpha(
        grid(pos, 10, vec3(0.2, 0.2, 0.2), 1),
        grid(pos, 5, vec3(0.4, 0.4, 0.4), 1)
    );
}

