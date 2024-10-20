#include "system/scr.h"
#include "system/via.h"

#include "main.h"

// #[no_mangle]
// #[link_section = ".text.fixed"]
// extern "C" fn __boot() {
//     unsafe {
//         let bank_reg: *mut u8 = 0x2005 as *mut u8;
//         ptr::write_volatile(bank_reg, 0);

//         __rc0 = 0xFF;
//         __rc1 = 0x1F;

//         let via: &'static mut Via = Via::new();
//         via.change_rom_bank(254);

//         // init();
//         main();
//     }
//     core::panic!("Came out of main");
// }


extern "C" {

    // @stu memreg?
    extern uint8_t __rc0, __rc1;

    extern void *vblankNmi, *blitIsr;

    __attribute__((section("text.fixed")))
    void __boot() {
        scr->bankingReg = 0;

        // apparently this inits the stack?
        __rc0 = 0xFF;
        __rc1 = 0x1F;

        // @stu why 254?
        via->changeRomBank(254);

        scr->audioRst = 0;
        scr->audioNmi = 0;
        // scr->bankingReg = 0;
        scr->audioCfg = 0;
        scr->videoCfg = 0;

        main();

        while(true);
    }

    __attribute__((section("vector_table")))
    static void *_VECTOR_TABLE[3] = {
        vblankNmi,
        __boot,
        blitIsr
    };
}
