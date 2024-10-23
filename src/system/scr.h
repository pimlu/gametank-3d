#pragma once
#include "types.h"

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
};

extern Scr scr;
