#include <iostream>

#include "geometry/recip_lut.h"
#include "geometry/fixed_big.h"


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

    GeoF res = mulRatio(x, num, den);
    std::cout << res.toDouble() << std::endl;

}

// not really a test, just random junk
int main(int argc, char** argv) {
    // checkLookup();
    checkRatio();
}