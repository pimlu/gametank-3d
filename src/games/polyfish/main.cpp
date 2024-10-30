
#include <array>
#include "system/bcr.h"
#include "system/scr.h"
#include "system/interrupts.h"
#include "graphics/screen.h"

#include "graphics/triangle.h"
#include "geometry/polygons.h"
#include "system/imul.h"

#include "geometry/sin_lut.h"

#include "geometry/triangle.h"
#include "geometry/camera.h"

#include "geometry/cube.h"

#include "fish.h"


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

    camera.rotation.heading.setTheta(5.0);
    camera.rotation.pitch.setTheta(-12.0);

    camera.position = {3.0, 4.0, 5.0};
    // camera.rotation.heading

    uint8_t cubeColors[6] = {
        (uint8_t) ~0b000'01'101,
        (uint8_t) ~0b000'01'101,
        (uint8_t) ~0b000'01'010,
        (uint8_t) ~0b000'01'110,
        (uint8_t) ~0b000'01'101,
        (uint8_t) ~0b000'01'100
    };

    geometry::Cube cube({0.0, 0.0, 0.0}, {2.0, 2.0, 2.0}, cubeColors);
    uint8_t fishColors[3] = {
        (uint8_t) ~0b011'11'100,
        (uint8_t) ~0b011'11'101,
        (uint8_t) ~0b011'10'010,
    };
    Fish fish1({0.0, 1.0, 0.0}, geometry::Angle(), fishColors);


    while (true) {
        bcr.resetIrq();
        waitForInterrupt(); // wait for next interrupt (vblank nmi)

        scr.flipFramebuffer();
        
        graphics::clearScreen(0b001'00'100);
        waitForInterrupt();

        uint16_t gamepad1 = scr.readGamepad1();

        camera.tickFrame();
        camera.updateFromGamepad(gamepad1);


        auto cubeDist = cube.calcDistance(camera);
        
        auto fishDist = fish1.calcDistance(camera);


        if (cubeDist < fishDist) {
            fish1.paint(camera);
            cube.paint(camera);
        } else {
            cube.paint(camera);
            fish1.paint(camera);
        }

        // cube.paint(camera);

    }
}