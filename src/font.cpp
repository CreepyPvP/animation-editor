#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "font.hpp"

#include <ft2build.h>
#include <stdio.h>
#include FT_FREETYPE_H

static int max(int a, int b) { 
    return a > b? a : b;
}

int setupFontAtlas(unsigned int* atlas, Glyph* glyphStore, const char* filepath) {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        printf("Could not init freetype library\n");
        return -1;
    }

    FT_Face face;
    if (FT_New_Face, filepath, 0, &face) {
        printf("Failed to load font: %s\n", filepath);
        return -2;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    int totalWidth = 0;
    int totalHeight = 0;
    for (unsigned char c = 0; c < GLYPH_COUNT; ++c) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            printf("Failed to load glyph: %c\n", c);
            continue;
        }

        glyphStore[c].width = face->glyph->bitmap.width;
        glyphStore[c].height = face->glyph->bitmap.rows;
        glyphStore[c].left = face->glyph->bitmap_left;
        glyphStore[c].top = face->glyph->bitmap_top;
        glyphStore[c].advance = face->glyph->advance.x;
        glyphStore[c].bitmapBuffer = face->glyph->bitmap.buffer;

        glyphStore[c].startX = totalWidth;
        totalWidth += glyphStore[c].width + 1;
        totalHeight = max(totalHeight, glyphStore[c].height);
    }

    glGenTextures(1, atlas);
    glBindTexture(GL_TEXTURE_2D, *atlas);
    glTexImage2D(
        GL_TEXTURE_2D, 
        0,
        GL_RED,
        totalWidth - 1,
        totalHeight,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        NULL
    );

    for (unsigned char c = 0; c < GLYPH_COUNT; ++c) {
        Glyph glyph = glyphStore[c];
        glTexSubImage2D(
            GL_TEXTURE_2D,
            0,
            glyph.startX,
            0,
            glyph.width,
            glyph.height,
            GL_RED,
            GL_UNSIGNED_BYTE,
            glyph.bitmapBuffer
        );
    }

    return 0;
}
