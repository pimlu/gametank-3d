.section .vector_table,"a",@progbits


.global _VECTOR_TABLE
_VECTOR_TABLE:
    .word vblankNmi
    .word _start
    .word blitIsr
