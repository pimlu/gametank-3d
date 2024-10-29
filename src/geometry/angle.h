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
        setTheta(getTheta() + val);
    }

    constexpr iUnitF sin() const {
        return sinVal;
    }

    constexpr iUnitF cos() const {
        return cosVal;
    }
  
};


}
