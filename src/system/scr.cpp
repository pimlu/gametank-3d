#include "scr.h"


Scr *scr = (Scr*) 0x2000;

void Scr::setColorfillMode(bool enabled) {
    videoCfg = updateBit(videoCfg, 3, enabled);
}
void Scr::flipFramebuffer() {
    videoCfg = toggleBit(videoCfg, 1);
    // @stu what is this?
    bankingReg = toggleBit(bankingReg, 3);
}
void Scr::setEnableVblankNmi(bool enabled) {
    videoCfg = updateBit(videoCfg, 2, enabled);
}