#include "renderer.hpp"
#include <stdlib.h>


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

void GeometryGenerator::drawRectangle(float x, float y, float width, float height) {
    int vertex = vertexBase + vertexSprite;
    int index = indexBase + indexSprite;

    vertexBuffer[vertex + 0].x = x;
    vertexBuffer[vertex + 0].y = y + height;
    vertexBuffer[vertex + 1].x = x + width;
    vertexBuffer[vertex + 1].y = y + height;
    vertexBuffer[vertex + 2].x = x;
    vertexBuffer[vertex + 2].y = y;
    vertexBuffer[vertex + 3].x = x + width;
    vertexBuffer[vertex + 3].y = y;

    indexBuffer[index + 0] = 1 + vertex;
    indexBuffer[index + 1] = 3 + vertex;
    indexBuffer[index + 2] = 2 + vertex;
    indexBuffer[index + 3] = 0 + vertex;
    indexBuffer[index + 4] = 1 + vertex;
    indexBuffer[index + 5] = 2 + vertex;

    vertexSprite += 4;
    indexSprite += 6;
}
