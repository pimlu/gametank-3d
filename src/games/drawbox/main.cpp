
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
    while (true) {
        bcr.resetIrq();
        waitForInterrupt(); // wait for next interrupt (vblank nmi)

        scr.flipFramebuffer();
        
        graphics::clearScreen(0b001'00'100);
        waitForInterrupt();

        if (count++ == 10) {
            count = 0;
        }
        // geometry::initMultiplication();

        // int8_t basisX = -50;
        // int8_t basisY = -50;
        // for (int8_t x = 0; x < 6; x++) {
        //     for (int8_t y = 0; y < 6; y++) {
        //         int8_t dx = x * 15 + count, dy = y * 15;
        //         graphics::fillTriangle(
        //             {(int8_t) (basisX + dx), (int8_t) (basisY + dy)},
        //             {(int8_t) (basisX + dx + 10), (int8_t) (basisY + dy + 15)},
        //             {(int8_t) (basisX + dx + 40), (int8_t) (basisY + dy)}, ~0b000'01'110);
        //     }
        // }
        
        geometry::GeoF z = geometry::GeoF(-4.0) + geometry::GeoF(-0.5).smallIntMult(count);

        // *(volatile uint8_t*) 0x2009 = count + 1;

        geometry::fillTriangle({-1.0,-1.0,z}, {1.0,3.0,z}, {2.0,-2.0,z}, ~0b000'01'110);
        geometry::fillTriangle({-1.0,-1.0,z}, {1.0,3.0,z}, {2.0,-2.0,z}, ~0b000'01'110);
        geometry::fillTriangle({-1.0,-1.0,z}, {1.0,3.0,z}, {2.0,-2.0,z}, ~0b000'01'110);

        // graphics::fillTriangle({-24, -24}, {24, 73}, {49, -49}, ~0b000'01'110);

        // graphics::fillTriangle({10,10}, {80,40}, {50,100}, ~0b000'01'110);
        // graphics::fillTriangle({110,110}, {80,40}, {50,100}, ~0b000'11'100);
    }
}