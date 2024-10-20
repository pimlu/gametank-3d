#include "bcr.h"

#include "scr.h"

Bcr *bcr = (Bcr*) 0x4000;

void Bcr::drawBox(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t c) {
    scr->setColorfillMode(true);

    vx = x;
    vy = y;
    width = w;
    height = h;
    color = c;
    
    start = 1;
}
void Bcr::resetIrq() {
    start = 0;
}