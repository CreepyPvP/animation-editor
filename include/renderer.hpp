#pragma once

struct Vertex {
    float x;
    float y;
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

    void drawRectangle(float x, float y, float width, float height);
};
