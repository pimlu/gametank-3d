#pragma once

#include <cstdint>
#include "types.h"

namespace geometry {

void fillTriangle(Coord a, Coord b, Coord c, uint8_t color);
void fillTriangle(Triangle t, uint8_t color);

}