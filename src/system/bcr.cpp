#include "bcr.h"

#include "scr.h"

__attribute__((section(".bss")))
Bcr bcr;

void Bcr::drawBox(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t c) {
    asm("sei");

    scr.setColorfillMode(true);

    vx.write(x);
    vy.write(y);
    width.write(w);
    height.write(h);
    color.write(c);
    
    start.write(1);
    asm("cli");
}
void Bcr::resetIrq() {
    start.write(0);
}