Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067129B60: 48 63 15 11 A8 1C  movsxd      rdx,dword ptr [00000000672F4378h]
                    00
  0000000067129B67: 4C 8D 05 FA A7 1C  lea         r8,[00000000672F4368h]
                    00
  0000000067129B6E: 8D 42 01           lea         eax,[rdx+1]
  0000000067129B71: 49 89 0C D0        mov         qword ptr [r8+rdx*8],rcx
  0000000067129B75: 99                 cdq
  0000000067129B76: 83 E0 01           and         eax,1
  0000000067129B79: 33 C2              xor         eax,edx
  0000000067129B7B: 2B C2              sub         eax,edx
  0000000067129B7D: 89 05 F5 A7 1C 00  mov         dword ptr [00000000672F4378h],eax
  0000000067129B83: 48 8B C1           mov         rax,rcx
  0000000067129B86: C3                 ret
  0000000067129B87: CC                 int         3
  0000000067129B88: CC                 int         3
  0000000067129B89: CC                 int         3
  0000000067129B8A: CC                 int         3
  0000000067129B8B: CC                 int         3
  0000000067129B8C: CC                 int         3
  0000000067129B8D: CC                 int         3
  0000000067129B8E: CC                 int         3
  0000000067129B8F: CC                 int         3
  0000000067129B90: 48 89 5C 24 10     mov         qword ptr [rsp+10h],rbx
  0000000067129B95: 48 89 6C 24 18     mov         qword ptr [rsp+18h],rbp
  0000000067129B9A: 48 89 74 24 20     mov         qword ptr [rsp+20h],rsi
  0000000067129B9F: 41 54              push        r12
  0000000067129BA1: 48 83 EC 40        sub         rsp,40h
  0000000067129BA5: 48 8B DA           mov         rbx,rdx
  0000000067129BA8: 0F BE 12           movsx       edx,byte ptr [rdx]
  0000000067129BAB: 4D 8B E0           mov         r12,r8
  0000000067129BAE: 83
  0000000067129BAF: E2

  Summary

      1A7000 .text
