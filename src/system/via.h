#pragma once

#include <cstdint>

#include "types.h"

volatile struct Via {
    memreg iorb; // output register b
    memreg iora; // output register a
    memreg ddrb;
    memreg ddra;
    memreg t1cl;
    memreg t1ch;
    memreg t2cl;
    memreg t2ch;
    memreg sr;
    memreg acr;
    memreg pcr;
    memreg ifr;
    memreg era;
    memreg iora_nh;

    void changeRomBank(uint8_t bankNum);
    void profilerStart(uint8_t id);
    void profilerEnd(uint8_t id);
};

Via *via;