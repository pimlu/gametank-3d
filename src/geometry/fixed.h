#pragma once

#include <cstdint>
#include <limits>

#include "imul.h"

namespace geometry {
    



// D = data (int16_t or uint16_t), 2D is the 32 bit version
template<typename D, typename D2, uint8_t M>
class Fixed16 {
    using FixedD = Fixed16<D, D2, M>;
    constexpr Fixed16(D data) : data(data) {}
    constexpr D round(double x) {
        D2 res = x >= 0 ? (D2)(x+0.5) : (D2)(x-0.5);
        return (D) res;
    }

public:
    D data;
    constexpr Fixed16() : data(0) {}
    constexpr Fixed16(const Fixed16& o) : data(o.data) {}
    constexpr Fixed16(double value) : data(round(value * (((uint32_t)1) << M))) {}
    constexpr static Fixed16 fromRaw(D data) {
        return Fixed16(data);
    }
    constexpr D getRaw() const {
        return data;
    }

    constexpr double toDouble() const {
        return data / (double) (1 << M);
    }

    constexpr uint8_t lsb() const {
        return data & 0xff;
    }

    constexpr uint8_t msb() const {
        return (data >> 8) & 0xff;
    }
    constexpr static Fixed16 fromRaw(uint8_t lsb, uint8_t msb) {
        D data = (msb << 8) | lsb;
        return Fixed16(data);
    }
    
    constexpr bool operator==(const Fixed16& rhs) const {
        return data == rhs.data;
    }
    constexpr bool operator<(const Fixed16& rhs) const {
        return data < rhs.data;
    }
    constexpr bool operator>(const Fixed16& rhs) const {
        return data > rhs.data;
    }
    constexpr bool operator<=(const Fixed16& rhs) const { return !(*this > rhs); }
    constexpr bool operator>=(const Fixed16& rhs) const { return !(*this < rhs); }
    constexpr bool operator!=(const Fixed16& rhs) const { return !(*this == rhs); }

    constexpr Fixed16 operator+(const Fixed16 &r) const {
        Fixed16 res = *this;
        res += r;
        return res;
    }

    constexpr Fixed16& operator+=(const Fixed16 &r) {
        data += r.data;
        return *this;
    }


    
    constexpr Fixed16 operator-() const {
        return fromRaw(-data);
    }
    constexpr void negate() {
        data = -data;
    }

    constexpr Fixed16 operator-(const Fixed16 &r) const {
        Fixed16 res = *this;
        res -= r;
        return res;
    }

    constexpr Fixed16& operator-=(const Fixed16 &r) {
        data -= r.data;
        return *this;
    }


    constexpr Fixed16 operator*(const Fixed16 &r) const {
        Fixed16 res = *this;
        res *= r;
        return res;
    }

    constexpr Fixed16 smallIntMult(const uint16_t &r) const {
        Fixed16 res = *this;
        res.data *= r;
        return res;
    }

    constexpr Fixed16& operator*=(const Fixed16 &r) {
        // static_assert(M % 8 == 0);
        int32_t res = geometry::imul16To32(data, r.data);
        data = (int16_t) (res >> M);
        return *this;
    }
};


}