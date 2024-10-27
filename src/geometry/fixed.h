#pragma once

#include <cstdint>
#include <limits>

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

    template<typename Dp, typename D2p, int8_t Mp>
    constexpr void shiftMulEqImpl(const Fixed16<Dp, D2p, Mp> &r) {
        // static_assert(Mp % 8 == 0);
        int32_t res = ((int32_t) data) * ((int32_t) r.data);
        data = (int16_t) (res >> Mp);
    }

public:
    D data;
    Fixed16() = delete;
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
        return *(uint8_t*) &data;
    }

    constexpr uint8_t msb() const {
        auto ptr = (uint8_t*) &data;
        return ptr[1];
    }
    constexpr static Fixed16 fromRaw(uint8_t lsb, uint8_t msb) {
        int16_t data = 0;
        auto ptr = (uint8_t*) &data;
        ptr[0] = lsb;
        ptr[1] = msb;
        return fromRaw(data);
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
        return Fixed16(-data);
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
        int32_t res = ((int32_t) data) * ((int32_t) r.data);
        data = (int16_t) (res >> M);
        return *this;
    }

    template<uint8_t Mp>
    constexpr void shiftMulEq(const Fixed16<uint16_t, uint32_t, Mp> &r) {
        shiftMulEqImpl<uint16_t, uint32_t, Mp>(r);
    }
    template<uint8_t Mp>
    constexpr void shiftMulEq(const Fixed16<int16_t, int32_t, Mp> &r) {
        shiftMulEqImpl<int16_t, int32_t, Mp>(r);
    }
    template<uint8_t Mp>
    constexpr Fixed16 shiftMul(const Fixed16<uint16_t, uint32_t, Mp> &r) {
        Fixed16 res = *this;
        res.shiftMulEq(r);
        return res;
    }
    template<uint8_t Mp>
    constexpr Fixed16 shiftMul(const Fixed16<int16_t, int32_t, Mp> &r) {
        Fixed16 res = *this;
        res.shiftMulEq(r);
        return res;
    }


    constexpr void scaleByUint8(uint8_t scale) {
        int32_t res = ((int32_t) data) *((int32_t) scale);
        data = (int16_t) (res >> 8);
    }
};


}