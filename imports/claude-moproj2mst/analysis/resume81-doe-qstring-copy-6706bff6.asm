Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006706BFF6: 66 0F 6E C2        movd        xmm0,edx
  000000006706BFFA: 8B C7              mov         eax,edi
  000000006706BFFC: C1 F8 04           sar         eax,4
  000000006706BFFF: 66 0F 70 D0 00     pshufd      xmm2,xmm0,0
  000000006706C004: 85 C0              test        eax,eax
  000000006706C006: 7E 30              jle         000000006706C038
  000000006706C008: 8B D0              mov         edx,eax
  000000006706C00A: 66 0F 1F 44 00 00  nop         word ptr [rax+rax]
  000000006706C010: F3 0F 6F 0B        movdqu      xmm1,xmmword ptr [rbx]
  000000006706C014: 48 83 C1 20        add         rcx,20h
  000000006706C018: 48 83 C3 10        add         rbx,10h
  000000006706C01C: 48 83 EA 01        sub         rdx,1
  000000006706C020: 66 0F 6F C1        movdqa      xmm0,xmm1
  000000006706C024: 66 0F 68 CA        punpckhbw   xmm1,xmm2
  000000006706C028: 66 0F 60 C2        punpcklbw   xmm0,xmm2
  000000006706C02C: F3 0F 7F 41 E0     movdqu      xmmword ptr [rcx-20h],xmm0
  000000006706C031: F3 0F 7F 49 F0     movdqu      xmmword ptr [rcx-10h],xmm1
  000000006706C036: 75 D8              jne         000000006706C010
  000000006706C038: 8B C7              mov         eax,edi
  000000006706C03A: 99                 cdq
  000000006706C03B: 83 E2 0F           and         edx,0Fh
  000000006706C03E: 8D 3C 02           lea         edi,[rdx+rax]
  000000006706C041: 83 E7 0F           and         edi,0Fh
  000000006706C044: 2B FA              sub         edi,edx
  000000006706C046: 85 FF              test        edi,edi
  000000006706C048: 74 19              je          000000006706C063
  000000006706C04A: 66 0F 1F 44 00 00  nop         word ptr [rax+rax]
  000000006706C050: 0F B6 03           movzx       eax,byte ptr [rbx]
  000000006706C053: 48 83 C1 02        add         rcx,2
  000000006706C057: 48 FF C3           inc         rbx
  000000006706C05A: 83 EF 01           sub         edi,1
  000000006706C05D: 66 89 41 FE        mov         word ptr [rcx-2],ax
  000000006706C061: 75 ED              jne         000000006706C050
  000000006706C063: 48 8B C6           mov         rax,rsi
  000000006706C066: 48 8B 74 24 20     mov         rsi,qword ptr [rsp+20h]
  000000006706C06B: 48 83 C4 28        add         rsp,28h
  000000006706C06F: 5F                 pop         rdi
  000000006706C070: 5B                 pop         rbx
  000000006706C071: C3                 ret

  Summary

      1A7000 .text
