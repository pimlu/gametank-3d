.section .text
.global waitForInterrupt, vblankNmi, blitIsr

waitForInterrupt:
    .byte 0xCB
    RTS

vblankNmi:
    RTI
blitIsr:
    RTI