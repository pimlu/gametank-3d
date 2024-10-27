#include <iostream>
#include <random>

#include "geometry/recip_lut.h"
#include "geometry/fixed_etc.h"
#include "geometry/projection.h"


using namespace geometry;


void checkLookup() {
    double step = 16.0 / 256;

    UnitF left = recipLut.lookup(1.0);

    std::cout << left.toDouble() << std::endl;


    GeoF foo = -66.0;
    foo.shiftMulEq(left);

    std::cout << left.toDouble() << std::endl;
    std::cout << foo.toDouble() << std::endl;

    UnitF right = recipLut.lookup(1.0 + step);
    double rightTrue = 1.0 / (1.0 + step);

    UnitF oneQ = recipLut.lookup(1.0 + step / 4);


    

    std::cout << left.toDouble() << std::endl;
    std::cout << right.toDouble() << " vs " << rightTrue << std::endl;
    std::cout << oneQ.toDouble() << " vs " << (1.0 *0.75 + rightTrue * 0.25) << std::endl;
}

void checkRatio() {


    GeoF x = 2.5, num = 3.0, den = 1.5;

    GeoF res = mulRatio(x, num, recipLut.lookup(den));
    std::cout << res.toDouble() << std::endl;

}

void checkProj() {
    auto mat = geometry::ProjectionMatrix::defaultMatrix();
    auto res = mat.project({-1.0,-1.0,-4.0});
    int8_t x = roundGeoF(res.x), y = roundGeoF(res.y);
    std::cout << res.x.toDouble() << ", " << res.y.toDouble() << std::endl;
    std::cout << (int) x << ", " << (int) y << std::endl;

    res = mat.project({1.0,3.0,-4.0});
    x = roundGeoF(res.x), y = roundGeoF(res.y);
    std::cout << res.x.toDouble() << ", " << res.y.toDouble() << std::endl;
    std::cout << (int) x << ", " << (int) y << std::endl;

    res = mat.project({2.0,-2.0,-4.0});
    x = roundGeoF(res.x), y = roundGeoF(res.y);
    std::cout << res.x.toDouble() << ", " << res.y.toDouble() << std::endl;
    std::cout << (int) x << ", " << (int) y << std::endl;
}


// not really a test, just random junk
int main(int argc, char** argv) {
    // checkLookup();
    // checkRatio();
    // checkProj();

    std::random_device rd;
    std::default_random_engine e1(rd());
    std::uniform_int_distribution<int16_t> uniform_dist(-10, 10);

    for (int i=0; i<1000; i++) {
        int16_t x = uniform_dist(e1);
        int16_t y = uniform_dist(e1);
        if (x*(int32_t)y != geometry::imul16To32(x, y)) {
            std::cout << (int)x << " " << (int) y << " " << geometry::imul16To32(x, y) << std::endl;
            return 1;
        }
    }

}