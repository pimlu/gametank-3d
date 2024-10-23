#pragma once

#include <cstdint>

#include "types.h"

struct Via {
    memreg<0x2800> iorb; // output register b
    memreg<0x2801> iora; // output register a
    memreg<0x2802> ddrb;
    memreg<0x2803> ddra;
    memreg<0x2804> t1cl;
    memreg<0x2805> t1ch;
    memreg<0x2806> t2cl;
    memreg<0x2807> t2ch;
    memreg<0x2808> sr;
    memreg<0x2809> acr;
    memreg<0x280a> pcr;
    memreg<0x280b> ifr;
    memreg<0x280c> era;
    memreg<0x280d> iora_nh;

    void changeRomBank(uint8_t bankNum);
    void profilerStart(uint8_t id);
    void profilerEnd(uint8_t id);
};

extern Via via;