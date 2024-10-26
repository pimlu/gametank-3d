
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
    graphics::clearBorder(~0b001'00'000);

        bcr.drawBox(
            graphics::FRAME_X_LO, graphics::FRAME_Y_LO,
            graphics::FRAME_W, graphics::FRAME_H,
            0b010'00'100);
        waitForInterrupt();
    scr.flipFramebuffer();
    // hue has to be nonzero for some reason to make the blitter work
    graphics::clearBorder(~0b001'00'000);
        bcr.drawBox(
            graphics::FRAME_X_LO, graphics::FRAME_Y_LO,
            graphics::FRAME_W, graphics::FRAME_H,
            0b010'00'100);
        waitForInterrupt();


    scr.setEnableVblankNmi(true);

    


    while (true) {
        bcr.resetIrq();
        waitForInterrupt(); // wait for next interrupt (vblank nmi)

        // if (cntr >= 2) {
        //     if (slow++ == 0) {
        //         scr.flipFramebuffer();
        //     }
        //     continue;
        // }
        // cntr++;
        scr.flipFramebuffer();


        // DMA_PAGE_OUT - Select framebuffer page sent to TV
        // scr.videoCfg.toggleBit(1);
        // // "Select which framebuffer to read/write/blit"
        // scr.bankingReg.toggleBit(3);
        
        // graphics::clearBorder(~0);

        
        bcr.drawBox(
            graphics::FRAME_X_LO, graphics::FRAME_Y_LO,
            graphics::FRAME_W, graphics::FRAME_H,
            0b001'00'100);
        waitForInterrupt();


        for (uint8_t i = 0; i < 40; i++) {
            bcr.drawBox(70-i, 70-i, 1, 1, ~0b000'00'010);
            waitForInterrupt();
        }
        // bcr.drawBox(127, 127, 1, 1, 0b010'00'100);
        // waitForInterrupt();
    }
}