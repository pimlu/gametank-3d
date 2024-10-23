#include "system/scr.h"
#include "system/via.h"
#include "system/interrupts.h"

int main();

extern "C" {

    extern uint8_t __rc0, __rc1;

    __attribute__((section(".text.fixed")))
    void __boot() {
        // set the banking register first since the "mirror" registers
        // are affected by the memory bank
        scr.bankingReg.write(0);

        // apparently this inits the stack?
        __rc0 = 0xFF;
        __rc1 = 0x1F;

        // why 254?
        via.changeRomBank(254);

        scr.audioRst.write(0);
        scr.audioNmi.write(0);
        scr.audioCfg.write(0);
        scr.videoCfg.write(0);

        main();
    }
}
