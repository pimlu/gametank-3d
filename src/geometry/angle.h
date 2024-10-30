#pragma once

#include "types.h"
#include "sin_lut.h"

namespace geometry {

class Angle {
    GeoF theta;

    iUnitF sinVal, cosVal;

public:

    constexpr Angle(GeoF theta) :
        theta(theta),
        sinVal(sinLut.sin(theta)),
        cosVal(sinLut.cos(theta)) {}

    constexpr Angle() : Angle(0.0) {}

    constexpr GeoF getTheta() const {
        return theta;
    }

    constexpr void setTheta(GeoF val) {
        theta = val;
        sinVal = sinLut.sin(val);
        cosVal = sinLut.cos(val);
    }
    constexpr void adjust(GeoF val) {
        GeoF next = getTheta() + val;
        if (next > 64.0) next -= 128.0;
        if (next < -64.0) next += 128.0;
        setTheta(next);
    }

    constexpr iUnitF sin() const {
        return sinVal;
    }

    constexpr iUnitF cos() const {
        return cosVal;
    }

    constexpr Coord2d apply(Coord2d p) const {
        GeoF x = cos() * p.x - sin() * p.y;
        GeoF y = sin() * p.x + cos() * p.y;
        return {x, y};
    }
    constexpr Coord2d applyNeg(Coord2d p) const {
        GeoF x = cos() * p.x + sin() * p.y;
        GeoF y = -sin() * p.x + cos() * p.y;
        return {x, y};
    }
  
};


}
