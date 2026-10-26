Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

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
  000000006706C072: CC                 int         3
  000000006706C073: CC                 int         3
  000000006706C074: CC                 int         3
  000000006706C075: CC                 int         3
  000000006706C076: CC                 int         3
  000000006706C077: CC                 int         3
  000000006706C078: CC                 int         3
  000000006706C079: CC                 int         3
  000000006706C07A: CC                 int         3
  000000006706C07B: CC                 int         3
  000000006706C07C: CC                 int         3
  000000006706C07D: CC                 int         3
  000000006706C07E: CC                 int         3
  000000006706C07F: CC                 int         3
  000000006706C080: 48 83 EC 28        sub         rsp,28h
  000000006706C084: 4C 63 DA           movsxd      r11,edx
  000000006706C087: 4C 8B D1           mov         r10,rcx
  000000006706C08A: 4A 8D 14 59        lea         rdx,[rcx+r11*2]
  000000006706C08E: 49 8B C3           mov         rax,r11
  000000006706C091: 4D 85 C0           test        r8,r8
  000000006706C094: 74 4C              je          000000006706C0E2
  000000006706C096: 41 83 F9 01        cmp         r9d,1
  000000006706C09A: 75 41              jne         000000006706C0DD
  000000006706C09C: 48 3B CA           cmp         rcx,rdx
  000000006706C09F: 73 1D              jae         000000006706C0BE
  000000006706C0A1: 41 0F B6 00        movzx       eax,byte ptr [r8]
  000000006706C0A5: 84 C0              test        al,al
  000000006706C0A7: 74 15              je          000000006706C0BE
  000000006706C0A9: 0F B6 C0           movzx       eax,al
  000000006706C0AC: 66 41 39 02        cmp         word ptr [r10],ax
  000000006706C0B0: 75 0C              jne         000000006706C0BE
  000000006706C0B2: 49 83 C2 02        add         r10,2
  000000006706C0B6: 49 FF C0           inc         r8
  000000006706C0B9: 4C 3B D2           cmp         r10,rdx
  000000006706C0BC: 72 E3              jb          000000006706C0A1
  000000006706C0BE: 4C 3B D2           cmp         r10,rdx
  000000006706C0C1: 75 0B              jne         000000006706C0CE
  000000006706C0C3: 41
  000000006706C0C4: 0F
  000000006706C0C5: B6

  Summary

      1A7000 .text
