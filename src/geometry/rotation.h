#pragma once

#include "angle.h"

namespace geometry {
    
struct Rotation {
    Angle heading, pitch;
    Coord apply(Coord input) const;
    Coord applyNeg(Coord c) const;

};

}
