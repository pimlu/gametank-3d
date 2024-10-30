#pragma once

#include <cstdint>

inline int8_t mini8(int8_t a, int8_t b) {
    return a <= b ? a : b;
}
inline int8_t maxi8(int8_t a, int8_t b) {
    return a <= b ? b : a;
}
inline uint8_t absi8(int8_t a) {
    return a < 0 ? -a : a;
}
inline uint8_t absDiffi8(int8_t a, int8_t b) {
    if (a >= b) {
        return a - b;
    } else {
        return b - a;
    }
}