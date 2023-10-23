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

void GeometryGenerator::drawBitmapString(float x, float y, const char* str, float scale) {
    const char* ptr = str;
    const float letterSpacing = 5;
    float xPos = x;
    while (*ptr) {
        int charIndex = *ptr - 65;
        if (charIndex > 26 || charIndex < 0) {
            return;
        }

        float uvx = (float) fontLookup[charIndex * 3] / TEXTURE_ATLAS_WIDTH;
        float uvy = (float) fontLookup[charIndex * 3 + 1] / TEXTURE_ATLAS_HEIGHT;
        int charWidth = fontLookup[charIndex * 3 + 2];
        float uvw = (float) charWidth / TEXTURE_ATLAS_WIDTH;
        float uvh = (float) FONT_HEIGHT / TEXTURE_ATLAS_HEIGHT;

        drawSprite(
            xPos, y,
            charWidth * scale, FONT_HEIGHT * scale,
            uvx, uvy,
            uvw, uvh
        );
        
        xPos += (charWidth + letterSpacing) * scale;

        ++ptr;
    }
}

void GeometryGenerator::drawString(float x, float y, const char* str, const Font* font, float scale) {
    const char* ptr = str;
    float xPos = x;
    while (*ptr) {
        if (*ptr >= GLYPH_COUNT) {
            return;
        }
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

