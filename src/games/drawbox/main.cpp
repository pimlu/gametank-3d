
#include <array>
#include "system/bcr.h"
#include "system/scr.h"
#include "system/interrupts.h"
#include "graphics/screen.h"

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


        for (uint8_t i = 0; i < 40; i++) {
            bcr.drawBox(70-i, 70-i, 1, 1, ~0b000'00'010);
            waitForInterrupt();
        }
    }
}