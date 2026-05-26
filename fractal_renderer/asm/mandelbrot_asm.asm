section .data
    four dq 4.0
    two  dq 2.0

section .text
    global mandelbrot_iter_asm

; int mandelbrot_iter_asm(double real, double imag, int max_iter)
; Windows x64 ABI:
; real     -> xmm0
; imag     -> xmm1
; max_iter -> r8d
; return   -> eax

mandelbrot_iter_asm:
    xorpd xmm2, xmm2        ; zr = 0.0
    xorpd xmm3, xmm3        ; zi = 0.0
    xor eax, eax            ; iter = 0

.loop:
    cmp eax, r8d
    jge .done

    movapd xmm4, xmm2
    mulsd xmm4, xmm2        ; xmm4 = zr * zr

    movapd xmm5, xmm3
    mulsd xmm5, xmm3        ; xmm5 = zi * zi

    addsd xmm5, xmm4        ; xmm5 = zr*zr + zi*zi
    ucomisd xmm5, [rel four]
    ja .done

    ; recalcular zr*zr y zi*zi para temp
    movapd xmm4, xmm2
    mulsd xmm4, xmm2        ; xmm4 = zr*zr

    movapd xmm5, xmm3
    mulsd xmm5, xmm3        ; xmm5 = zi*zi

    subsd xmm4, xmm5        ; xmm4 = zr*zr - zi*zi
    addsd xmm4, xmm0        ; temp = zr*zr - zi*zi + real

    mulsd xmm3, xmm2        ; zi = zi * zr
    mulsd xmm3, [rel two]   ; zi = 2 * zr * zi
    addsd xmm3, xmm1        ; zi = 2*zr*zi + imag

    movapd xmm2, xmm4       ; zr = temp

    inc eax
    jmp .loop

.done:
    ret