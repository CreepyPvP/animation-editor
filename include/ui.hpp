#pragma once

#define MAX_UI_LAYER

struct UiStack {
    float xOff;
    float yOff;

    int containerId;
};

struct UiContext {
    int hot;
    int active;

    int currentStackDepth;
    UiStack stack[MAX_UI_LAYER];
};

void beginContainer(int id, UiContext* ctx);
void endContainer(int id, UiContext* ctx);

bool button(float x, float y, int id, UiContext* ctx);
