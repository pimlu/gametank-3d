#pragma once

#include "types.h"
#include "lut.h"

#include "sin_lut_data.h"


namespace geometry {


struct SinLut {
    static constexpr double START = 0.0;
    static constexpr double STEP = 32.0 / 256;

    Lut<GeoF, GeoF(START), GeoF(STEP), UnitF> lut; 

    consteval SinLut() : lut() {
        for (int i = 0; i < 256; i++) {
            lut.setEntry(i, sinLutData[i]);
        }
        // 1 is slightly outside the range lol
        lut.setLast(UnitF::fromRaw(0xffff));
    }

private:
    constexpr UnitF lookup(GeoF x) const {
        return lut.lookupOrConst(x, 0.0, 1.0);
    }
    constexpr UnitF sinWrap(GeoF x) const {
        if (x <= 32.0) {
            return lookup(x);
        } else {
            return lookup(GeoF(64.0) - x);
        }
        return lut.lookupOrConst(x, 0.0, 1.0);
    }
public:
    // calculates sin(x/32 * Math.PI/2)
    // a half range of GeoF becomes one sin loop
    constexpr iUnitF __attribute__((noinline)) sin(GeoF x) const {
        if (x >= 0.0) {
            return {
                sinWrap(x),
                false
            };
        } else {
            return {
                sinWrap(-x),
                true
            };
        }
    }
    constexpr iUnitF cos(GeoF x) const {
        // prevent overflow by looping back around
        if (x <= -32.0) {
            return sin(GeoF(32.0 - 128.0) - x);
        }
        return sin(GeoF(32.0) - x);
    }


    
};

extern const SinLut sinLut;

}