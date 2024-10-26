
#include <array>
#include "system/bcr.h"
#include "system/scr.h"
#include "system/interrupts.h"
#include "graphics/screen.h"


int main() {
    bcr.resetIrq();

    scr.setDefaultVideoFlags();
    
    scr.flipFramebuffer();
    // hue has to be nonzero for some reason to make the blitter work
    graphics::clearBorder(~0b001'00'000);
    scr.flipFramebuffer();
    // hue has to be nonzero for some reason to make the blitter work
    graphics::clearBorder(~0b001'00'000);


    scr.setEnableVblankNmi(true);

    while (true) {
        bcr.resetIrq();
        waitForInterrupt(); // wait for next interrupt (vblank nmi)

        scr.flipFramebuffer();

        // DMA_PAGE_OUT - Select framebuffer page sent to TV
        // scr.videoCfg.toggleBit(1);
        // // "Select which framebuffer to read/write/blit"
        // scr.bankingReg.toggleBit(3);
        
        // graphics::clearBorder(~0);

        bcr.drawBox(
            graphics::FRAME_X_LO, graphics::FRAME_Y_LO,
            graphics::FRAME_W, graphics::FRAME_H,
            0b010'00'100);
        waitForInterrupt();

        *(volatile uint8_t*) 0x2008 = 124;
        bcr.drawBox(30, 30, 80, 80, ~0b000'00'010);
        waitForInterrupt();

        // bcr.drawBox(127, 127, 1, 1, 0b010'00'100);
        // waitForInterrupt();
    }
}