#pragma once

#include "renderer.hpp"
#define MAX_UI_LAYER 10

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

    GeometryGenerator* geometry;

    float mouseX;
    float mouseY;
};

void beginContainer(int id, UiContext* context);
void endContainer(int id, UiContext* context);

bool button(float x, float y, int id, UiContext* context, Batch* batch);
