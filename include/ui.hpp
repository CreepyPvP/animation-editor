#pragma once

#include "geometry_generator.hpp"

#define MAX_UI_LAYER 10
#define MOUSE_JUST_PRESSED 1 << 0
#define MOUSE_JUST_RELEASED 1 << 1

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
    unsigned char mouseFlags;
};

void beginContainer(int id, UiContext* context);
void endContainer(int id, UiContext* context);

bool button(float x, float y, float width, float height, int id, UiContext* context, Batch* batch);
