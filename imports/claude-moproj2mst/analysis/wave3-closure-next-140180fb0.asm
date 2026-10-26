Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140180FB0: F2 0F 11 4C 24 10  movsd       mmword ptr [rsp+10h],xmm1
  0000000140180FB6: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140180FBB: 48 81 EC B8 00 00  sub         rsp,0B8h
                    00
  0000000140180FC2: 48 8D 54 24 58     lea         rdx,[rsp+58h]
  0000000140180FC7: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140180FCF: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  0000000140180FD3: FF 15 B7 66 C8 01  call        qword ptr [0000000141E07690h]
  0000000140180FD9: 48 8B C8           mov         rcx,rax
  0000000140180FDC: FF 15 A6 37 C8 01  call        qword ptr [0000000141E04788h]
  0000000140180FE2: F2 0F 11 44 24 40  movsd       mmword ptr [rsp+40h],xmm0
  0000000140180FE8: 48 8D 54 24 78     lea         rdx,[rsp+78h]
  0000000140180FED: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140180FF5: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  0000000140180FF9: FF 15 91 66 C8 01  call        qword ptr [0000000141E07690h]
  0000000140180FFF: 48 8B C8           mov         rcx,rax
  0000000140181002: FF 15 78 37 C8 01  call        qword ptr [0000000141E04780h]
  0000000140181008: F2 0F 11 44 24 20  movsd       mmword ptr [rsp+20h],xmm0
  000000014018100E: F2 0F 10 44 24 40  movsd       xmm0,mmword ptr [rsp+40h]
  0000000140181014: 66 0F 2F 44 24 20  comisd      xmm0,mmword ptr [rsp+20h]
  000000014018101A: 76 11              jbe         000000014018102D
  000000014018101C: F2 0F 10 44 24 40  movsd       xmm0,mmword ptr [rsp+40h]
  0000000140181022: F2 0F 11 84 24 98  movsd       mmword ptr [rsp+0000000000000098h],xmm0
                    00 00 00
  000000014018102B: EB 0F              jmp         000000014018103C
  000000014018102D: F2 0F 10 44 24 20  movsd       xmm0,mmword ptr [rsp+20h]
  0000000140181033: F2 0F 11 84 24 98  movsd       mmword ptr [rsp+0000000000000098h],xmm0
                    00 00 00
  000000014018103C: F2 0F 10 84 24 98  movsd       xmm0,mmword ptr [rsp+0000000000000098h]
                    00 00 00
  0000000140181045: F2 0F 11 44 24 30  movsd       mmword ptr [rsp+30h],xmm0
  000000014018104B: F2 0F 10 44 24 40  movsd       xmm0,mmword ptr [rsp+40h]
  0000000140181051: 66 0F 2F 44 24 20  comisd      xmm0,mmword ptr [rsp+20h]
  0000000140181057: 76 2D              jbe         0000000140181086
  0000000140181059: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140181061: 48 8B 09           mov         rcx,qword ptr [rcx]
  0000000140181064: FF 15 FE A4 C8 01  call        qword ptr [0000000141E0B568h]
  000000014018106A: 48 8B C8           mov         rcx,rax
  000000014018106D: FF 15 9D 5D C8 01  call        qword ptr [0000000141E06E10h]
  0000000140181073: 66 0F 6E C0        movd        xmm0,eax
  0000000140181077: F3 0F E6 C0        cvtdq2pd    xmm0,xmm0
  000000014018107B: F2 0F 11 84 24 A0  movsd       mmword ptr [rsp+00000000000000A0h],xmm0
                    00 00 00
  0000000140181084: EB 2B              jmp         00000001401810B1
  0000000140181086: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  000000014018108E: 48 8B 09           mov         rcx,qword ptr [rcx]
  0000000140181091: FF 15 D1 A4 C8 01  call        qword ptr [0000000141E0B568h]
  0000000140181097: 48 8B C8           mov         rcx,rax
  000000014018109A: FF 15 38 6D C8 01  call        qword ptr [0000000141E07DD8h]
  00000001401810A0: 66 0F 6E C0        movd        xmm0,eax
  00000001401810A4: F3 0F E6 C0        cvtdq2pd    xmm0,xmm0
  00000001401810A8: F2 0F 11 84 24 A0  movsd       mmword ptr [rsp+00000000000000A0h],xmm0
                    00 00 00
  00000001401810B1: F2 0F 10 84 24 A0  movsd       xmm0,mmword ptr [rsp+00000000000000A0h]
                    00 00 00
  00000001401810BA: F2 0F 11 44 24 48  movsd       mmword ptr [rsp+48h],xmm0
  00000001401810C0: F2 0F 10 44 24 48  movsd       xmm0,mmword ptr [rsp+48h]
  00000001401810C6: F2 0F 59 05 4A 2C  mulsd       xmm0,mmword ptr [00000001421D3D18h]
                    05 02
  00000001401810CE: F2 0F 5E 44 24 30  divsd       xmm0,mmword ptr [rsp+30h]
  00000001401810D4: F2 0F 11 44 24 38  movsd       mmword ptr [rsp+38h],xmm0
  00000001401810DA: F2 0F 10 05 56 84  movsd       xmm0,mmword ptr [00000001421D9538h]
                    05 02
  00000001401810E2: F2 0F 11 44 24 50  movsd       mmword ptr [rsp+50h],xmm0
  00000001401810E8: F2 0F 10 05 F0 38  movsd       xmm0,mmword ptr [00000001421249E0h]
                    FA 01
  00000001401810F0: F2 0F 11 44 24 28  movsd       mmword ptr [rsp+28h],xmm0
  00000001401810F6: F2 0F 10 05 E2 38  movsd       xmm0,mmword ptr [00000001421249E0h]
                    FA 01
  00000001401810FE: 66 0F 2F 84 24 C8  comisd      xmm0,mmword ptr [rsp+00000000000000C8h]
                    00 00 00
  0000000140181107: 76 33              jbe         000000014018113C
  0000000140181109: F2 0F 10 84 24 C8  movsd       xmm0,mmword ptr [rsp+00000000000000C8h]
                    00 00 00
  0000000140181112: F2 0F 5C 05 C6 38  subsd       xmm0,mmword ptr [00000001421249E0h]
                    FA 01
  000000014018111A: F2 0F 59 05 56 0F  mulsd       xmm0,mmword ptr [0000000142142078h]
                    FC 01
  0000000140181122: F2 0F 10 0D B6 38  movsd       xmm1,mmword ptr [00000001421249E0h]
                    FA 01
  000000014018112A: F2 0F 5C 4C 24 38  subsd       xmm1,mmword ptr [rsp+38h]
  0000000140181130: F2 0F 5E C1        divsd       xmm0,xmm1
  0000000140181134: F2 0F 11 44 24 28  movsd       mmword ptr [rsp+28h],xmm0
  000000014018113A: EB 1D              jmp         0000000140181159
  000000014018113C: F2 0F 10 84 24 C8  movsd       xmm0,mmword ptr [rsp+00000000000000C8h]
                    00 00 00
  0000000140181145: F2 0F 5C 05 93 38  subsd       xmm0,mmword ptr [00000001421249E0h]
                    FA 01
  000000014018114D: F2 0F 5E 44 24 50  divsd       xmm0,mmword ptr [rsp+50h]
  0000000140181153: F2 0F 11 44 24 28  movsd       mmword ptr [rsp+28h],xmm0
  0000000140181159: F2 0F 2C 44 24 28  cvttsd2si   eax,mmword ptr [rsp+28h]
  000000014018115F: 48 81 C4 B8 00 00  add         rsp,0B8h
                    00
  0000000140181166: C3                 ret

  Summary

     1E02000 .text
