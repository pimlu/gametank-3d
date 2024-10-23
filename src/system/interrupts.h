#pragma once

extern "C" {
    void waitForInterrupt();
    void vblankNmi();
    void blitIsr();
    void panicPls();
}