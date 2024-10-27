#pragma once

#include "fixed.h"


namespace geometry {


using GeoF = Fixed16<int16_t, int32_t, 8>;
using UnitF = Fixed16<uint16_t, uint32_t, 16>;
using ScreenF = Fixed16<int16_t, int32_t, 12>;

struct Coord {
    GeoF x, y, z;
    constexpr Coord operator+(const Coord &r) const {
        return {x + r.x, y + r.y, z + r.z};
    }

};

struct Triangle {
    Coord a, b, c;

    inline Triangle offset(Coord o) {
        return {a + o, b + o, c + o};
    }
};

struct Coord2d {
    GeoF x, y;
};

}

