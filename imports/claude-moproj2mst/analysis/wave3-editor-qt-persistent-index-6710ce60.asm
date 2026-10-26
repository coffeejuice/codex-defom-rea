Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006710CE60: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006710CE63: 48 85 C0           test        rax,rax
  000000006710CE66: 74 03              je          000000006710CE6B
  000000006710CE68: 8B 00              mov         eax,dword ptr [rax]
  000000006710CE6A: C3                 ret
  000000006710CE6B: 83 C8 FF           or          eax,0FFFFFFFFh
  000000006710CE6E: C3                 ret
  000000006710CE6F: CC                 int         3
  000000006710CE70: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006710CE73: 48 85 C0           test        rax,rax
  000000006710CE76: 74 04              je          000000006710CE7C
  000000006710CE78: 8B 40 04           mov         eax,dword ptr [rax+4]
  000000006710CE7B: C3                 ret
  000000006710CE7C: 83 C8 FF           or          eax,0FFFFFFFFh
  000000006710CE7F: C3                 ret
  000000006710CE80: 40 53              push        rbx
  000000006710CE82: 48 83 EC 20        sub         rsp,20h
  000000006710CE86: 48 8B 09           mov         rcx,qword ptr [rcx]
  000000006710CE89: 33 C0              xor         eax,eax
  000000006710CE8B: 48 8B DA           mov         rbx,rdx
  000000006710CE8E: 89 44 24 30        mov         dword ptr [rsp+30h],eax
  000000006710CE92: 48 85 C9           test        rcx,rcx
  000000006710CE95: 74 0E              je          000000006710CEA5
  000000006710CE97: E8 64 FA FF FF     call        000000006710C900
  000000006710CE9C: 48 8B C3           mov         rax,rbx
  000000006710CE9F: 48

  Summary

      1A7000 .text
