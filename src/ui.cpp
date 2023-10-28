#include "ui.hpp"
#include "texture_atlas.hpp"
#include <assert.h>

void beginContainer(int id, UiContext* ctx) {
    UiStack stack;
    stack.containerId = id;
    
    int prev = ctx->currentStackDepth;
    if (prev >= 0) {
        UiStack parentStack = ctx->stack[prev];
        stack.xOff = parentStack.xOff;
        stack.yOff = parentStack.yOff;
    } else {
        stack.xOff = 0;
        stack.yOff = 0;
    }

    int current = prev + 1;
    assert(current < MAX_UI_LAYER);

    ctx->currentStackDepth = current;
    ctx->stack[current] = stack;
}

void endContainer(int id, UiContext* ctx) {
    int index = ctx->currentStackDepth;
    assert(index >= 0 && ctx->stack[index].containerId == id);
    ctx->currentStackDepth--;
}

bool button(float x, float y, int id, UiContext* context, Batch* batch) {
    const int width = 100;
    const int height = 100;

    context->geometry->startBatch();
    context->geometry->drawNineSlice(x, y, width, height, getButton(), 8);
    *batch = context->geometry->endBatch();

    return false;
}
