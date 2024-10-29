#pragma once

#include "types.h"


namespace geometry {

struct CachedCoord {
    Coord val;
    uint8_t projFrame;



    constexpr CachedCoord(Coord val): val(val), projFrame(0xff) {}
    constexpr CachedCoord(): CachedCoord(Coord()) {}

    template<typename F>
    Coord get(uint8_t frame, F calc) {
        if (projFrame == frame) return val;
        projFrame = frame;
        val = calc();
        return val;
    }
};

}
