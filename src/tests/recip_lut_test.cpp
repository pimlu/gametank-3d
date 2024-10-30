#include <iostream>
#include <random>
#include <limits>
#include <numbers>

#include "geometry/recip_lut.h"
#include "geometry/fixed_etc.h"
#include "geometry/projection.h"
#include "geometry/sin_lut.h"
#include "geometry/rotation.h"
#include "geometry/camera.h"
#include "geometry/triangle.h"
#include "geometry/cube.h"

using namespace geometry;


void checkLookup() {
    double step = 16.0 / 256;

    UnitF left = recipLut.lookup(1.0);

    std::cout << left.toDouble() << std::endl;


    GeoF foo = -66.0;
    //////////////////////////////////////
    // foo.shiftMulEq(left);

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

    bool overflow = false;
    GeoF res = mulRatio(x, num, recipLut.lookup(den), overflow);
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

void sweepRecip() {

    double worst = 0;
    double maxDiff = 0.0;
    for (double i = 1.0; i < 16.0; i += 1.0 / 1024) {
        double reference = 1/i;
        double actual = recipLut.lookup(i).toDouble();
        double diff = std::abs(actual - reference);
        if (diff > maxDiff) {
            worst = i;
            maxDiff = diff;
        }

        // std::cout << i << "," << reference << "," << actualD << std::endl;
    }


    double reference = 1 / worst;
    double actual = recipLut.lookup(worst).toDouble();
    double diff = std::abs(actual - reference);
    std::cout << "worst=" << worst << ", reference=" << reference << ", actual=" << actual << std::endl; 
}

void sweepSin() {
    // GeoF val = 1.0;
    // GeoF x = 0.0;
    // auto s = sinLut.cos(x);
    // auto sr = s.val.getRaw();
    // auto vr = val.getRaw();
    // std::cout << (int) sr << " " << (int) vr << std::endl;

    // std::cout << (int) (sr*(int32_t)vr) << std::endl;
    // std::cout << (int) imul16To32(sr, vr) << std::endl;
    // std::cout << s.val.toDouble() << std::endl;
    // std::cout << ( s * val).toDouble() << std::endl;


    double worst = -53.3125;
    double maxDiff = 0.0;
    // for (double i = -64.0; i <= 64.0; i += 1.0 / 32) {
    //     double reference = std::sin(i / 32.0 * (std::numbers::pi)/2);
    //     iUnitF actual = sinLut.sin(i);
    //     double actualD = actual.val.toDouble() * (actual.negated ? -1 : 1);
    //     double diff = std::abs(actualD - reference);
    //     if (diff > maxDiff) {
    //         worst = i;
    //         maxDiff = diff;
    //     }

    //     // std::cout << i << "," << reference << "," << actualD << std::endl;
    // }


    double reference = std::cos(worst / 32.0 * (std::numbers::pi)/2);
    iUnitF actual = sinLut.cos(worst);
    double actualD = actual.val.toDouble() * (actual.negated ? -1 : 1);
    double diff = std::abs(actualD - reference);
    std::cout << "worst=" << worst << ", reference=" << reference << ", actual=" << actualD << std::endl; 
}

int checkMul() {
    // std::random_device rd;
    // std::default_random_engine e1(rd());
    // std::uniform_int_distribution<int32_t> uniform_dist(std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max());

    // std::cout << "fuzzing mul" << std::endl;
    // for (int i=0; i<100'000'000; i++) {
    //     int32_t x = uniform_dist(e1);
    //     int16_t y = uniform_dist(e1);
    //     // std::cout << (int)x << " " << (int) y << " " << imul32To64(x, y) << std::endl;
    //     if (x*(int64_t)y != imul32To64(x, y)) {
    //         std::cout << (int)x << " " << (int) y << " " << imul32To64(x, y) << std::endl;
    //         return 1;
    //     }
    // }
        int32_t x = 32999;
        int32_t y = 128;
        std::cout << (int)x << " " << (int) y << " " << imul16To32(x, y) << " " << (x * y) << std::endl;
    return 0;
}

void checkRotation() {
    Rotation r;
    Coord c = {1.0, 1.0, 1.0};
    Coord cp = r.apply(c);
    std::cout << cp.x.toDouble() << " " << cp.y.toDouble() << " " << cp.z.toDouble() << std::endl;
}

void checkCamera() {

    geometry::Camera camera;

    camera.position = {0.0, 0.0, 0.0};

    // Cube cube({0.0, 0.0, 0.0}, {0.5, 0.5, 0.5});

    geometry::Coord a = {-0.25,-0.25,-3.5};
    geometry::Coord b = {0.25,3.0/4,-3.5};
    geometry::Coord c = {0.5,-0.5,-3.5};
    geometry::Triangle t = {a, b, c};


    camera.project(a);

    camera.project(b);

    camera.project(c);

}

void output(Coord c) {
    std::cout << "{" << c.x.toDouble() << ", " << c.y.toDouble() << ", " << c.z.toDouble() << "}" << std::endl;
}


namespace geometry {

    void fillTriangle(const Camera &cam, const Triangle &t, uint8_t color) {}
}

// not really a test, just random junk
int main(int argc, char** argv) {
    // checkLookup();
    // checkRatio();
    // checkProj();


    // checkSin();
    // checkMul();
    // checkRotation();
    // checkCamera();

    // sweepRecip();

    // sweepSin();
    // checkMul();


    // geometry::Rotation rot = {geometry::Angle(-32.0)};
    // geometry::Coord delta = rot.apply({0.0, 0.0, -32.0});
    // std::cout << delta.z.toDouble() << " " << delta.x.toDouble() << std::endl;


    // geometry::Camera camera;

    // camera.position = {0.0, -0.5, 5.0};

    // // auto res = camera.project({2.0, 0.0, 3.0});
    // geometry::Cube cube2({1.0, 0.0, 1.0}, {2.0, 1.0, 3.0});

    // cube2.calcDistance(camera);

    // for (int i = 0; i < 8; i++) {
    //     std::cout << i << " ";
    //     output(cube2.debugGetVert(i));
    // }
    // std::cout<< std::endl;

    // camera.rotation.heading.adjust(-2.0);

    // cube2.calcDistance(camera);

    // for (int i = 0; i < 8; i++) {
    //     std::cout << i << " ";
    //     output(cube2.debugGetVert(i));
    // }

    //    0, 1, 4

    // std::cout << res.x.toDouble() << " " << res.y.toDouble() << " " << res.z.toDouble() << std::endl;
    

    bool overflow = false;
    std::cout << mulRatio(64.0, 64.0, UnitF::fromRaw(0xffff), overflow).toDouble() << std::endl;;

    // std::cout << "yay" << std::endl;
}