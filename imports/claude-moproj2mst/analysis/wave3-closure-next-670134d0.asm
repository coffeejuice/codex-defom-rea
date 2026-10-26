Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670134D0: 8B 51 20           mov         edx,dword ptr [rcx+20h]
  00000000670134D3: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  00000000670134D7: 85 D2              test        edx,edx
  00000000670134D9: 74 14              je          00000000670134EF
  00000000670134DB: 0F 1F 44 00 00     nop         dword ptr [rax+rax]
  00000000670134E0: FF CA              dec         edx
  00000000670134E2: 48 39 08           cmp         qword ptr [rax],rcx
  00000000670134E5: 75 0C              jne         00000000670134F3
  00000000670134E7: 48 83 C0 08        add         rax,8
  00000000670134EB: 85 D2              test        edx,edx
  00000000670134ED: 75 F1              jne         00000000670134E0
  00000000670134EF: 48 8B C1           mov         rax,rcx
  00000000670134F2: C3                 ret
  00000000670134F3: 48 8B 00           mov         rax,qword ptr [rax]
  00000000670134F6: C3                 ret
  00000000670134F7: CC                 int         3
  00000000670134F8: CC                 int         3
  00000000670134F9: CC                 int         3
  00000000670134FA: CC                 int         3
  00000000670134FB: CC                 int         3
  00000000670134FC: CC                 int         3
  00000000670134FD: CC                 int         3
  00000000670134FE: CC                 int         3
  00000000670134FF: CC                 int         3
  0000000067013500: 40 53              push        rbx
  0000000067013502: 48 83 EC 20        sub         rsp,20h
  0000000067013506: 4D 8B C8           mov         r9,r8
  0000000067013509: 4C 8B C2           mov         r8,rdx
  000000006701350C: 48 8B D9           mov         rbx,rcx
  000000006701350F: 48

  Summary

      1A7000 .text
