Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065506070: 83 3A 00           cmp         dword ptr [rdx],0
  0000000065506073: 7C 10              jl          0000000065506085
  0000000065506075: 83 7A 04 00        cmp         dword ptr [rdx+4],0
  0000000065506079: 7C 0A              jl          0000000065506085
  000000006550607B: 48 83 7A 10 00     cmp         qword ptr [rdx+10h],0
  0000000065506080: 74 03              je          0000000065506085
  0000000065506082: 33 C0              xor         eax,eax
  0000000065506084: C3                 ret
  0000000065506085: 48 8B 41 20        mov         rax,qword ptr [rcx+20h]
  0000000065506089: 8B 40 08           mov         eax,dword ptr [rax+8]
  000000006550608C: C3                 ret
  000000006550608D: CC                 int         3
  000000006550608E: CC                 int         3
  000000006550608F: CC                 int         3

  Summary

      695000 .text
