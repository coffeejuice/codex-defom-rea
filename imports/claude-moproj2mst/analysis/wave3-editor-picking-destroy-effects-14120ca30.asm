Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014120CA30: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014120CA35: C3                 ret
  000000014120CA36: CC                 int         3
  000000014120CA37: CC                 int         3
  000000014120CA38: CC                 int         3
  000000014120CA39: CC                 int         3
  000000014120CA3A: CC                 int         3
  000000014120CA3B: CC                 int         3
  000000014120CA3C: CC                 int         3
  000000014120CA3D: CC                 int         3
  000000014120CA3E: CC                 int         3
  000000014120CA3F: CC                 int         3
  000000014120CA40: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014120CA45: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014120CA4A: 48 83 EC 28        sub         rsp,28h
  000000014120CA4E: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  000000014120CA54: 75 05              jne         000000014120CA5B
  000000014120CA56: E9 F8 02 00 00     jmp         000000014120CD53
  000000014120CA5B: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014120CA60: E8 BB FD FF FF     call        000000014120C820
  000000014120CA65: 4C 8B 5C 24 30     mov         r11,qword ptr [rsp+30h]
  000000014120CA6A: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  000000014120CA6F: 8B 40 08           mov         eax,dword ptr [rax+8]
  000000014120CA72: 41 89 43 08        mov         dword ptr [r11+8],eax
  000000014120CA76: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014120CA7B: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  000000014120CA80: F2 0F 10 40 10     movsd       xmm0,mmword ptr [rax+10h]
  000000014120CA85: F2 0F 11 41 10     movsd       mmword ptr [rcx+10h],xmm0
  000000014120CA8A: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014120CA8F: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  000000014120CA94: F2 0F 10 40 18     movsd       xmm0,mmword ptr [rax+18h]
  000000014120CA99: F2 0F 11 41 18     movsd       mmword ptr [rcx+18h],xmm0
  000000014120CA9E: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014120CAA3: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  000000014120CAA8: F2 0F 10 40 20     movsd       xmm0,mmword ptr [rax+20h]
  000000014120CAAD: F2
  000000014120CAAE: 0F
  000000014120CAAF: 11

  Summary

     1E02000 .text
