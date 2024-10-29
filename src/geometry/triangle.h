#pragma once
#include "cached_coord.h"


namespace geometry {

struct Triangle {
    // this represents already projected coords
    Coord &a, &b, &c;
    constexpr geometry::Triangle& operator=(const geometry::Triangle &o) {
        a = o.a;
        b = o.b;
        c = o.c;
        return *this;
    }
};


}
