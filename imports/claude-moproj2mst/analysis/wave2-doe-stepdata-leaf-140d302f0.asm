Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D302F0: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  0000000140D302F5: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140D302FA: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D302FF: 48 83 EC 28        sub         rsp,28h
  0000000140D30303: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140D30308: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000140D3030C: 39 81 78 04 00 00  cmp         dword ptr [rcx+0000000000000478h],eax
  0000000140D30312: 74 0A              je          0000000140D3031E
  0000000140D30314: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140D30319: E8 A2 FF FF FF     call        0000000140D302C0
  0000000140D3031E: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  0000000140D30324: 74 20              je          0000000140D30346
  0000000140D30326: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140D3032B: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140D30330: 48 89 81 70 04 00  mov         qword ptr [rcx+0000000000000470h],rax
                    00
  0000000140D30337: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140D3033C: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000140D30340: 89 81 78 04 00 00  mov         dword ptr [rcx+0000000000000478h],eax
  0000000140D30346: 48 83 C4 28        add         rsp,28h
  0000000140D3034A: C3                 ret
  0000000140D3034B: CC                 int         3

  Summary

     1E02000 .text
