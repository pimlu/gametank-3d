#pragma once

#include "types.h"
#include "fixed.h"
#include "recip_lut.h"


namespace geometry {

GeoF mulRatio(GeoF x, GeoF num, UnitF den);
int8_t roundGeoF(GeoF x);

}