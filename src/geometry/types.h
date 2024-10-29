#pragma once

#include "fixed.h"
#include "graphics/types.h"


namespace geometry {


using GeoF = Fixed16<int16_t, int32_t, 8>;
using UnitF = Fixed16<uint16_t, uint32_t, 16>;

constexpr GeoF scaleGeoF(GeoF val, UnitF scale) {
    int16_t raw = val.getRaw();
    bool negated = raw < 0;
    uint16_t posRaw = negated ? -(int32_t) raw : raw;
    uint32_t product = posRaw * (uint32_t) scale.getRaw();
    GeoF result = GeoF::fromRaw(product >> 16);
    return negated ? -result : result;
}


// this lame thing is because UnitF is unsigned
struct iUnitF {
    UnitF val;
    bool negated;
    

    GeoF operator*(const GeoF &r) const {
        // *(volatile int16_t*) 0x2008 = 0xbeAf;
        // *(volatile int16_t*) 0x2008 = r.getRaw();
        // *(volatile uint16_t*) 0x2008 = val.getRaw();
        // if (r.getRaw()) {
        //     *(volatile int16_t*) 0x2008 = r.getRaw();
        // }
        GeoF res = scaleGeoF(r, val);
        // *(volatile int16_t*) 0x2008 = res.getRaw();
        // *(volatile int16_t*) 0x2008 = 0xdead;
        return negated ? -res : res; 
    }
    iUnitF operator-() const {
        return {val, !negated};
    }
};

struct Coord {
    GeoF x, y, z;

    constexpr Coord& operator+=(const Coord &r) {
        x += r.x;
        y += r.y;
        z += r.z;
        return *this;
    }
    constexpr Coord operator+(const Coord &r) const {
        Coord res = *this;
        res += r;
        return res;
    }
    constexpr Coord operator-(const Coord &r) const {
        return {x - r.x, y - r.y, z - r.z};
    }
};

struct Coord2d {
    GeoF x, y;
};

}

