#include "cube.h"

#include "geometry/cached_coord.h"
#include "geometry/polygons.h"
#include "geometry/fixed_etc.h"

using namespace geometry;


GeoF Cube::calcDistance(Camera &cam) {
    GeoF totZ = 0.0;
    for (uint8_t i = 0; i < 8; i++) {

        geometry::GeoF x = i & 1 ? lo.x : hi.x;
        geometry::GeoF y = i & 2 ? lo.y : hi.y;
        geometry::GeoF z = i & 4 ? lo.z : hi.z;

        
        verts[i] = cam.project({x, y, z});
        totZ += verts[i].z;
    }
    return scaleByUint8(totZ, 256 / 8);
}


#define TRIANGLE(a, b, c) geometry::Triangle {verts[a], verts[b], verts[c]}

void Cube::paint(Camera &cam) {
    Coord pos = cam.position;

    const uint8_t c = (uint8_t) ~0b000'01'110;

    if (pos.z < lo.z) {
        fillTriangle(cam, TRIANGLE(0, 1, 2), c);
        fillTriangle(cam, TRIANGLE(3, 1, 2), c);
    } else if (pos.z > hi.z) {
        fillTriangle(cam, TRIANGLE(4, 5, 6), (uint8_t) ~0b000'01'110);
        fillTriangle(cam, TRIANGLE(7, 5, 6), (uint8_t) ~0b100'01'110);
    }

    if (pos.x < lo.x) {
        fillTriangle(cam, TRIANGLE(0, 2, 4), c);
        fillTriangle(cam, TRIANGLE(6, 2, 4), c);
    } else if (pos.x > hi.x) {
        fillTriangle(cam, TRIANGLE(1, 3, 5), ~c);
        fillTriangle(cam, TRIANGLE(7, 3, 5), ~c);
    }

    if (pos.y < lo.y) {
        fillTriangle(cam, TRIANGLE(0, 1, 4), c);
        fillTriangle(cam, TRIANGLE(5, 1, 4), c);
    } else if (pos.y > hi.y) {
        fillTriangle(cam, TRIANGLE(2, 3, 6), c);
        fillTriangle(cam, TRIANGLE(7, 3, 6), c);
    }
}

