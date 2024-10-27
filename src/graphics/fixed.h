#pragma once

#include <cstdint>

namespace graphics {
    

constexpr int16_t roundi16(double x) {
    return x >= 0 ? (int16_t)(x+0.5) : (int16_t)(x-0.5);
}

class Fixed8d8 {
public:
    int16_t data;
    Fixed8d8() = delete;
    constexpr Fixed8d8(const Fixed8d8& o) : data(o.data) {}
    constexpr Fixed8d8(double value) : data(roundi16(value * 256)) {}
    
    bool operator==(const Fixed8d8& rhs);
    bool operator<(const Fixed8d8& rhs);
    bool operator>(const Fixed8d8& rhs);
    inline bool operator<=(const Fixed8d8& rhs) { return !(*this > rhs); }
    inline bool operator>=(const Fixed8d8& rhs) { return !(*this < rhs); }
    inline bool operator!=(const Fixed8d8& rhs) { return !(*this == rhs); }

    Fixed8d8 operator+(const Fixed8d8 &r);
    Fixed8d8& operator+=(const Fixed8d8 &r);

    Fixed8d8 operator*(const Fixed8d8 &r);
    Fixed8d8& operator*=(const Fixed8d8 &r);
};

}