#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "renderer.hpp"
#include "defines.hpp"
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define VERTEX_BUFFER_CAPACITY 4000 
#define INDEX_BUFFER_CAPACITY 4000 

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

void GeometryGenerator::reset() {
    vertexBase = 0;
    indexBase = 0;
    vertexSprite = 0;
    indexSprite = 0;
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
    vertexBuffer[vertex + 0].uvX = 0;
    vertexBuffer[vertex + 0].uvY = 1;
    vertexBuffer[vertex + 1].x = endX;
    vertexBuffer[vertex + 1].y = endY;
    vertexBuffer[vertex + 1].uvX = 1;
    vertexBuffer[vertex + 1].uvY = 1;
    vertexBuffer[vertex + 2].x = startX;
    vertexBuffer[vertex + 2].y = startY;
    vertexBuffer[vertex + 2].uvX = 0;
    vertexBuffer[vertex + 2].uvY = 0;
    vertexBuffer[vertex + 3].x = endX;
    vertexBuffer[vertex + 3].y = startY;
    vertexBuffer[vertex + 3].uvX = 1;
    vertexBuffer[vertex + 3].uvY = 0;

    indexBuffer[index + 0] = 1 + vertex;
    indexBuffer[index + 1] = 3 + vertex;
    indexBuffer[index + 2] = 2 + vertex;
    indexBuffer[index + 3] = 0 + vertex;
    indexBuffer[index + 4] = 1 + vertex;
    indexBuffer[index + 5] = 2 + vertex;

    vertexSprite += 4;
    indexSprite += 6;
}

unsigned int setupUiVao() {
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int buffers[2];
    GL(glGenBuffers(2, buffers));
    unsigned int vertexBuffer = buffers[0];
    unsigned int indexBuffer = buffers[1];

    GL(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));
    GL(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * VERTEX_BUFFER_CAPACITY, NULL, GL_DYNAMIC_DRAW));
    GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer));
    GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * INDEX_BUFFER_CAPACITY, NULL, GL_DYNAMIC_DRAW));
    GL(glEnableVertexAttribArray(0));

    GL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0));
    GL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, uvX)));

    return vao;
}

void GeometryGenerator::updateUiBuffers(unsigned int uiVao) {
    glBindVertexArray(uiVao);
        
    Vertex* vertexBuffMap = (Vertex*) glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * vertexBase, GL_MAP_WRITE_BIT);
    memcpy(vertexBuffMap, vertexBuffer, sizeof(Vertex) * vertexBase);
    GL(glUnmapBuffer(GL_ARRAY_BUFFER));
    unsigned int* indexBuffMap = (unsigned int*) glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(unsigned int) * indexBase, GL_MAP_WRITE_BIT);
    memcpy(indexBuffMap, indexBuffer, sizeof(unsigned int) * indexBase);
    GL(glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER));
}
