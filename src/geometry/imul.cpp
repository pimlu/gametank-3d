#include "imul.h"
#include <cstdint>


#ifdef IS_6502

extern "C" {
 
uint16_t __multiplyu8Tou16(uint8_t a, uint8_t b);

__attribute__((section(".zp.mul")))
volatile uint8_t __imulBuf[3];

struct alignas(256) __attribute__ ((packed))  IMul8Consts {
    uint8_t data[512];
    consteval IMul8Consts() : data() {
        for (uint32_t i = 0; i < 256; i++) {
            uint16_t sqr = i*i;
            // sqrlo
            data[i + 256*0] = sqr & 0xff;
            // sqrhi
            data[i + 256*1] = sqr >> 8;
        }
    }
};


__attribute__((section(".rodata.fixed")))
const volatile IMul8Consts __imulConsts;


using AsmFn = void (*)();
extern AsmFn multiply_16bit_signed, init_multiplication;
}

#endif


namespace geometry {

void initMultiplication() {
#ifdef IS_6502
    //asm("jsr init_multiplication");
#endif
}

uint16_t mul8To16(uint8_t x, uint8_t y) {
#ifdef IS_6502
    uint16_t swapped = __multiplyu8Tou16(x, y);
    uint8_t lo = swapped & 0xff, hi = swapped >> 8;
    return (lo << 8) | hi;
#else
    return ((uint16_t) x) * ((uint16_t) y);
#endif
}

uint32_t mul16To32(uint16_t x, uint16_t y) {
    // TODO karatsuba
    uint8_t x0 = x & 0xff, x1 = x >> 8;
    uint8_t y0 = y & 0xff, y1 = y >> 8;

    uint32_t z0 = mul8To16(x0, y0);
    uint32_t z1 = mul8To16(x0, y1) + (uint32_t) mul8To16(x1, y0);
    z1 <<= 8;
    uint32_t z2 = mul8To16(x1, y1);
    z2 <<= 16;

    return z0 + z1 + z2;
}
uint64_t mul32To64(uint32_t x, uint32_t y) {
    // TODO karatsuba
    uint16_t x0 = x & 0xffff, x1 = x >> 16;
    uint16_t y0 = y & 0xffff, y1 = y >> 16;

    uint64_t z0 = mul16To32(x0, y0);
    uint64_t z1 = mul16To32(x0, y1) + (uint64_t) mul16To32(x1, y0);
    z1 <<= 16;
    uint64_t z2 = mul16To32(x1, y1);
    z2 <<= 32;

    return z0 + z1 + z2;
}

// TODO overflow probably handled wrong
int32_t imul16To32(int16_t x, int16_t y) {
    bool neg = false;
    uint16_t ux, uy;
    if (x < 0) {
        neg ^= true;
        ux = -(uint16_t)x;
    } else {
        ux = x;
    }
    if (y < 0) {
        neg ^= true;
        uy = -(uint16_t)y;
    } else {
        uy = y;
    }
    int32_t res = (int32_t) mul16To32(ux, uy);
    return neg ? -res : res;
}

int64_t imul32To64(int32_t x, int32_t y) {
    bool neg = false;
    uint32_t ux, uy;
    if (x < 0) {
        neg ^= true;
        ux = -(uint32_t)x;
    } else {
        ux = x;
    }
    if (y < 0) {
        neg ^= true;
        uy = -(uint32_t)y;
    } else {
        uy = y;
    }
    int64_t res = (int64_t) mul32To64(ux, uy);
    return neg ? -res : res;
}
}
