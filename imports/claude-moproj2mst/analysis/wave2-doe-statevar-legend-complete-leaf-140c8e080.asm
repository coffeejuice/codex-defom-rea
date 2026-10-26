Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140C8E080: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C8E085: 48 83 EC 38        sub         rsp,38h
  0000000140C8E089: 66 0F 57 C0        xorpd       xmm0,xmm0
  0000000140C8E08D: F2 0F 11 44 24 20  movsd       mmword ptr [rsp+20h],xmm0
  0000000140C8E093: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140C8E098: E8 43 E0 FE FF     call        0000000140C7C0E0
  0000000140C8E09D: 0F B6 C0           movzx       eax,al
  0000000140C8E0A0: 85 C0              test        eax,eax
  0000000140C8E0A2: 74 19              je          0000000140C8E0BD
  0000000140C8E0A4: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140C8E0A9: 48 81 C1 08 01 00  add         rcx,108h
                    00
  0000000140C8E0B0: E8 7B B5 5A 00     call        0000000141239630
  0000000140C8E0B5: F2 0F 11 44 24 20  movsd       mmword ptr [rsp+20h],xmm0
  0000000140C8E0BB: EB 17              jmp         0000000140C8E0D4
  0000000140C8E0BD: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140C8E0C2: 48 81 C1 48 01 00  add         rcx,148h
                    00
  0000000140C8E0C9: E8 42 C2 58 00     call        000000014121A310
  0000000140C8E0CE: F2 0F 11 44 24 20  movsd       mmword ptr [rsp+20h],xmm0
  0000000140C8E0D4: F2 0F 10 44 24 20  movsd       xmm0,mmword ptr [rsp+20h]
  0000000140C8E0DA: 48 83 C4 38        add         rsp,38h
  0000000140C8E0DE: C3                 ret
  0000000140C8E0DF: CC                 int         3

  Summary

     1E02000 .text
