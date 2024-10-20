#pragma once
#include "types.h"

// https://wiki.gametank.zone/doku.php?id=hardware:memorymap#system_control_registers
__attribute__((packed))
struct Scr {
    memreg audioRst;
    memreg audioNmi;

private:
    memreg __unused[3];
public:

    memreg bankingReg;
    memreg audioCfg;
    memreg videoCfg;
    memreg gamepad1;
    memreg gamepad2;

    void setColorfillMode(bool enabled);
    void flipFramebuffer();
    void setEnableVblankNmi(bool enabled);
};

Scr *scr;
