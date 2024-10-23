#pragma once

#include <cstdint>
#include <array>

namespace bits {
    uint8_t updateBit(uint8_t reg, uint8_t bit, bool val);
    uint8_t toggleBit(uint8_t reg, uint8_t bit);
    bool getBit(uint8_t reg, uint8_t bit);
    uint8_t oneShiftN(uint8_t n);
}

template<uint16_t ADDR>
struct memreg {
    uint8_t mirror = 0;

    // uint8_t operator=(const uint8_t &rhs) {
    //     mirror = rhs;
    //     *(volatile uint8_t*) 0x2009 = 0xfe;
    //     *(volatile uint8_t*) 0x2008 = mirror;
    //     sync();
    //     return mirror;
    // }
    __attribute__((section(".text.fixed")))
    inline void write(uint8_t val) {
        // write ADDR first. this is important for the memory bank register
        // because the mirror will be affected
        *(volatile uint8_t*) ADDR = val;
        // treat just the write as volatile since I'm okay with read-side
        // optimizations
        *(volatile uint8_t*) &mirror = val;
    }

    __attribute__((section(".text.fixed")))
    inline uint8_t setBit(uint8_t bit, bool val) {
        uint8_t res = bits::updateBit(mirror, bit, val);
        write(res);
        return res;
    }
    __attribute__((section(".text.fixed")))
    inline uint8_t toggleBit(uint8_t bit) {
        uint8_t res = bits::toggleBit(mirror, bit);
        write(res);
        return res;
    }
    __attribute__((section(".text.fixed")))
    inline bool getBit(uint8_t bit) {
        return bits::getBit(mirror, bit);
    }
};


template<uint16_t ADDR>
struct readreg {
    uint8_t read() {
        volatile uint8_t *ptr = (volatile uint8_t*) ADDR;
        return *ptr;
    }
};
