
#include <array>
#include "system/bcr.h"
#include "system/scr.h"
#include "system/interrupts.h"
#include "graphics/screen.h"

#include "graphics/triangle.h"
#include "geometry/polygons.h"
#include "geometry/imul.h"

static volatile uint8_t cntr = 0;
static volatile uint8_t slow = 0;

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

    // volatile int16_t raw = geometry::GeoF(1.5).getRaw();
    // geometry::GeoF foo = geometry::GeoF::fromRaw(raw);
    // raw = geometry::GeoF(5.0).getRaw();
    // geometry::GeoF bar = geometry::GeoF::fromRaw(raw);
    // raw = geometry::GeoF(2.5).getRaw();
    // geometry::GeoF baz = geometry::GeoF::fromRaw(raw);
    // *(volatile uint16_t*) 0x2008 = geometry::mulRatio(foo, bar, baz).getRaw();

    int8_t count = 0;
    bool dir = false;
    while (true) {
        bcr.resetIrq();
        waitForInterrupt(); // wait for next interrupt (vblank nmi)

        scr.flipFramebuffer();
        
        graphics::clearScreen(0b001'00'100);
        waitForInterrupt();
        if (dir) {
            count++;
            if (count >= 20) {
                dir = false;
            }
        } else {
            count--;
            if (count <= 0) {
                dir = true;
            }
        }
        
        geometry::GeoF x = geometry::GeoF(-1.0) + geometry::GeoF(0.1).smallIntMult(count);

        geometry::Triangle base = {{-0.25,-0.25,0.0}, {0.25,3.0/4,0.0}, {0.5,-0.5,0.0}};


        geometry::fillTriangle(base.offset({x, 0.8, -3.5}), (uint8_t) ~0b111'01'110);
        geometry::fillTriangle(base.offset({x, 0.4, -3.0}), (uint8_t) ~0b110'01'110);
        geometry::fillTriangle(base.offset({x, 0.0, -2.5}), (uint8_t) ~0b100'01'110);
        geometry::fillTriangle(base.offset({x, -0.4, -2.0}), (uint8_t) ~0b010'01'110);
        geometry::fillTriangle(base.offset({x, -0.8, -1.5}), (uint8_t) ~0b000'01'110);

    }
}