#include "system/scr.h"
#include "system/via.h"
#include "system/interrupts.h"

int main();

extern "C" {

    extern uint8_t __rc0, __rc1;
    extern uint8_t _bss_start, _bss_end;

    __attribute__((section(".text.fixed")))
    void __boot() {
        // set the banking register first since the "mirror" registers
        // are affected by the memory bank
        scr.bankingReg.write(0);

        // apparently this inits the stack?
        __rc0 = 0xFF;
        __rc1 = 0x1F;
        
        // the bss section is zero initialized memory, and is mapped to RAM.
        // but gametank RAM starts as ghostly random bits, so zero it.
        // take that, inscrutable unseen entities
        for (volatile uint8_t *bssPtr = &_bss_start; bssPtr < &_bss_end; bssPtr++) {
            *bssPtr = 0;
        }

        // why 254? apparently 255 (which is the same as 127, but they recommend setting the MSB)
        // is always in the fixed section, and this is the bank immediately preceding.
        via.changeRomBank(254);

        scr.audioRst.write(0);
        scr.audioNmi.write(0);
        scr.audioCfg.write(0);
        scr.videoCfg.write(0);

        main();
    }
}
