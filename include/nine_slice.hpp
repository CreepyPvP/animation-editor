#pragma once

#define NS_STRAT_REPEAT 0

struct NineSlice {
    int strategy;

    int startX;
    int startY;

    int widthLeft;
    int widthMiddle;
    int widthRight;

    int heightTop;
    int heightMiddle;
    int heightBot;
};
