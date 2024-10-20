#include <via.h>

#include "types.h"

Via *via = (Via*) 0x2800;

// https://wiki.gametank.zone/doku.php?id=hardware:flashcarts
__attribute__((section("text.fixed")))
void Via::changeRomBank(uint8_t bankNum) {
    ddra = 0b00000111;
    iora = 0;

    // this is like a sequence where it turns the clock (the LSB) on and
    // off while sending data
    for (int i = 7; i >= 0; i--) {
        iora = getBit(bankNum, i) << 1;
        iora |= 1;
    }

    iora |= 1 << 2;
    iora = 0;
}
void Via::profilerStart(uint8_t id) {
    iorb = 0x80;
    iorb = id;
}
void Via::profilerEnd(uint8_t id) {
    iorb = 0x80;
    iorb = id | 0x40;
}
