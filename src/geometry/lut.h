#pragma once

#include <cstdint>

#include "fixed_etc.h"

namespace geometry {

constexpr uint8_t bitShift(int16_t x, uint8_t shift = 0) {
    return x <= 1 ? shift : bitShift(x >> 1, shift + 1);
}

// step bits should be <= 8
template<typename Inp, Inp START, Inp STEP, typename Out>
struct alignas(256) Lut {
    static constexpr int16_t STEP_RAW = STEP.getRaw();
    static constexpr uint8_t STEP_BITS = bitShift(STEP_RAW);
    static constexpr Inp END = START + STEP.smallIntMult(256);
    static constexpr uint8_t STEP_MASK = STEP_RAW - 1;

    static_assert (STEP_RAW == (1 << STEP_BITS));

    constexpr Lut() : lsb(), msb(), last(0.0) {
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
    Out last;

    constexpr void setEntry(uint8_t idx, Out val) {
        lsb[idx] = val.lsb();
        msb[idx] = val.msb();
    }
    constexpr void setLast(Out val) {
        last = val;
    }
    


    Out getEntry(uint8_t idx) const {
        return Out::fromRaw(lsb[idx], msb[idx]);
    }

    Out lerp (Out a, Out b, uint8_t frac) const {
        // std::cout << "lerp" << std::endl;
        // std::cout << (int) frac << std::endl;
        constexpr uint8_t REM_BITS = 8 - STEP_BITS;
        uint8_t invFrac = (1 << STEP_BITS) - frac;

        // this only exists for UnitF right now...
        Out aPart = scaleByUint8(a, invFrac << REM_BITS);
        Out bPart = scaleByUint8(b, frac << REM_BITS);
        return aPart + bPart;
    }

    Out lookupOrConst(Inp x, Out loMiss, Out hiMiss) const {
        if (x < START) return loMiss;
        if (END != START) {
            if (x == END) return last;
            else if (x > END) return hiMiss;
        }
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
        Out next = last;
        if (idx != 255) {
            [[likely]]
            next = getEntry(idx+1);
        }
        Out res = lerp(getEntry(idx), next, frac);

        return res;
    }
};


}