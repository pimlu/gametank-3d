#pragma once

#include <cstdint>
// #include <iostream>

namespace geometry {

constexpr uint8_t bitShift(int16_t x, uint8_t shift = 0) {
    return x <= 1 ? shift : bitShift(x >> 1, shift + 1);
}

// step bits should be <= 8
template<typename Inp, Inp START, Inp STEP, typename Out>
struct Lut {
    static constexpr int16_t STEP_RAW = STEP.getRaw();
    static constexpr uint8_t STEP_BITS = bitShift(STEP_RAW);
    static constexpr Inp END = START + STEP.smallIntMult(256);
    static constexpr uint8_t STEP_MASK = STEP_RAW - 1;

    static_assert (STEP_RAW == (1 << STEP_BITS));

    constexpr Lut() : lsb(), msb() {
        // std::cout << "lut" << std::endl;
        // std::cout << (int) STEP_RAW << std::endl;
        // std::cout << (int) STEP_BITS << std::endl;
        // std::cout << END.toDouble() << std::endl;
        // std::cout << (int) STEP_MASK << std::endl;
        // std::cout << "end lut, values" << std::endl;
        // std::cout << getEntry(0).toDouble() << std::endl;
    }
    uint8_t lsb[256];
    uint8_t msb[256];

    constexpr void setEntry(uint8_t idx, Out val) {
        lsb[idx] = val.lsb();
        msb[idx] = val.msb();
    }


    Out getEntry(uint8_t idx) const {
        return Out::fromRaw(lsb[idx], msb[idx]);
    }

    Out lerp (Out a, Out b, uint8_t frac) const {
        // std::cout << "lerp" << std::endl;
        // std::cout << (int) frac << std::endl;
        constexpr uint8_t REM_BITS = 8 - STEP_BITS;
        uint8_t invFrac = ((int16_t) 256) - frac;
        a.scaleByUint8(invFrac << REM_BITS);
        b.scaleByUint8(frac << REM_BITS);
        return a + b;
    }

    Out lookupOrConst(Inp x, Out missVal) const {
        if (x < START) return missVal;
        if (END != START && x >= END) return missVal;
        Inp shifted = x - START;
        int16_t shiftedBits = shifted.getRaw();
        // // std::cout << "lookup " << shifted.toDouble() << std::endl;
        // // std::cout << "bits" << (int) shiftedBits<< std::endl;
        uint8_t idx = shiftedBits >> STEP_BITS;
        uint8_t frac = shiftedBits & STEP_MASK;

        // *(volatile int16_t*) 0x2008 = 0xbeef;
        if (!frac) {
            return getEntry(idx);
        }
        Out res = lerp(getEntry(idx), getEntry(idx+1), frac);

        return res;
    }
};


}