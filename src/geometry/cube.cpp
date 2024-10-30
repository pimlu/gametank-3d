#include "cube.h"

#include "geometry/polygons.h"
#include "geometry/fixed_etc.h"
/*

z   y
|  /
| /
  -----x


    6------7
   /       /
  /       /|
 /       / |
4-------5  |
|   |   |  |
|   2---|--3
|  /    |  /
| /     | /
|/      |/
0-------1

*/
namespace geometry {


GeoF Cube::calcDistance(Camera &cam) {
    GeoF totZ = 0.0;
    for (uint8_t i = 0; i < 8; i++) {

        geometry::GeoF x = i & 1 ? hi.x : lo.x;
        geometry::GeoF y = i & 2 ? hi.y : lo.y;
        geometry::GeoF z = i & 4 ? hi.z : lo.z;

        
        verts[i] = cam.project({x, y, z});
        totZ += verts[i].z;
    }
    return scaleByUint8(totZ, 256 / 8);
}


#define TRIANGLE(a, b, c) geometry::Triangle {verts[a], verts[b], verts[c]}

void Cube::paint(Camera &cam) {
    Coord pos = cam.position;
    if (pos.x < lo.x) {
        fillTriangle(cam, TRIANGLE(0, 2, 4), colors[0]);
        fillTriangle(cam, TRIANGLE(6, 2, 4), colors[0]);
    } else if (pos.x > hi.x) {
        fillTriangle(cam, TRIANGLE(1, 3, 5), colors[1]);
        fillTriangle(cam, TRIANGLE(7, 3, 5), colors[1]);
    }

    if (pos.y < lo.y) {
        fillTriangle(cam, TRIANGLE(0, 1, 4), colors[2]);
        fillTriangle(cam, TRIANGLE(5, 1, 4), colors[2]);
    } else if (pos.y > hi.y) {
        fillTriangle(cam, TRIANGLE(2, 3, 6), colors[3]);
        fillTriangle(cam, TRIANGLE(7, 3, 6), colors[3]);
    }

    if ((pos.z < lo.z)) {
        fillTriangle(cam, TRIANGLE(0, 1, 2), colors[4]);
        fillTriangle(cam, TRIANGLE(3, 1, 2), colors[4]);
    } else if (pos.z > hi.z) {
        fillTriangle(cam, TRIANGLE(4, 5, 6), colors[5]);
        fillTriangle(cam, TRIANGLE(7, 5, 6), colors[5]);
    }

}

Coord Cube::debugGetVert(uint8_t i) {
    return verts[i];
}


}