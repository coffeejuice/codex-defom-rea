Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000650AA340: 48 8B 51 10        mov         rdx,qword ptr [rcx+10h]
  00000000650AA344: 48 85 D2           test        rdx,rdx
  00000000650AA347: 74 39              je          00000000650AA382
  00000000650AA349: 8B 42 30           mov         eax,dword ptr [rdx+30h]
  00000000650AA34C: 83 F8 06           cmp         eax,6
  00000000650AA34F: 74 2E              je          00000000650AA37F
  00000000650AA351: 83 F8 05           cmp         eax,5
  00000000650AA354: 74 29              je          00000000650AA37F
  00000000650AA356: 83 F8 08           cmp         eax,8
  00000000650AA359: 74 24              je          00000000650AA37F
  00000000650AA35B: 83 F8 0C           cmp         eax,0Ch
  00000000650AA35E: 74 1F              je          00000000650AA37F
  00000000650AA360: 83 F8 0A           cmp         eax,0Ah
  00000000650AA363: 74 1A              je          00000000650AA37F
  00000000650AA365: 83 F8 0F           cmp         eax,0Fh
  00000000650AA368: 74 15              je          00000000650AA37F
  00000000650AA36A: F6 42 58 04        test        byte ptr [rdx+58h],4
  00000000650AA36E: 74 12              je          00000000650AA382
  00000000650AA370: 83 F8 03           cmp         eax,3
  00000000650AA373: 74 0A              je          00000000650AA37F
  00000000650AA375: 83 F8 01           cmp         eax,1
  00000000650AA378: 74 05              je          00000000650AA37F
  00000000650AA37A: 83 F8 02           cmp         eax,2
  00000000650AA37D: 75 03              jne         00000000650AA382
  00000000650AA37F: B0 01              mov         al,1
  00000000650AA381: C3                 ret
  00000000650AA382: 32 C0              xor         al,al
  00000000650AA384: C3                 ret
  00000000650AA385: CC                 int         3
  00000000650AA386: CC                 int         3
  00000000650AA387: CC                 int         3
  00000000650AA388: CC                 int         3
  00000000650AA389: CC                 int         3
  00000000650AA38A: CC                 int         3
  00000000650AA38B: CC                 int         3
  00000000650AA38C: CC                 int         3
  00000000650AA38D: CC                 int         3
  00000000650AA38E: CC                 int         3
  00000000650AA38F: CC                 int         3
  00000000650AA390: 48 83 EC 28        sub         rsp,28h
  00000000650AA394: 48 8B 41 10        mov         rax,qword ptr [rcx+10h]
  00000000650AA398: 48 85 C0           test        rax,rax
  00000000650AA39B: 75 05              jne         00000000650AA3A2
  00000000650AA39D: 48
  00000000650AA39E: 83
  00000000650AA39F: C4

  Summary

      695000 .text
