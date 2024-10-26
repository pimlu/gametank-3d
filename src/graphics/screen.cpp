#include "screen.h"

#include "system/bcr.h"
#include "system/interrupts.h"

namespace graphics {

void clearBorder(uint8_t c) {
    bcr.drawBox(0, 0, SCREEN_WIDTH-PADDING_RIGHT, PADDING_TOP, c);
    waitForInterrupt(); 
    bcr.drawBox(0, PADDING_TOP, PADDING_RIGHT, SCREEN_HEIGHT-PADDING_TOP, c);
    waitForInterrupt(); 
    bcr.drawBox(PADDING_LEFT, SCREEN_HEIGHT-PADDING_BOT, SCREEN_WIDTH-PADDING_LEFT, PADDING_BOT, c);
    waitForInterrupt(); 
    bcr.drawBox(SCREEN_WIDTH-PADDING_RIGHT, 0, PADDING_RIGHT, SCREEN_HEIGHT-PADDING_BOT, c);
    waitForInterrupt();
}

}