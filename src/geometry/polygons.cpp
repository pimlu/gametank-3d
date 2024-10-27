#include "polygons.h"

#include "projection.h"
#include "fixed_etc.h"

#include "graphics/types.h"
#include "graphics/triangle.h"

namespace geometry {



void fillTriangle(Coord a, Coord b, Coord c, uint8_t color) {
    ProjectionMatrix mat = ProjectionMatrix::defaultMatrix();
    graphics::ScreenPos ap = mat.projectScreen(a),
        bp = mat.projectScreen(b),
        cp = mat.projectScreen(c);
    graphics::fillTriangle(ap, bp, cp, color);
}
void fillTriangle(Triangle t, uint8_t color) {
    fillTriangle(t.a, t.b, t.c, color);
}

}