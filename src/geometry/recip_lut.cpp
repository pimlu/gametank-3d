#include "recip_lut.h"

namespace geometry {

__attribute__((section(".rodata.fixed")))
const RecipLut recipLut;
}
