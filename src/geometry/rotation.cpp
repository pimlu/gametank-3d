#include "rotation.h"

#include "system/scr.h"

namespace geometry {

Coord __attribute__((noinline)) Rotation::apply(Coord c) const {
    // first, tilt according to pitch
    GeoF z = pitch.cos() * c.z - pitch.sin() * c.y;
    GeoF y = pitch.sin() * c.z + pitch.cos() * c.y;


    // then, rotate to heading
    GeoF x = heading.cos() * c.x - heading.sin() * z;
    z = heading.sin() * c.x + heading.cos() * z;

    return {x, y, z};
}

Coord __attribute__((noinline)) Rotation::applyNeg(Coord c) const {
    // first, tilt according to pitch
    GeoF z = pitch.cos() * c.z + pitch.sin() * c.y;
    GeoF y = -pitch.sin() * c.z + pitch.cos() * c.y;


    // then, rotate to heading
    GeoF x = heading.cos() * c.x + heading.sin() * z;
    z = -heading.sin() * c.x + heading.cos() * z;

    return {x, y, z};
}


}