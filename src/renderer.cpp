#include "renderer.hpp"
#include <stdlib.h>
#include <math.h>

void GeometryGenerator::init(int vertices, int indices) {
    vertexBuffer = (Vertex*) malloc(sizeof(Vertex) * vertices);
    indexBuffer = (unsigned int*) malloc(sizeof(unsigned int) * indices);
    vertexBase = 0;
    indexBase = 0;
}

void GeometryGenerator::startBatch() {
    vertexSprite = 0;
    indexSprite = 0;
}

Batch GeometryGenerator::endBatch() {
    Batch b;
    b.baseIndex = indexBase;
    b.indexCount = indexSprite;
    vertexBase += vertexSprite;
    indexBase += indexSprite;
    return b;
}

void GeometryGenerator::drawRectangle(float x1, float y1, float x2, float y2) {
    int vertex = vertexBase + vertexSprite;
    int index = indexBase + indexSprite;

    // Scale with dpi factor here
    float startX = floor(x1 + 0.5);
    float startY = floor(y1 + 0.5);
    float endX = floor(x2 + 0.5);
    float endY = floor(y2 + 0.5);

    vertexBuffer[vertex + 0].x = startX;
    vertexBuffer[vertex + 0].y = endY;
    vertexBuffer[vertex + 1].x = endX;
    vertexBuffer[vertex + 1].y = endY;
    vertexBuffer[vertex + 2].x = startX;
    vertexBuffer[vertex + 2].y = startY;
    vertexBuffer[vertex + 3].x = endX;
    vertexBuffer[vertex + 3].y = startY;

    indexBuffer[index + 0] = 1 + vertex;
    indexBuffer[index + 1] = 3 + vertex;
    indexBuffer[index + 2] = 2 + vertex;
    indexBuffer[index + 3] = 0 + vertex;
    indexBuffer[index + 4] = 1 + vertex;
    indexBuffer[index + 5] = 2 + vertex;

    vertexSprite += 4;
    indexSprite += 6;
}


