#include "fixed_big.h"

// #include <iostream>

namespace geometry {

GeoF mulRatio(GeoF x, GeoF num, GeoF den) {
    int16_t xD = x.getRaw();
    int16_t numD = num.getRaw();
    uint16_t denD = recipLut.lookup(den).getRaw();


    int64_t prod = ((int64_t) xD) * ((int64_t) numD) * ((int64_t)denD);
    int16_t res = prod >> (8 + 16);
    return GeoF::fromRaw(res);
}
}
