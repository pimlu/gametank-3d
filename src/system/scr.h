#pragma once
#include "types.h"

#define INPUT_MASK_UP		2056
#define INPUT_MASK_DOWN		1028
#define INPUT_MASK_LEFT		512
#define INPUT_MASK_RIGHT	256
#define INPUT_MASK_A		16
#define INPUT_MASK_B		4096
#define INPUT_MASK_C		8192
#define INPUT_MASK_START	32
#define INPUT_MASK_ALL_KEYS INPUT_MASK_UP|INPUT_MASK_DOWN|INPUT_MASK_LEFT|INPUT_MASK_RIGHT|INPUT_MASK_A|INPUT_MASK_B|INPUT_MASK_C|INPUT_MASK_START

// https://wiki.gametank.zone/doku.php?id=hardware:memorymap#system_control_registers
struct __attribute__((packed)) Scr {
    memreg<0x2000> audioRst;
    memreg<0x2001> audioNmi;
    memreg<0x2005> bankingReg;
    memreg<0x2006> audioCfg;
    memreg<0x2007> videoCfg;
    readreg<0x2008> gamepad1;
    readreg<0x2009> gamepad2;

    void setColorfillMode(bool enabled);
    void flipFramebuffer();
    void setDefaultVideoFlags();
    void setEnableVblankNmi(bool enabled);

    uint16_t readGamepad1();
};

extern Scr scr;
