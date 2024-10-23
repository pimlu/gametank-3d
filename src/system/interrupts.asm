.section .text
.global waitForInterrupt, vblankNmi, blitIsr, panicPls

waitForInterrupt:
    .byte 0xCB
    RTS

vblankNmi:
    RTI
blitIsr:
    RTI

panicPls:
    .byte 0x02
    JMP panicPls
