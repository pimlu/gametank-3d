
#include <array>
#include "system/bcr.h"
#include "system/scr.h"
#include "system/interrupts.h"
#include "graphics/screen.h"

#include "graphics/triangle.h"

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

    int8_t count = 0;
    while (true) {
        bcr.resetIrq();
        waitForInterrupt(); // wait for next interrupt (vblank nmi)

        scr.flipFramebuffer();
        
        graphics::clearScreen(0b001'00'100);
        waitForInterrupt();

        if (++count == 10) {
            count = 0;
        }
        for (int8_t x = 0; x < 4; x++) {
            for (int8_t y = 0; y < 2; y++) {
                int8_t dx = x * 20 + count, dy = y * 20;
                graphics::fillTriangle(
                    {(int8_t) (dx + 10), (int8_t) (dy + 10)},
                    {(int8_t) (dx + 20), (int8_t) (dy + 25)},
                    {(int8_t) (dx + 30), (int8_t) (dy + 10)}, ~0b000'01'110);
            }

        }

        // graphics::fillTriangle({10,10}, {80,40}, {50,100}, ~0b000'01'110);
        // graphics::fillTriangle({110,110}, {80,40}, {50,100}, ~0b000'11'100);
    }
}