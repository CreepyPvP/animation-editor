#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "font.hpp"

#include <ft2build.h>
#include <stdio.h>
#include FT_FREETYPE_H

static int max(int a, int b) { 
    return a > b? a : b;
}

int setupFontAtlas(unsigned int* atlas, Font* font, const char* filepath) {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        printf("Could not init freetype library\n");
        return -1;
    }

    FT_Face face;
    if (FT_New_Face(ft, filepath, 0, &face)) {
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

        font->glyphs[c].width = face->glyph->bitmap.width;
        font->glyphs[c].height = face->glyph->bitmap.rows;
        font->glyphs[c].left = face->glyph->bitmap_left;
        font->glyphs[c].top = face->glyph->bitmap_top;
        font->glyphs[c].advance = face->glyph->advance.x;
        font->glyphs[c].bitmapBuffer = face->glyph->bitmap.buffer;

        font->glyphs[c].startX = totalWidth;
        totalWidth += font->glyphs[c].width + 1;
        totalHeight = max(totalHeight, font->glyphs[c].height);
    }
    font->atlasWidth = totalWidth - 1;
    font->atlasHeight = totalHeight;

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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    for (unsigned char c = 0; c < GLYPH_COUNT; ++c) {
        Glyph glyph = font->glyphs[c];
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

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    return 0;
}
