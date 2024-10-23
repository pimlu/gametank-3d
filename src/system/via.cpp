#include "via.h"

#include "types.h"

__attribute__((section(".bss")))
Via via;

extern bool debugIt;

// https://wiki.gametank.zone/doku.php?id=hardware:flashcarts
__attribute__((section(".text.fixed")))
void Via::changeRomBank(uint8_t bankNum) {
    ddra.write(0b00000111);
    iora.write(0);


    *(volatile uint16_t**) 0x2008 = (volatile uint16_t*) &iora;

    // this is like a sequence where it turns the clock (the LSB) on and
    // off while sending data
    for (int i = 7; i >= 0; i--) {

        iora.write(bits::getBit(bankNum, i) ? 2 : 0);
        debugIt = i == 7;
        iora.setBit(0, true);
        debugIt = false;
    }

    iora.setBit(2, true);
    iora.write(0);
}
void Via::profilerStart(uint8_t id) {
    iorb.write(0x80);
    iorb.write(id);
}
void Via::profilerEnd(uint8_t id) {
    iorb.write(0x80);
    iorb.write(id | 0x40);
}
