#include "projection.h"

#include "recip_lut.h"
#include "fixed_etc.h"

namespace geometry {

Coord ProjectionMatrix::project(Coord c) const {
    GeoF z = -c.z;
    if (z < near || z > far) {
        return {0.0, 0.0, 0.0};
    }
    UnitF div = recipLut.lookup(z);
    
    GeoF x = mulRatio(c.x, px, div);
    GeoF y = mulRatio(c.y, py, div);

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
    return {px, py, near, FAR};
}

}
