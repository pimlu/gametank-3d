#pragma once

#include "types.h"
#include "fixed.h"

#include "system/imul.h"


namespace geometry {

GeoF mulRatio(GeoF x, GeoF num, UnitF den, bool &overflow);
int8_t roundGeoF(GeoF x);

constexpr UnitF scaleByUint8(UnitF val, uint8_t scale) {
    uint32_t res = mul16To32(val.getRaw(), scale);
    return UnitF::fromRaw(res >> 8);
}

constexpr GeoF scaleByUint8(GeoF val, uint8_t scale) {
    int32_t res = mul16To32(val.getRaw(), scale);
    return GeoF::fromRaw(res >> 8);
}
}