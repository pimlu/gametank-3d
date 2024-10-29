#include "projection.h"

#include "recip_lut.h"
#include "fixed_etc.h"

namespace geometry {

Coord ProjectionMatrix::project(Coord c) const {
    GeoF z = -c.z;
    UnitF div = recipLut.lookup(z);
    
    GeoF x = mulRatio(c.x, px, div);
    GeoF y = mulRatio(c.y, py, div);

    z -= near;

    return {x, y, z};
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
