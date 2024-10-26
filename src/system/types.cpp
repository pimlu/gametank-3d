#include "types.h"

#include "interrupts.h"

__attribute__((section(".rodata.fixed")))
uint8_t lut[8] = {
    0b1,
    0b10,
    0b100,
    0b1000,
    0b10000,
    0b100000,
    0b1000000,
    0b10000000
};
// llvm-mos generates attrocious code for 1 << bit so fix that
__attribute__((section(".text.fixed")))
uint8_t bits::oneShiftN(uint8_t n) {
    if (n >= 8) {
        panicPls();
    }
    return lut[n];
}

__attribute__((section(".text.fixed")))
uint8_t bits::updateBit(uint8_t reg, uint8_t bit, bool val) {
    if (val) {
        return reg | oneShiftN(bit);
    } else {
        return reg & ~oneShiftN(bit);
    }
}
__attribute__((section(".text.fixed")))
uint8_t bits::toggleBit(uint8_t reg, uint8_t bit) {
    return reg ^ oneShiftN(bit);
}
__attribute__((section(".text.fixed")))
bool bits::getBit(uint8_t reg, uint8_t bit) {
    return reg & oneShiftN(bit);
}