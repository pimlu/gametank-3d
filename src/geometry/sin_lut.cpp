#include "sin_lut.h"

namespace geometry {

__attribute__((section(".rodata.fixed")))
const SinLut sinLut;

}