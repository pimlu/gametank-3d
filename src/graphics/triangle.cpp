#include "triangle.h"

#include "bresenham.h"
#include "screen.h"
#include "system/bcr.h"

namespace graphics {

void fillTriangle(ScreenPos a, ScreenPos b, ScreenPos c, uint8_t color) {

    bool shouldWait = false;

    bcr.setupRowFill(color);
    fillTriangleGeneric<Bresenham>(a, b, c, [&](int8_t y, int8_t xLeft, int8_t xRight) {
        // change y to point up
        y = -y;
        // check for clipping and remove
        if (y < SCENE_Y_LO || y >= SCENE_Y_HI) {
            return;
        }
        xLeft = maxi8(xLeft, SCENE_X_LO);
        xRight = mini8(xRight, SCENE_X_HI);
        if (xLeft >= xRight) {
            return;
        }
        int8_t width = xRight - xLeft;

        // switch from scene coordinates (origin at center of screen, -y) to
        // frame coordinates (what the blitter uses, origin at top left) 
        xLeft += SCENE_X_TO_FRAME;
        y += SCENE_Y_TO_FRAME;

        // it runs bresenham while the blitter is busy, so we wait before
        // calling the blitter, not after
        if (shouldWait) {
            bcr.rowFillWait();
        } else {
            shouldWait = true;
        }

        bcr.triggerRowFill(xLeft, y, width);
    });
    if (shouldWait) {
        bcr.rowFillWait();
    }
}

}