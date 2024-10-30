#include "horizon.h"

#include "geometry/fixed_etc.h"
#include "system/i8helpers.h"
#include "graphics/screen.h"
#include "system/bcr.h"
#include "system/interrupts.h"

using namespace geometry;
using namespace graphics;

void drawHorizon(geometry::GeoF pos_) {
    int8_t pos = -roundGeoF(pos_);

    int8_t oceanLo = SCENE_Y_TO_FRAME + SCENE_Y_LO;
    int8_t oceanHi = SCENE_Y_TO_FRAME + mini8(SCENE_Y_HI, pos);

    int8_t sandLo = SCENE_Y_TO_FRAME + maxi8(SCENE_Y_LO, pos);
    int8_t sandHi = SCENE_Y_TO_FRAME + SCENE_Y_HI;

    
    if (oceanLo < oceanHi) {
        bcr.drawBox(
            FRAME_X_LO, oceanLo,
            FRAME_W, oceanHi - oceanLo,
            ~0b110'11'011);
        waitForInterrupt();
    }
    if (sandLo < sandHi) {
        bcr.drawBox(
            FRAME_X_LO, sandLo,
            FRAME_W, sandHi - sandLo,
            ~0b010'01'100);
        waitForInterrupt();
    }

}