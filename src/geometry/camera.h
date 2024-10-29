#pragma once
#include "types.h"
#include "rotation.h"
#include "projection.h"
#include "cached_coord.h"

namespace geometry {

struct Camera {
    Rotation rotation;
    Coord position;
    ProjectionMatrix mat = ProjectionMatrix::defaultMatrix();
    uint8_t projFrame = 0;

    void tickFrame();
    void updateFromGamepad(uint16_t gamepad);
    Coord project(const Coord &cc) const;
};

}
