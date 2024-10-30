#include "fish.h"

#include "geometry/polygons.h"

using namespace geometry;

#define V(i, x, y) { \
    auto [dx, dz] = angle.apply({0.0, -(x)}); \
    verts[(i)] = cam.project(basePos + Coord({pz + dz, (y), px + dx})); \
}

Fish::Fish(geometry::Coord basePos, geometry::Angle angle, uint8_t colors_[3]) :
    basePos(basePos), angle(angle) {
    for (uint8_t i = 0; i < 3; i++) {
        colors[i] = colors_[i];
    }
}

GeoF Fish::calcDistance(Camera &cam) {
    angle.adjust(1.0);
    auto [px, pz] = angle.apply({3.0, 0.0});
    V(0, -0.5, 0.0);

    V(1, 0.0, 1.0);
    V(2, 0.0, -1.0);

    V(3, 1.0, 0.5);
    V(4, 1.0, -0.5);

    V(5, 1.5, 1.0);
    V(6, 1.5, -1.0);

    V(7, -0.25, 0.25);
    return verts[7].z;
}
#define TRIANGLE(a, b, c) geometry::Triangle {verts[a], verts[b], verts[c]}
void Fish::paint(Camera &cam) {
    // front
    fillTriangle(cam, TRIANGLE(0, 1, 2), colors[0]);

    // middle
    fillTriangle(cam, TRIANGLE(0, 1, 2), colors[0]);
    fillTriangle(cam, TRIANGLE(1, 2, 3), colors[0]);
    fillTriangle(cam, TRIANGLE(2, 3, 4), colors[0]);

    // tail
    fillTriangle(cam, TRIANGLE(3, 4, 5), colors[1]);
    fillTriangle(cam, TRIANGLE(3, 4, 6), colors[1]);

    // eye
    graphics::ScreenPos eyePos = toScreen(verts[7]);

}