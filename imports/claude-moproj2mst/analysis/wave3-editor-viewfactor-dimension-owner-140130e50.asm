Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140130E50: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140130E55: 48 83 EC 38        sub         rsp,38h
  0000000140130E59: C7 44 24 28 03 00  mov         dword ptr [rsp+28h],3
                    00 00
  0000000140130E61: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140130E66: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140130E69: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140130E6E: FF 90 98 01 00 00  call        qword ptr [rax+0000000000000198h]
  0000000140130E74: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140130E79: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140130E7F: 74 0E              je          0000000140130E8F
  0000000140130E81: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140130E86: E8 55 F0 B4 00     call        0000000140C7FEE0
  0000000140130E8B: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  0000000140130E8F: 8B 44 24 28        mov         eax,dword ptr [rsp+28h]
  0000000140130E93: 48 83 C4 38        add         rsp,38h
  0000000140130E97: C3                 ret

  Summary

     1E02000 .text
