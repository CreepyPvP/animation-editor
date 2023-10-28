#include "ui.hpp"
#include "nine_slice.hpp"
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

bool button(float x, float y, float width, float height, int id, UiContext* context, Batch* batch) {
    if (context->mouseX > x &&
        context->mouseX < x + width &&
        context->mouseY > y &&
        context->mouseY < y + height) {
        context->hot = id;

        if (context->mouseFlags & MOUSE_JUST_PRESSED) {
            context->active = id;
        }
    } else {
        if (context->active == id) {
            context->active = -1;
        }
    }


    const NineSlice* model = getButton();
    bool value = false;
    if (context->hot == id) {
        model = getButtonHovered();
    }
    if (context->active == id) {
        model = getButtonPressed();
        if (context->mouseFlags & MOUSE_JUST_RELEASED) {
            value = true;
            context->active = -1;
        }
    }

    context->geometry->startBatch();
    context->geometry->drawNineSlice(x, y, width, height, model, 8);
    *batch = context->geometry->endBatch();

    return value;
}
