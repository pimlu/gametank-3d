#pragma once
#include "geometry/types.h"
#include "geometry/rotation.h"
#include "geometry/camera.h"

class Fish {
    geometry::Coord basePos;
    geometry::Angle angle;
    uint8_t colors[3];
    // 7 fish verts then the eye
    geometry::Coord verts[8];
public:
    Fish(geometry::Coord basePos, geometry::Angle angle, uint8_t colors_[3]);

    geometry::GeoF calcDistance(geometry::Camera &cam);
    void paint(geometry::Camera &cam);
};
