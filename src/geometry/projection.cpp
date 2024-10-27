#include "projection.h"

#include "recip_lut.h"
#include "fixed_etc.h"

namespace geometry {

ScreenCoord ProjectionMatrix::project(Coord c) const {
    UnitF div = recipLut.lookup(-c.z);
    
    GeoF x = mulRatio(c.x, px, div);
    GeoF y = mulRatio(c.y, py, div);

    return {x, y};
}

graphics::ScreenPos ProjectionMatrix::projectScreen(Coord c) const {
    ScreenCoord s = project(c);
    int8_t x = roundGeoF(s.x), y = roundGeoF(s.y);
    return {x, y};
}

GeoF ProjectionMatrix::projectZ(Coord c) const {
    GeoF z = -c.z;
    z -= near;
    z *= dyRange;
    return z;
}

ProjectionMatrix ProjectionMatrix::defaultMatrix() {
    // formulas from https://gamedev.stackexchange.com/a/120355
    GeoF px = DEF_PX,
        py = DEF_PY,
        near = NEAR,
        dyRange = DEF_DYRANGE;
        // pz = -(FAR + NEAR) / (FAR - NEAR),
        // pw = - (2 * FAR * NEAR) / (FAR - NEAR);
    return {px, py, near, dyRange};
}

}
