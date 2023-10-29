#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assert.h>
#include <glm/mat4x4.hpp>

#include "geometry_generator.hpp"
#include "defines.hpp"

struct RenderMessage {
    // 0: draw batch
    // 1: bind context
    unsigned char type;

    union {
        Batch batch;
        // -1: no context
        // 0: render ui
        // 1: render font
        char context;
    } value;
};

struct RenderQueue {
    RenderMessage* renderBuffer;
    int renderBufferCapacity;
    int renderBufferLength;

    unsigned int fontShader;
    unsigned int fontAtlas;
    unsigned int uiShader;
    unsigned int uiAtlas;

    void init(int capacity);
    void execute();
    void push(RenderMessage message);

    void renderFont(Batch batch);
    void renderUi(Batch batch);
};
