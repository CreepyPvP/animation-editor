#include <cassert>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "renderer.hpp"
#include "defines.hpp"
#include "texture_atlas.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define VERTEX_BUFFER_CAPACITY 4000 
#define INDEX_BUFFER_CAPACITY 4000 


static int min(int a, int b) {
    return a < b ? a : b;
}

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

void GeometryGenerator::drawRectangle(float x, float y, float width, float height) {
    int vertex = vertexBase + vertexSprite;
    int index = indexBase + indexSprite;

    // TODO: Scale with dpi factor here
    float startX = floor(x + 0.5);
    float startY = floor(y + 0.5);
    float endX = floor(x + width + 0.5);
    float endY = floor(y + height + 0.5);

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

void GeometryGenerator::drawSprite(float x, float y, float width, float height, float uvx, float uvy, float uvw, float uvh) {
    int vertex = vertexBase + vertexSprite;
    int index = indexBase + indexSprite;

    // TODO: Scale with dpi factor here
    float startX = floor(x + 0.5);
    float startY = floor(y + 0.5);
    float endX = floor(x + width + 0.5);
    float endY = floor(y + height + 0.5);

    vertexBuffer[vertex + 0].x = startX;
    vertexBuffer[vertex + 0].y = endY;
    vertexBuffer[vertex + 0].uvX = uvx;
    vertexBuffer[vertex + 0].uvY = uvy + uvh;
    vertexBuffer[vertex + 1].x = endX;
    vertexBuffer[vertex + 1].y = endY;
    vertexBuffer[vertex + 1].uvX = uvx + uvw;
    vertexBuffer[vertex + 1].uvY = uvy + uvh;
    vertexBuffer[vertex + 2].x = startX;
    vertexBuffer[vertex + 2].y = startY;
    vertexBuffer[vertex + 2].uvX = uvx;
    vertexBuffer[vertex + 2].uvY = uvy;
    vertexBuffer[vertex + 3].x = endX;
    vertexBuffer[vertex + 3].y = startY;
    vertexBuffer[vertex + 3].uvX = uvx + uvw;
    vertexBuffer[vertex + 3].uvY = uvy;

    indexBuffer[index + 0] = 1 + vertex;
    indexBuffer[index + 1] = 3 + vertex;
    indexBuffer[index + 2] = 2 + vertex;
    indexBuffer[index + 3] = 0 + vertex;
    indexBuffer[index + 4] = 1 + vertex;
    indexBuffer[index + 5] = 2 + vertex;

    vertexSprite += 4;
    indexSprite += 6;
}

void GeometryGenerator::drawString(float x, float y, const char* str, const Font* font, float scale) {
    const char* ptr = str;
    float xPos = x;
    while (*ptr) {
        Glyph glyph = font->glyphs[*ptr];

        float uvx = (float) glyph.startX / (float) font->atlasWidth;
        float uvy = 0;
        float uvw = (float) glyph.width / (float) font->atlasWidth;
        float uvh = (float) glyph.height / (float) font->atlasHeight;

        drawSprite(
            xPos + glyph.left * scale, y - glyph.top * scale,
            glyph.width * scale, glyph.height * scale,
            uvx, uvy,
            uvw, uvh
        );
        
        xPos += (float) glyph.advance * scale / 64;

        ++ptr;
    }
}

void GeometryGenerator::drawNineSlice(float x, float y, float width, float height, const NineSlice* nineSlice, float scale) {
    // Note: Scale does not influence actual screen size.
    // TODO: Scale with dpi factor here
    // TODO: Check for fill strategy
    const int startX = x + 0.5;
    const int startY = y + 0.5;
    const int endX = x + width + 0.5;
    const int endY = y + height + 0.5;
    const float uvScale = 1 / scale;

    int widthLeft = nineSlice->widthLeft * scale + 0.5;
    int widthMiddle = nineSlice->widthMiddle * scale + 0.5;
    int widthRight = nineSlice->widthRight * scale + 0.5;

    int heightTop = nineSlice->heightTop * scale + 0.5;
    int heightMiddle = nineSlice->heightMiddle * scale + 0.5;
    int heightBot = nineSlice->heightBot * scale + 0.5;

    assert(endX - startX >= widthLeft + widthRight);
    assert(endY - startY >= heightTop + heightBot);

    // Top Left
    pixelSprite(
        startX, 
        startY, 
        widthLeft, 
        heightTop, 
        nineSlice->startX, 
        nineSlice->startY,
        uvScale
    );
    // Top Middle
    int dx = 0;
    int distanceToLeft = width - widthLeft - widthRight - dx;
    while (distanceToLeft > 0) {
        pixelSprite(
            startX + dx + widthLeft, 
            startY,
            min(widthMiddle, distanceToLeft), 
            heightTop, 
            nineSlice->startX + nineSlice->widthRight, 
            nineSlice->startY,
            uvScale
        );

        dx += widthMiddle;
        distanceToLeft = width - widthLeft - widthRight - dx;
    }
    // Top right
    pixelSprite(
        startX + width - widthRight, 
        startY, 
        widthRight, 
        heightTop, 
        nineSlice->startX + nineSlice->widthLeft + nineSlice->widthMiddle, 
        nineSlice->startY,
        uvScale
    );

    // Middle Left
    pixelSprite(
        startX, 
        startY + heightTop, 
        widthLeft, 
        heightMiddle, 
        nineSlice->startX, 
        nineSlice->startY + nineSlice->heightTop,
        uvScale
    );
    // Middle Middle
    dx = 0;
    distanceToLeft = width - widthLeft - widthRight - dx;
    while (distanceToLeft > 0) {
        pixelSprite(
            startX + dx + widthLeft, 
            startY + heightTop,
            min(widthMiddle, distanceToLeft), 
            heightMiddle, 
            nineSlice->startX + nineSlice->widthRight, 
            nineSlice->startY + nineSlice->heightTop,
            uvScale
        );

        dx += widthMiddle;
        distanceToLeft = width - widthLeft - widthRight - dx;
    }
    // Middle right
    pixelSprite(
        startX + width - widthRight, 
        startY + heightTop, 
        widthRight, 
        heightMiddle, 
        nineSlice->startX + nineSlice->widthLeft + nineSlice->widthMiddle, 
        nineSlice->startY + nineSlice->heightTop,
        uvScale
    );
}

void GeometryGenerator::pixelSprite(int x, int y, int width, int height, int uvx, int uvy, float uvScale) {
    int vertex = vertexBase + vertexSprite;
    int index = indexBase + indexSprite;

    vertexBuffer[vertex + 0].x = x;
    vertexBuffer[vertex + 0].y = y + height;
    vertexBuffer[vertex + 0].uvX = (float) uvx / TEXTURE_ATLAS_WIDTH;
    vertexBuffer[vertex + 0].uvY = (float) (uvy + height * uvScale) / TEXTURE_ATLAS_HEIGHT;
    vertexBuffer[vertex + 1].x = x + width;
    vertexBuffer[vertex + 1].y = y + height;
    vertexBuffer[vertex + 1].uvX = (float) (uvx + width * uvScale) / TEXTURE_ATLAS_WIDTH;
    vertexBuffer[vertex + 1].uvY = (float) (uvy + height * uvScale) / TEXTURE_ATLAS_HEIGHT;
    vertexBuffer[vertex + 2].x = x;
    vertexBuffer[vertex + 2].y = y;
    vertexBuffer[vertex + 2].uvX = (float) uvx / TEXTURE_ATLAS_WIDTH;
    vertexBuffer[vertex + 2].uvY = (float) uvy / TEXTURE_ATLAS_HEIGHT;
    vertexBuffer[vertex + 3].x = x + width;
    vertexBuffer[vertex + 3].y = y;
    vertexBuffer[vertex + 3].uvX = (float) (uvx + width * uvScale) / TEXTURE_ATLAS_WIDTH;
    vertexBuffer[vertex + 3].uvY = (float) uvy / TEXTURE_ATLAS_HEIGHT;

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
    GL(glEnableVertexAttribArray(1));

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

