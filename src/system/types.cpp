#include "types.h"

uint8_t updateBit(uint8_t reg, uint8_t bit, bool val) {
    if (val) {
        return reg | (1 << bit);
    } else {
        return reg & ~(1 << bit);
    }
}
uint8_t toggleBit(uint8_t reg, uint8_t bit) {
    return reg ^ (1 << bit);
}
bool getBit(uint8_t reg, uint8_t bit) {
    return reg & (1 << bit);
}