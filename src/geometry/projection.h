#pragma once


#include "types.h"
#include "fixed.h"

#include "graphics/screen.h"
#include "graphics/types.h"

namespace geometry {

// near vs far - dynamic range of the frustum
constexpr double NEAR = 0.5;
// chosen to make FAR-NEAR a power of 2
constexpr double FAR = 8.5;
// Math.tan(60 / 2 * (1/360 * 2 * Math.PI))
constexpr double TAN_FOV2 = 0.5773502691896257;
constexpr double ASPECT_RATIO = graphics::FRAME_W / (double)graphics::FRAME_H;

constexpr double DEF_PX = (graphics::FRAME_W / 2.0) / (ASPECT_RATIO * TAN_FOV2);
constexpr double DEF_PY = (graphics::FRAME_H / 2.0) / TAN_FOV2;
constexpr double DEF_DYRANGE = 1 / (FAR - NEAR);


struct ProjectionMatrix {
    // the values range from [0,4] ish
    // this contains the formulas from L to R of:
    // https://gamedev.stackexchange.com/a/120355
    GeoF px, py, near, far;

    Coord project(Coord c) const;

    static ProjectionMatrix defaultMatrix(); 

};



}