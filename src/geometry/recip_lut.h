#pragma once

#include "types.h"
#include "lut.h"


namespace geometry {


struct RecipLut {
    static constexpr double START = 0.5;
    static constexpr double STEP = 8.0 / 256;

    Lut<GeoF, GeoF(START), GeoF(STEP), UnitF> lut; 

    consteval RecipLut() : lut() {
        double cur = START;
        // 1 is slightly outside the range lol
        lut.setEntry(0, UnitF::fromRaw(0xffff));
        cur += STEP;

        for (int i = 1; i < 256; i++) {
            lut.setEntry(i, 1 / cur);

            cur += STEP;
        }
        lut.setLast(1 / cur);
    }
    constexpr UnitF lookup(GeoF x) const {
        return lut.lookupOrConst(x, 0.0, 0.0);
    }
};

extern const RecipLut recipLut;

}