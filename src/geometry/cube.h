#pragma once

#include "types.h"
#include "triangle.h"
#include "camera.h"


namespace geometry {

class Cube {
    Coord lo, hi;

    Coord verts[8];
    // x-lo, x-hi, y-lo, y-hi, z-lo, z-hi
    uint8_t colors[6];
public:
    
    constexpr Cube(Coord lo, Coord hi, uint8_t colors_[6]) : lo(lo), hi(hi), verts() {
        for (uint8_t i = 0; i < 6; i++) {
            colors[i] = colors_[i];
        }
    }

    GeoF calcDistance(Camera &cam);
    void paint(Camera &cam);

    Coord debugGetVert(uint8_t i);
};


}