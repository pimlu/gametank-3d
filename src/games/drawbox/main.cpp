
#include <array>
#include "system/bcr.h"
#include "system/scr.h"
#include "system/interrupts.h"

uint16_t doAddSub(uint8_t a, uint8_t b, uint8_t c) {
    uint8_t sum;
    bool carry = __builtin_add_overflow(a, b, &sum);
    return a + b;
}

int main() {
    bcr.resetIrq();

    scr.setDefaultVideoFlags();

    while (true) {
        waitForInterrupt(); // wait for next interrupt (vblank nmi)

        scr.flipFramebuffer();

        // bcr.drawBox(0, 0, 127, 127, 0b010'00'100);
        // waitForInterrupt();

        // bcr.drawBox(127, 0, 1, 127, 0b010'00'100);
        // waitForInterrupt();

        bcr.drawBox(0, 30, 8, 8, 92);
        waitForInterrupt();

        // bcr.drawBox(127, 127, 1, 1, 0b010'00'100);
        // waitForInterrupt();
    }
}