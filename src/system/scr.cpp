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
    // DMA_PAGE_OUT (framebuffer) should be opposite the blitter target in order
    // to prevent screen tearing
    videoCfg.write(0b01101111);
}
void Scr::setEnableVblankNmi(bool enabled) {
    videoCfg.setBit(2, enabled);
}

uint16_t Scr::readGamepad1() {
    // according to https://gametank.zone/manual/, reset the state via reading 2 first
    scr.gamepad2.read();
    
    uint8_t lo = scr.gamepad1.read();
    uint8_t hi = scr.gamepad1.read();
    uint16_t combo = (hi << 8) | lo;

    return ~combo;
}