#include "texture_atlas.hpp"
#include "nine_slice.hpp"

static const NineSlice button = {
    NS_STRAT_REPEAT,
    0, 0,
    3, 3, 3,
    3, 3, 3,
};

static const NineSlice buttonHovered = {
    NS_STRAT_REPEAT,
    9, 0,
    3, 3, 3,
    3, 3, 3,
};

static const NineSlice buttonPressed = {
    NS_STRAT_REPEAT,
    18, 0,
    3, 3, 3,
    3, 3, 3,
};


const NineSlice* getButton() {
    return &button;
}

const NineSlice* getButtonHovered() {
    return &buttonHovered;
}

const NineSlice* getButtonPressed() {
    return &buttonPressed;
}
