#include "scr.h"


__attribute__((section(".bss")))
Scr scr;

void Scr::setColorfillMode(bool enabled) {
    videoCfg.setBit(3, enabled);
}
void Scr::flipFramebuffer() {
    // DMA_PAGE_OUT - Select framebuffer page sent to TV
    videoCfg.toggleBit(1);
    // "Select which framebuffer to read/write/blit"
    bankingReg.toggleBit(3);
}
void Scr::setDefaultVideoFlags() {
    videoCfg.write(0b01101101);
}
void Scr::setEnableVblankNmi(bool enabled) {
    videoCfg.setBit(2, enabled);
}