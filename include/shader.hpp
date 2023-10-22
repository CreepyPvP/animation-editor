#pragma once
#include <glm/mat4x4.hpp>
#include <string>

#define Program unsigned int
#define Uniform unsigned int

struct GridShader {
    Program id;
    Uniform uScreenDimensions;
};

struct UiShader {
    Program id;
    Uniform uProjection;
};

struct FontShader {
    Program id;
    Uniform uProjection;
};

GridShader createGridShader(std::string vert, std::string frag);

UiShader createUiShader(std::string vert, std::string frag);

FontShader createFontShader(std::string vert, std::string frag);

void setUniformMat4(unsigned int uniformId, glm::mat4* matrix);

void setUniformVec2(unsigned int uniformId, glm::vec2* vec);

