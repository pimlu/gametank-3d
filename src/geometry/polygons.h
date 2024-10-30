#pragma once

#include <cstdint>
#include "types.h"
#include "triangle.h"
#include "camera.h"

namespace geometry {

graphics::ScreenPos toScreen(Coord c);
// void fillTriangle(Coord a, Coord b, Coord c, uint8_t color);
void fillTriangle(const Camera &cam, const Triangle &t, uint8_t color);

}