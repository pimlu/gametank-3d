#pragma once

#include "geometry/types.h"
#include "geometry/cached_coord.h"
#include "geometry/triangle.h"
#include "geometry/camera.h"

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
class Cube {
    geometry::Coord lo, hi;

    geometry::Coord verts[8];
    bool tick;

public:
    
    constexpr Cube(geometry::Coord lo, geometry::Coord hi) : lo(lo), hi(hi), verts() {}

    geometry::GeoF calcDistance(geometry::Camera &cam);
    void paint(geometry::Camera &cam);    
};


#undef TRIANGLE