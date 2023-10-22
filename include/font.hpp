#pragma once

#define GLYPH_COUNT 128


struct Glyph { 
    // glyph coordinates
    
    int startX;

    int width;
    int height;
    int left;
    int top;
    int advance;

    unsigned char* bitmapBuffer;
};

int setupFontAtlas(unsigned int* atlas, Glyph* glyphStore, const char* filepath);
