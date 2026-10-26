Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065506090: 83 3A 00           cmp         dword ptr [rdx],0
  0000000065506093: 7C 10              jl          00000000655060A5
  0000000065506095: 83 7A 04 00        cmp         dword ptr [rdx+4],0
  0000000065506099: 7C 0A              jl          00000000655060A5
  000000006550609B: 48 83 7A 10 00     cmp         qword ptr [rdx+10h],0
  00000000655060A0: 74 03              je          00000000655060A5
  00000000655060A2: 33 C0              xor         eax,eax
  00000000655060A4: C3                 ret
  00000000655060A5: 48 8B 41 28        mov         rax,qword ptr [rcx+28h]
  00000000655060A9: 8B 40 08           mov         eax,dword ptr [rax+8]
  00000000655060AC: C3                 ret
  00000000655060AD: CC                 int         3
  00000000655060AE: CC                 int         3
  00000000655060AF: CC                 int         3

  Summary

      695000 .text
