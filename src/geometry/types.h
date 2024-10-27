#pragma once

#include "fixed.h"


namespace geometry {


using GeoF = Fixed16<int16_t, int32_t, 8>;
using UnitF = Fixed16<uint16_t, uint32_t, 16>;
using ScreenF = Fixed16<int16_t, int32_t, 12>;

struct Coord {
    GeoF x, y, z;
};
struct ScreenCoord {
    GeoF x, y;
};

}

