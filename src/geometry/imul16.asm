; smult12.a
; from smult3, but tweaked to use mult86 as a base
;
; 16 bit x 16 bit signed multiplication, 32 bit result
; Average cycles: 234.57
; 2210 bytes (including init routine)


.global __imulBuf

; pointers to tables of squares
p_sqr_lo1    = __imulBuf      ; 2 bytes
p_sqr_hi1    = __imulBuf+2   ; 2 bytes
p_neg_sqr_lo = __imulBuf+4   ; 2 bytes
p_neg_sqr_hi = __imulBuf+8   ; 2 bytes
p_sqr_lo2    = __imulBuf+10   ; 2 bytes
p_sqr_hi2    = __imulBuf+12   ; 2 bytes



; the inputs and outputs
x0  = p_sqr_lo1      ; multiplier, 2 bytes
x1  = p_sqr_lo2      ; [WARNING: note non-consecutive bytes!]
y0  = __imulBuf+14            ; multiplicand, 2 bytes
y1  = __imulBuf+15
z0  = __imulBuf+16            ; product, 4 bytes
z1  = __imulBuf+17            ;
z2  = __imulBuf+18            ;
z3  = __imulBuf+19            ;


.section .rodata

.global __imulConsts
; Align tables to start of page
; Note - the last byte of each table is never referenced, as a+b<=510

sqrlo = __imulConsts + 512*0
sqrhi = __imulConsts + 512*1
negsqrlo = __imulConsts + 512*2
negsqrhi = __imulConsts + 512*3

; sqrlo:
;     !for i, 0, 511 {
;         !byte <((i*i)/4)
;     }
; sqrhi:
;     !for i, 0, 511 {
;         !byte >((i*i)/4)
;     }
; 
; negsqrlo:
;     !for i, 0, 511 {
;         !byte <(((255-i)*(255-i))/4)
;     }
; 
; negsqrhi:
;     !for i, 0, 511 {
;         !byte >(((255-i)*(255-i))/4)
;     }

.section .rodata
.global multiply_16bit_signed, init_multiplication

; Description: Signed 16-bit multiplication with signed 32-bit result.
;
; Input: 16-bit signed value in x0,x1
;        16-bit signed value in y0,y1
;
; Output: 32-bit signed value in z0,z1,z2,z3
;
; Clobbered: X, A, C
multiply_16bit_signed:
    ; Step 1: unsigned multiply
    jsr multiply_16bit_unsigned

    ; Step 2: Apply sign (See C=Hacking16 for details).
    bit x1
    bpl 123f
    sec
;    lda z2
    sbc y0
    sta z2
    lda z3
    sbc y1
    sta z3
123:
    bit y1
    bpl 123f
    sec
    lda z2
    sbc x0
    sta z2
    lda z3
    sbc x1
    sta z3
123:
    rts

; mult86.a
; from 6502.org, by Repose: http://forum.6502.org/viewtopic.php?p=106519#p106519
;
; 16 bit x 16 bit unsigned multiply, 32 bit result
; Average cycles: 193.07 (including z1 and z2 results being saved to memory locations)

; How to use:
; call jsr init, before first use
; put numbers in (x0,x1) and (y0,y1) and result is (z3, z2 (also A), z1 (also Y), z0)

; Diagram of the additions
;                 y1    y0
;              x  x1    x0
;                 --------
;              x0y0h x0y0l
; +      x0y1h x0y1l
; +      x1y0h x1y0l
; +x1y1h x1y1l
; ------------------------
;     z3    z2    z1    z0

multiply_16bit_unsigned:
    ; set multiplier as x1
    lda x1
    sta p_sqr_hi1
    eor #0xff
    sta p_neg_sqr_lo
    sta p_neg_sqr_hi

    ; set multiplicand as y0
    ldy y0

    ; x1y0l =  low(x1*y0)
    ; x1y0h = high(x1*y0)
    sec
    lda (p_sqr_lo2),y
    sbc (p_neg_sqr_lo),y
    sta x1y0l+1
    lda (p_sqr_hi1), y
    sbc (p_neg_sqr_hi),y
    sta x1y0h+1

    ; set multiplicand as y1
    ldy y1

    ; x1y1l =  low(x1*y1)
    ; z3    = high(x1*y1)
    lda (p_sqr_lo2),y
    sbc (p_neg_sqr_lo),y
    sta x1y1l+1
    lda (p_sqr_hi1),y
    sbc (p_neg_sqr_hi),y
    sta z3

    ; set multiplier as x0
    lda x0
    sta p_sqr_hi2
    eor #0xff
    sta p_neg_sqr_lo
    sta p_neg_sqr_hi

    ; x0y1l =  low(x0*y1)
    ; X     = high(x0*y1)
    lda (p_sqr_lo1),y
    sbc (p_neg_sqr_lo),y
    sta x0y1l+1
    lda (p_sqr_hi2),y
    sbc (p_neg_sqr_hi),y
    tax

    ; set multiplicand as y0
    ldy y0

    ; z0    =  low(x0*y0)
    ; A     = high(x0*y0)
    lda (p_sqr_lo1),y
    sbc (p_neg_sqr_lo),y
    sta z0
    lda (p_sqr_hi2),y
    sbc (p_neg_sqr_hi),y

    clc
do_adds:
    ; add the first two numbers of column 1
x0y1l:
    adc #0      ; x0y0h + x0y1l
    tay

    ; continue to first two numbers of column 2
    txa
x1y0h:
    adc #0      ; x0y1h + x1y0h
    tax         ; X=z2 so far
    bcc 123f
    inc z3      ; column 3
    clc

    ; add last number of column 1
123:
    tya
x1y0l:
    adc #0      ; + x1y0l
    tay         ; Y=z1

    ; add last number of column 2
    txa
x1y1l:
    adc #0      ; + x1y1l
    bcc fin     ; A=z2
    inc z3      ; column 3
fin:
    sta z2      ; Added: save middle byte results from registers A,Y
    sty z1      ;
    rts

; Once only initialisation
; (TODO: this could set up the pointer values in a loop to save memory
; it could also generate the square tables in code rather than load them)
init_multiplication:
    lda #>sqrlo
    sta p_sqr_lo2+1
    sta p_sqr_lo1+1

    lda #>sqrhi
    sta p_sqr_hi1+1
    sta p_sqr_hi2+1

    lda #>negsqrlo
    sta p_neg_sqr_lo+1

    lda #>negsqrhi
    sta p_neg_sqr_hi+1
    rts
