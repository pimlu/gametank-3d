#pragma once
#include "types.h"

// https://wiki.gametank.zone/doku.php?id=hardware:blitter#blitter_registers
__attribute__((packed))
struct Bcr {
    memreg vx;
    memreg vy;
    memreg gx;
    memreg gy;
    memreg width;
    memreg height;
    memreg start;
    memreg color;

    void drawBox(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t c);
    void resetIrq();
};

Bcr *bcr;
