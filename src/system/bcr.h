#pragma once
#include "types.h"

// https://wiki.gametank.zone/doku.php?id=hardware:blitter#blitter_registers
struct __attribute__((packed)) Bcr {
    memreg<0x4000> vx;
    memreg<0x4001> vy;
    memreg<0x4002> gx;
    memreg<0x4003> gy;
    memreg<0x4004> width;
    memreg<0x4005> height;
    memreg<0x4006> start;
    memreg<0x4007> color;

    void drawBox(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t c);
    void resetIrq();
};

extern Bcr bcr;
