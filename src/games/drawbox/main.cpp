
#include <array>
#include "system/bcr.h"
#include "system/scr.h"
#include "system/interrupts.h"
#include "graphics/screen.h"

#include <graphics/bresenham.h>

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

    while (true) {
        bcr.resetIrq();
        waitForInterrupt(); // wait for next interrupt (vblank nmi)

        scr.flipFramebuffer();
        
        graphics::clearScreen(0b001'00'100);
        waitForInterrupt();


        graphics::fillTriangle({10,10}, {80,40}, {50,100}, [](uint8_t y, uint8_t xLeft, uint8_t xRight) {
            bcr.drawBox(xLeft, y, xRight - xLeft, 1, ~0b000'01'110);
            waitForInterrupt();
        });

        graphics::fillTriangle({110,110}, {80,40}, {50,100}, [&](uint8_t y, uint8_t xLeft, uint8_t xRight) {
            bcr.drawBox(xLeft, y, xRight - xLeft, 1, ~0b000'11'100);
            waitForInterrupt();
        });
    }
}