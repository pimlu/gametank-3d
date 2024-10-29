
#include <array>
#include "system/bcr.h"
#include "system/scr.h"
#include "system/interrupts.h"
#include "graphics/screen.h"

#include "graphics/triangle.h"
#include "geometry/polygons.h"
#include "geometry/imul.h"

#include "geometry/triangle.h"
#include "geometry/camera.h"

#include "cube.h"


int main() {
    bcr.resetIrq();

    scr.setDefaultVideoFlags();
    
    scr.flipFramebuffer();

    // hue has to be nonzero for some reason to make the blitter work
    uint8_t black = ~0b001'00'000;
    graphics::clearBorder(black);
    scr.flipFramebuffer();
    graphics::clearBorder(black);


    scr.setEnableVblankNmi(true);

    geometry::Camera camera;

    camera.position = {0.0, 0.0, 2.5};

    
    Cube cube({0.0, 0.0, 0.0}, {0.5, 0.5, 0.5});


    Cube cube2({0.5, 0.0, 0.0}, {1.0, 0.5, 0.5});

    // geometry::CachedCoord a = {{-0.25,-0.25,-3.5}};
    // geometry::CachedCoord b = {{0.25,3.0/4,-3.5}};
    // geometry::CachedCoord c = {{0.5,-0.5,-3.5}};
    // geometry::Triangle t = {&a, &b, &c};


    while (true) {
        bcr.resetIrq();
        waitForInterrupt(); // wait for next interrupt (vblank nmi)

        scr.flipFramebuffer();
        
        graphics::clearScreen(0b001'00'100);
        waitForInterrupt();

        uint16_t gamepad1 = scr.readGamepad1();

        camera.tickFrame();
        camera.updateFromGamepad(gamepad1);

        // auto fillAt = [](graphics::ScreenPos pos, uint8_t color) {
        //     graphics::fillTriangle(pos, {(int8_t)(pos.x+3),(int8_t)(pos.y+3)}, {(int8_t)(pos.x+3),pos.y}, color);
        // };





        // if (dir) {
        //     count++;
        //     if (count >= 20) {
        //         dir = false;
        //     }
        // } else {
        //     count--;
        //     if (count <= 0) {
        //         dir = true;
        //     }
        // }
        
        // geometry::GeoF x = geometry::GeoF(-1.0) + geometry::GeoF(0.1).smallIntMult(count);

        // geometry::Triangle base = {{-0.25,-0.25,0.0}, {0.25,3.0/4,0.0}, {0.5,-0.5,0.0}};


        // geometry::fillTriangle(base.offset({x, 0.8, -3.5}), (uint8_t) ~0b111'01'110);
        // geometry::fillTriangle(base.offset({x, 0.4, -3.0}), (uint8_t) ~0b110'01'110);
        // geometry::fillTriangle(base.offset({x, 0.0, -2.5}), (uint8_t) ~0b100'01'110);
        // geometry::fillTriangle(base.offset({x, -0.4, -2.0}), (uint8_t) ~0b010'01'110);
        // geometry::fillTriangle(base.offset({x, -0.8, -1.5}), (uint8_t) ~0b000'01'110);

        auto dist1 = cube.calcDistance(camera);
        auto dist2 = cube2.calcDistance(camera);
        if (dist1 < dist2) {
            cube2.paint(camera);
            cube.paint(camera);
        } else {
            cube.paint(camera);
            cube2.paint(camera);
        }
        // geometry::fillTriangle(camera, t, (uint8_t) ~0b000'01'110);
        // fillAt({0, 0}, black);

        // fillAt({(int8_t)(64 + roundGeoF(camera.position.x * 2.0)), (int8_t)(64 + roundGeoF(camera.position.z * 2.0))}, black);

        bcr.drawBox(10, 10, roundGeoF(dist1 * 10.0), 1, black);

        bcr.drawBox(64, 64, 1, 1, black);



        geometry::Coord delta = camera.rotation.apply({0.0, 0.0, -2.0});
        bcr.drawBox(64 + roundGeoF(camera.position.z * 5.0), 64 + roundGeoF(camera.position.x * 5.0), 1, 1, black);
        bcr.drawBox(64 + roundGeoF(camera.position.z * 5.0 + delta.z), 64 + roundGeoF(camera.position.x * 5.0 + delta.x), 1, 1, black);
    }
}