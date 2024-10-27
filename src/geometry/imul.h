#pragma once

#include <cstdint>

namespace geometry {

void initMultiplication();

uint32_t mul8To16(uint16_t a, uint16_t b);
uint32_t mul16To32(uint16_t a, uint16_t b);
uint64_t mul32To64(uint32_t a, uint32_t b);

int32_t imul16To32(int16_t x, int16_t y);
int64_t imul32To64(int32_t x, int32_t y);

}