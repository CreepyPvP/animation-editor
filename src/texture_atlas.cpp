#include "texture_atlas.hpp"
#include "nine_slice.hpp"

static const NineSlice button = {
    NS_STRAT_REPEAT,
    0, 0,
    16, 16, 16,
    16, 16, 16,
};

const NineSlice* getButton() {
    return &button;
}
