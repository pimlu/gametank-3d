#include "fixed.h"


namespace graphics {


bool Fixed8d8::operator==(const Fixed8d8& rhs) {
    return data == rhs.data;
}
bool Fixed8d8::operator<(const Fixed8d8& rhs) {
    return data < rhs.data;
}
bool Fixed8d8::operator>(const Fixed8d8& rhs) {
    return data > rhs.data;
}

Fixed8d8 Fixed8d8::operator+(const Fixed8d8 &r) {
    Fixed8d8 res = *this;
    res += r;
    return res;
}

Fixed8d8& Fixed8d8::operator+=(const Fixed8d8 &r) {
    data += r.data;
    return *this;
}


Fixed8d8 Fixed8d8::operator*(const Fixed8d8 &r) {
    Fixed8d8 res = *this;
    res *= r;
    return res;
}

Fixed8d8& Fixed8d8::operator*=(const Fixed8d8 &r) {
    int32_t res = ((int32_t) data) * ((int32_t) r.data);
    data = (int16_t) (res >> 8);
    return *this;
}

}