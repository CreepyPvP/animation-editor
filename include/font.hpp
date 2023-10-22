#pragma once

#define GLYPH_COUNT 128


struct Glyph { 
    // glyph coordinates
    
    int startX;

    int width;
    int height;
    int left;
    int top;
    unsigned int advance;

    unsigned char* bitmapBuffer;
};

struct Font {
    int atlasWidth;
    int atlasHeight;
    Glyph glyphs[GLYPH_COUNT];
};

int setupFontAtlas(unsigned int* atlas, Font* font, const char* filepath);
