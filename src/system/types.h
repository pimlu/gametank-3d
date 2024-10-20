#pragma once

#include <cstdint>
#include <array>

using memreg = volatile uint8_t;

uint8_t updateBit(uint8_t reg, uint8_t bit, bool val);
uint8_t toggleBit(uint8_t reg, uint8_t bit);
bool getBit(uint8_t reg, uint8_t bit);