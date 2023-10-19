#pragma once

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

    void drawRectangle(float x1, float y1, float x2, float y2);
    void drawSprite(float x1, float y1, float x2, float y2, float uvx1, float uvy1, float uvx2, float uvy2);
};
