#include "projection.h"

#include "recip_lut.h"
#include "fixed_etc.h"

namespace geometry {

Coord oob = {0.0, 0.0, 0.0};

Coord ProjectionMatrix::project(Coord c) const {
    GeoF z = -c.z;
    if (z < near || z > far) {
        return oob;
    }
    UnitF div = recipLut.lookup(z);
    
    bool overflow = false;
    GeoF x = mulRatio(c.x, px, div, overflow);
    if (overflow) return oob;
    GeoF y = mulRatio(c.y, py, div, overflow);
    if (overflow) return oob;

    return {x, y, z};
}

ProjectionMatrix ProjectionMatrix::defaultMatrix() {
    // formulas from https://gamedev.stackexchange.com/a/120355
    GeoF px = DEF_PX,
        py = DEF_PY,
        near = NEAR,
        far = FAR;
        // pz = -(FAR + NEAR) / (FAR - NEAR),
        // pw = - (2 * FAR * NEAR) / (FAR - NEAR);
    return {px, py, near, far};
}

}
