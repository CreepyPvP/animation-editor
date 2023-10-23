#pragma once

#include "font.hpp"

struct Vertex {
    float x;
    float y;
    float uvX;
    float uvY;
};

struct Batch {
    int baseIndex;
    int indexCount;
};

struct GeometryGenerator {
    Vertex* vertexBuffer;
    unsigned int* indexBuffer;

    int vertexBase;
    int indexBase;
    int vertexSprite;
    int indexSprite;

    void init(int vertexCapacity, int indexCapacity);
    void startBatch();
    Batch endBatch();
    void reset();

    void drawRectangle(float x, float y, float width, float height);
    void drawSprite(float x, float y, float w, float h, float uvx, float uvy, float uvw, float uvh);
    void drawBitmapString(float x, float y, const char* str, float scale);
    void drawString(float x, float y, const char* str, const Font* font, float scale);

    void updateUiBuffers(unsigned int uiVao);
};

unsigned int setupUiVao();

