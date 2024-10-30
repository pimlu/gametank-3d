#include "fixed_etc.h"

#include "system/imul.h"

#include <limits>

// #include <iostream>

// stuff that uses fixed integer sizes and doesn't go in the template

namespace geometry {

GeoF mulRatio(GeoF x, GeoF num, UnitF den, bool &overflow) {
    int16_t xD = x.getRaw();
    int16_t numD = num.getRaw();
    uint16_t denD = den.getRaw();

    int64_t firstProd = imul32To64(xD, numD);
    int64_t prod = imul32To64(firstProd, denD);
    int64_t res = prod >> (8 + 16);
    if (res < std::numeric_limits<int16_t>::min() || res > std::numeric_limits<int16_t>::max()) {
        overflow = true;
    }

    return GeoF::fromRaw(res);
}


int8_t roundGeoF(GeoF x) {
    int16_t raw = x.getRaw();
    int8_t res = raw >> 8;
    int8_t rem = raw & 0xff;
    // sign bit of rem is the MSB
    if (rem < 0) {
        res++;
    }
    return res;
}

}
