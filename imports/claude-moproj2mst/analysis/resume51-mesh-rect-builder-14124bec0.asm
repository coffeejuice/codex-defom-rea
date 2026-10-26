Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014124BEC0: 44 89 4C 24 20     mov         dword ptr [rsp+20h],r9d
  000000014124BEC5: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  000000014124BECA: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  000000014124BECE: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014124BED3: 53                 push        rbx
  000000014124BED4: 48 83 EC 60        sub         rsp,60h
  000000014124BED8: 0F 29 74 24 50     movaps      xmmword ptr [rsp+50h],xmm6
  000000014124BEDD: C7 44 24 30 00 00  mov         dword ptr [rsp+30h],0
                    00 00
  000000014124BEE5: C7 44 24 48 00 00  mov         dword ptr [rsp+48h],0
                    00 00
  000000014124BEED: C7 44 24 44 00 00  mov         dword ptr [rsp+44h],0
                    00 00
  000000014124BEF5: 83 BC 24 80 00 00  cmp         dword ptr [rsp+0000000000000080h],0
                    00 00
  000000014124BEFD: 7E 0A              jle         000000014124BF09
  000000014124BEFF: 83 BC 24 88 00 00  cmp         dword ptr [rsp+0000000000000088h],0
                    00 00
  000000014124BF07: 7F 05              jg          000000014124BF0E
  000000014124BF09: E9 32 02 00 00     jmp         000000014124C140
  000000014124BF0E: 44 8B 8C 24 88 00  mov         r9d,dword ptr [rsp+0000000000000088h]
                    00 00
  000000014124BF16: 44 8B 84 24 80 00  mov         r8d,dword ptr [rsp+0000000000000080h]
                    00 00
  000000014124BF1E: 8B 54 24 78        mov         edx,dword ptr [rsp+78h]
  000000014124BF22: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  000000014124BF27: E8 B4 02 00 00     call        000000014124C1E0
  000000014124BF2C: C7 44 24 48 00 00  mov         dword ptr [rsp+48h],0
                    00 00
  000000014124BF34: EB 0B              jmp         000000014124BF41
  000000014124BF36: 8B 44 24 48        mov         eax,dword ptr [rsp+48h]
  000000014124BF3A: 83 C0 01           add         eax,1
  000000014124BF3D: 89 44 24 48        mov         dword ptr [rsp+48h],eax
  000000014124BF41: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  000000014124BF46: 8B 80 8C 01 00 00  mov         eax,dword ptr [rax+000000000000018Ch]
  000000014124BF4C: 39 44 24 48        cmp         dword ptr [rsp+48h],eax
  000000014124BF50: 7D 7C              jge         000000014124BFCE
  000000014124BF52: C7 44 24 44 00 00  mov         dword ptr [rsp+44h],0
                    00 00
  000000014124BF5A: EB 0B              jmp         000000014124BF67
  000000014124BF5C: 8B 44 24 44        mov         eax,dword ptr [rsp+44h]
  000000014124BF60: 83 C0 01           add         eax,1
  000000014124BF63: 89 44 24 44        mov         dword ptr [rsp+44h],eax
  000000014124BF67: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  000000014124BF6C: 8B 80 88 01 00 00  mov         eax,dword ptr [rax+0000000000000188h]
  000000014124BF72: 39 44 24 44        cmp         dword ptr [rsp+44h],eax
  000000014124BF76: 7D 51              jge         000000014124BFC9
  000000014124BF78: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  000000014124BF7D: 8B 44 24 48        mov         eax,dword ptr [rsp+48h]
  000000014124BF81: 0F AF 81 88 01 00  imul        eax,dword ptr [rcx+0000000000000188h]
                    00
  000000014124BF88: 03 44 24 44        add         eax,dword ptr [rsp+44h]
  000000014124BF8C: 48 63 C8           movsxd      rcx,eax
  000000014124BF8F: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  000000014124BF97: 8B 1C 88           mov         ebx,dword ptr [rax+rcx*4]
  000000014124BF9A: 83 EB 01           sub         ebx,1
  000000014124BF9D: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  000000014124BFA2: 8B 44 24 48        mov         eax,dword ptr [rsp+48h]
  000000014124BFA6: 0F AF 81 88 01 00  imul        eax,dword ptr [rcx+0000000000000188h]
                    00
  000000014124BFAD: 03 44 24 44        add         eax,dword ptr [rsp+44h]
  000000014124BFB1: 48 63 D0           movsxd      rdx,eax
  000000014124BFB4: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  000000014124BFB9: 48 81 C1 98 01 00  add         rcx,198h
                    00
  000000014124BFC0: E8 7B 76 E7 FE     call        00000001400C3640
  000000014124BFC5: 89 18              mov         dword ptr [rax],ebx
  000000014124BFC7: EB 93              jmp         000000014124BF5C
  000000014124BFC9: E9 68 FF FF FF     jmp         000000014124BF36
  000000014124BFCE: 0F 57 C0           xorps       xmm0,xmm0
  000000014124BFD1: F3 0F 11 44 24 20  movss       dword ptr [rsp+20h],xmm0
  000000014124BFD7: F3 0F 10 05 5D FC  movss       xmm0,dword ptr [00000001421EBC3Ch]
                    F9 00
  000000014124BFDF: F3 0F 11 44 24 24  movss       dword ptr [rsp+24h],xmm0
  000000014124BFE5: F3 0F 10 05 4F FC  movss       xmm0,dword ptr [00000001421EBC3Ch]
                    F9 00
  000000014124BFED: F3 0F 11 44 24 28  movss       dword ptr [rsp+28h],xmm0
  000000014124BFF3: 0F 57 C0           xorps       xmm0,xmm0
  000000014124BFF6: F3 0F 11 44 24 40  movss       dword ptr [rsp+40h],xmm0
  000000014124BFFC: 48 8D 44 24 40     lea         rax,[rsp+40h]
  000000014124C001: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  000000014124C006: F3 0F 10 44 24 20  movss       xmm0,dword ptr [rsp+20h]
  000000014124C00C: F3 0F 59 05 04 FB  mulss       xmm0,dword ptr [00000001421EBB18h]
                    F9 00
  000000014124C014: F3 48 0F 2C C0     cvttss2si   rax,xmm0
  000000014124C019: 0F B6 C8           movzx       ecx,al
  000000014124C01C: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  000000014124C021: 88 08              mov         byte ptr [rax],cl
  000000014124C023: F3 0F 10 44 24 24  movss       xmm0,dword ptr [rsp+24h]
  000000014124C029: F3 0F 59 05 E7 FA  mulss       xmm0,dword ptr [00000001421EBB18h]
                    F9 00
  000000014124C031: F3 48 0F 2C C0     cvttss2si   rax,xmm0
  000000014124C036: 0F B6 C8           movzx       ecx,al
  000000014124C039: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  000000014124C03E: 88 48 01           mov         byte ptr [rax+1],cl
  000000014124C041: F3 0F 10 44 24 28  movss       xmm0,dword ptr [rsp+28h]
  000000014124C047: F3 0F 59 05 C9 FA  mulss       xmm0,dword ptr [00000001421EBB18h]
                    F9 00
  000000014124C04F: F3 48 0F 2C C0     cvttss2si   rax,xmm0
  000000014124C054: 0F B6 C8           movzx       ecx,al
  000000014124C057: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  000000014124C05C: 88 48 02           mov         byte ptr [rax+2],cl
  000000014124C05F: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  000000014124C064: C6 40 03 FF        mov         byte ptr [rax+3],0FFh
  000000014124C068: C7 44 24 44 00 00  mov         dword ptr [rsp+44h],0
                    00 00
  000000014124C070: EB 0B              jmp         000000014124C07D
  000000014124C072: 8B 44 24 44        mov         eax,dword ptr [rsp+44h]
  000000014124C076: 83 C0 01           add         eax,1
  000000014124C079: 89 44 24 44        mov         dword ptr [rsp+44h],eax
  000000014124C07D: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  000000014124C082: 8B 80 90 01 00 00  mov         eax,dword ptr [rax+0000000000000190h]
  000000014124C088: 39 44 24 44        cmp         dword ptr [rsp+44h],eax
  000000014124C08C: 0F 8D 8A 00 00 00  jge         000000014124C11C
  000000014124C092: 8B 44 24 44        mov         eax,dword ptr [rsp+44h]
  000000014124C096: 6B C0 03           imul        eax,eax,3
  000000014124C099: 48 63 D0           movsxd      rdx,eax
  000000014124C09C: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  000000014124C0A1: 48 81 C1 A0 01 00  add         rcx,1A0h
                    00
  000000014124C0A8: E8 A3 4C 00 00     call        0000000141250D50
  000000014124C0AD: F3 0F 10 44 24 40  movss       xmm0,dword ptr [rsp+40h]
  000000014124C0B3: F3 0F 11 00        movss       dword ptr [rax],xmm0
  000000014124C0B7: C7 44 24 30 00 00  mov         dword ptr [rsp+30h],0
                    00 00
  000000014124C0BF: EB 0B              jmp         000000014124C0CC
  000000014124C0C1: 8B 44 24 30        mov         eax,dword ptr [rsp+30h]
  000000014124C0C5: 83 C0 01           add         eax,1
  000000014124C0C8: 89 44 24 30        mov         dword ptr [rsp+30h],eax
  000000014124C0CC: 83 7C 24 30 02     cmp         dword ptr [rsp+30h],2
  000000014124C0D1: 7D 44              jge         000000014124C117
  000000014124C0D3: 8B 4C 24 30        mov         ecx,dword ptr [rsp+30h]
  000000014124C0D7: 8B 44 24 44        mov         eax,dword ptr [rsp+44h]
  000000014124C0DB: 8D 04 41           lea         eax,[rcx+rax*2]
  000000014124C0DE: 48 63 C8           movsxd      rcx,eax
  000000014124C0E1: 48 8B 84 24 98 00  mov         rax,qword ptr [rsp+0000000000000098h]
                    00 00
  000000014124C0E9: F2 0F 5A 34 C8     cvtsd2ss    xmm6,mmword ptr [rax+rcx*8]
  000000014124C0EE: 8B 4C 24 44        mov         ecx,dword ptr [rsp+44h]
  000000014124C0F2: 6B C9 03           imul        ecx,ecx,3
  000000014124C0F5: 8B 44 24 30        mov         eax,dword ptr [rsp+30h]
  000000014124C0F9: 8D 44 01 01        lea         eax,[rcx+rax+1]
  000000014124C0FD: 48 63 D0           movsxd      rdx,eax
  000000014124C100: 48

  Summary

     1E02000 .text
