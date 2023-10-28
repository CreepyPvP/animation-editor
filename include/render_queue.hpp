#pragma once

#include <assert.h>

#include "geometry_generator.hpp"
struct RenderMessage {
    // 0 draw batch
    // 1 bind texture
    unsigned char type;

    union {
        Batch batch;
        unsigned int texture;
    } value;
};

struct RenderQueue {
    RenderMessage* messageBuffer;
    int messageBufferCapacity;
    int renderBufferLength;

    void init(int capacity);
    void execute();
    void push(RenderMessage message);
};


