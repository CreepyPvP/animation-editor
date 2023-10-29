#include "render_queue.hpp"

#include <stdlib.h>


void RenderQueue::init(int capacity) {
    renderBuffer = (RenderMessage*) malloc(sizeof(RenderMessage) * capacity);
    renderBufferCapacity = capacity;
    renderBufferLength = 0;
}

// TODO: merge draw calls, avoid unnecessary context switches
void RenderQueue::execute() {
    int current = renderBufferLength;
    while (--current >= 0) {
        RenderMessage message = renderBuffer[current];
        switch (message.type) {
            case 0: {
                Batch batch = message.value.batch;
                GL(glDrawElements(
                    GL_TRIANGLES, 
                    batch.indexCount, 
                    GL_UNSIGNED_INT, 
                    (void*) (batch.baseIndex * sizeof(unsigned int))
                ));
                break;
            }
            case 1: {
                char context = message.value.context;
                if (context == 0) {
                    GL(glUseProgram(uiShader));
                    GL(glBindTexture(GL_TEXTURE_2D, uiAtlas));
                } else if (context == 1) {
                    GL(glUseProgram(fontShader));
                    GL(glBindTexture(GL_TEXTURE_2D, fontAtlas));
                }
                break;
            }
        }
    }

    renderBufferLength = 0;
}

void RenderQueue::push(RenderMessage message) {
    assert(renderBufferLength < renderBufferCapacity);
    renderBuffer[renderBufferLength] = message;
    ++renderBufferLength;
}

void RenderQueue::renderFont(Batch batch) {
    RenderMessage drawCall;
    drawCall.type = 0;
    drawCall.value.batch = batch;
    RenderMessage bindContext;
    bindContext.type = 1;
    bindContext.value.context = 1;

    push(drawCall);
    push(bindContext);
}

void RenderQueue::renderUi(Batch batch) {
    RenderMessage drawCall;
    drawCall.type = 0;
    drawCall.value.batch = batch;
    RenderMessage bindContext;
    bindContext.type = 1;
    bindContext.value.context = 0;

    push(drawCall);
    push(bindContext);
}
