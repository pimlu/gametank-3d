#include "polygons.h"

#include "projection.h"
#include "fixed_etc.h"
#include "triangle.h"
#include "camera.h"


#include "graphics/types.h"
#include "graphics/triangle.h"

namespace geometry {


graphics::ScreenPos toScreen(Coord c) {
    int8_t sx = roundGeoF(c.x), sy = roundGeoF(c.y);
    return {sx, sy};
}

void fillTriangle(const Camera &cam, const Triangle &t, uint8_t color) {
    if (t.a.z <= 0.0 || t.b.z == 0.0 || t.c.z == 0.0) {
        return;
    }

    graphics::ScreenPos a = toScreen(t.a),
        b = toScreen(t.b),
        c = toScreen(t.c);
    

    graphics::fillTriangle(a, b, c, color);
}

}