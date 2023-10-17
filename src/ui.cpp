#include <ui.hpp>
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
