#include "triangle.h"

#include "bresenham.h"
#include "system/bcr.h"

namespace graphics {

void fillTriangle(ScreenPos a, ScreenPos b, ScreenPos c, uint8_t color) {

    bool shouldWait = false;

    bcr.setupRowFill(color);
    fillTriangleGeneric<Bresenham>(a, b, c, [&](uint8_t y, uint8_t xLeft, uint8_t xRight) {
        if (shouldWait) {
            bcr.rowFillWait();
        } else {
            shouldWait = true;
        }

        bcr.triggerRowFill(xLeft, y, xRight - xLeft);
    });
    if (shouldWait) {
        bcr.rowFillWait();
    }
}

}