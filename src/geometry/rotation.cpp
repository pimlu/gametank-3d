#include "rotation.h"

#include "system/scr.h"

namespace geometry {

Coord __attribute__((noinline)) Rotation::apply(Coord c) const {
    // first, rotate to heading
    GeoF x = heading.cos() * c.x - heading.sin() * c.z;
    GeoF z = heading.sin() * c.x + heading.cos() * c.z;

    // then, tilt according to pitch
    GeoF y = pitch.sin() * z + pitch.cos() * c.y;
    z = pitch.cos() * z - pitch.sin() * c.y;

    return {x, y, z};
}

Coord __attribute__((noinline)) Rotation::applyNeg(Coord c) const {
    // first, rotate to heading
    GeoF x = heading.cos() * c.x + heading.sin() * c.z;
    GeoF z = -heading.sin() * c.x + heading.cos() * c.z;

    // then, tilt according to pitch
    GeoF y = -pitch.sin() * z + pitch.cos() * c.y;
    z = pitch.cos() * z + pitch.sin() * c.y;

    
    return {x, y, z};
}


}