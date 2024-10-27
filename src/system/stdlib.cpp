#include <cstdint>
// for now, trigger a compiler error if this is gone

// extern "C" void memcpy(void* dst, const void* src, int16_t n) {
//     char *dstC = (char*) dst;
//     const char *srcC = (const char*) src;
//     *(volatile uint16_t*) 0x2008 = n;
//     for (int16_t i = 0; i < n; i++) {
//         *(dstC++) = *(srcC++);
//     }
// }