; https://www.lysator.liu.se/~nisse/misc/6502-mul.html
; In: Factors in A and X
; Out: High byte in A, low byte in X


.global __multiplyu8Tou16, __imulConsts, __imulBuf

sqtab_lsb = __imulConsts
sqtab_msb = __imulConsts + 256

tmp_a = __imulBuf
tmp_b = __imulBuf + 1
tmp_c = __imulBuf + 2

__multiplyu8Tou16:
  sta tmp_a
  cpx tmp_a
  bcc sorted
  txa
  ldx tmp_a
sorted:
  sta tmp_c
  stx tmp_a
  sec
  sbc tmp_a
  tay
  ldx tmp_c
  lda sqtab_lsb,x
  sbc sqtab_lsb,y
  sta tmp_b
  lda sqtab_msb,x
  sbc sqtab_msb,y
  sta tmp_c
  clc
  ldx tmp_a
  lda tmp_b
  adc sqtab_lsb,x
  sta tmp_b
  lda tmp_c
  adc sqtab_msb,x
  ror
  ror tmp_b
  ldx tmp_b
  rts