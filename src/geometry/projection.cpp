#include "projection.h"

#include "recip_lut.h"

namespace geometry {

Coord ProjectionMatrix::project(Coord c) {
    c.z.negate();
    UnitF div = recipLut.lookup(c.z);
    GeoF pxAdj = px.shiftMul(div);

    // w.shiftMul(pw);


    c.x *= px;
    c.y.shiftMul(py);
    c.z -= near;
    c.z *= dyRange;



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
