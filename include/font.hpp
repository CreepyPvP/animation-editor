#pragma once

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
