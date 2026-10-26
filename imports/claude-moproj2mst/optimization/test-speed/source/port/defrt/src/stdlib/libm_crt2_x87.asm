; MSVC CRT pow (140483f60): the x87 core that computes base**y = 2**(y*log2(base)).
; MSVC x64 has no inline asm, and FYL2X / F2XM1 / FSCALE are not expressible in SSE2 C++, so the
; exact instruction sequence of the original is kept here. Like the original (and like the host
; x87 unit), the result depends on this CPU's FYL2X/F2XM1 implementation.
;
; The original sets the x87 precision-control field to 53-bit (double) and round-to-nearest for
; the duration of the computation and restores the caller's control word afterwards; this is kept
; exactly (FNSTCW / AND 0xf0ff / OR 0x200 / FLDCW ... / FLDCW original).
;
; extern "C" double def_rt_libm_crt_pow_x87(double base, double y);
;   base in xmm0, y in xmm1, result in xmm0.  @src 140483f60 (x87 block 140483f7f..140484450)

_TEXT SEGMENT

def_rt_libm_crt_pow_x87 PROC
        sub     rsp, 24
        fnstcw  word ptr [rsp+16]              ; @140483f7f  save caller control word
        movsd   qword ptr [rsp], xmm0          ; base
        movsd   qword ptr [rsp+8], xmm1        ; y
        movzx   eax, word ptr [rsp+16]         ; @140483f91
        and     eax, 0f0ffh                    ; @140483fba  clear PC (and RC) field
        or      eax, 0200h                     ; @140483fc0  PC = 53-bit double, RC = nearest
        mov     word ptr [rsp+18], ax          ; @140483fc6
        fldcw   word ptr [rsp+18]              ; @140483fd8  install
        fld     qword ptr [rsp+8]              ; @140484424  st0 = y
        fld     qword ptr [rsp]                ; @140484428  st0 = base, st1 = y
        fyl2x                                  ; @14048442c  st0 = y*log2(base)
        fld     st(0)                          ; @14048442e
        frndint                                ; @140484430  st0 = round(w)
        fxch    st(1)                          ; @140484432  st0 = w, st1 = round
        fsub    st(0), st(1)                   ; @140484434  st0 = w - round (fraction)
        f2xm1                                  ; @140484436  st0 = 2**frac - 1
        fld1                                   ; @140484438
        faddp   st(1), st(0)                   ; @14048443a  st0 = 2**frac
        fscale                                 ; @14048443c  st0 = 2**frac * 2**round
        fstp    st(1)                          ; @14048443e
        fstp    qword ptr [rsp]                ; @140484440  store result
        fldcw   word ptr [rsp+16]              ; @14048452a  restore caller control word
        movsd   xmm0, qword ptr [rsp]
        add     rsp, 24
        ret
def_rt_libm_crt_pow_x87 ENDP

_TEXT ENDS
END
