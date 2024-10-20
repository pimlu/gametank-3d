#include "main.h"

#include "system/bcr.h"
#include "system/scr.h"
#include "system/interrupts.h"

void main() {
    bcr->resetIrq();

    scr->setEnableVblankNmi(true);

    while (true) {
        waitForInterrupt(); // wait for next interrupt (vblank nmi)

        scr->flipFramebuffer();

        bcr->drawBox(0, 0, 127, 127, 0b010'00'100);
        waitForInterrupt();
    }

}