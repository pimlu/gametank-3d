#include "imul.h"
#include <cstdint>


#ifdef IS_6502

extern "C" {
__attribute__((section(".zp.imul")))
volatile uint8_t __imulBuf[20];


struct alignas(256) IMulConsts {
    uint8_t data[2048];
    consteval IMulConsts() : data() {
        for (uint32_t i = 0; i < 512; i++) {
            uint16_t sqr = (i*i) / 4;
            uint16_t negsqr = (255-i)*(255-i)/4;
            // sqrlo
            data[i + 512*0] = sqr & 0xff;
            // sqrhi
            data[i + 512*1] = sqr >> 8;
            // negsqrlo
            data[i + 512*2] = sqr & 0xff;
            // negsqrhi
            data[i + 512*3] = sqr >> 8;
        }
    }
};

__attribute__((section(".rodata")))
const volatile IMulConsts __imulConsts;


using AsmFn = void (*)();
extern AsmFn multiply_16bit_signed, init_multiplication;
}

#endif


namespace geometry {

void initMultiplication() {
#ifdef IS_6502
    asm("jsr init_multiplication");
#endif
}

int32_t imul16To32(int16_t a, int16_t b) {
#ifdef IS_6502
    // documented in imul16.asm
    
    __imulBuf[0] = a & 0xff;
    __imulBuf[10] = (a >> 8) & 0xff;
    __imulBuf[14] = b & 0xff;
    __imulBuf[15] = (b >> 8) & 0xff;

    asm("jsr multiply_16bit_signed");

    volatile int32_t *ptr = (volatile int32_t *) &__imulBuf[16];
    return *ptr;
#else
    return ((int32_t) a) * ((int32_t) b);
#endif
}


}
