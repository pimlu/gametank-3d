.section .text.fixed
.global waitForInterrupt, vblankNmi, blitIsr, panicPls

waitForInterrupt:
    .byte 0xCB
    RTS

vblankNmi:
    RTI
blitIsr:
    STZ 0x4006 ; DMA_Start
    RTI

panicPls:
    .byte 0x02
    JMP panicPls
