Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140DD7D40: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  0000000140DD7D45: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  0000000140DD7D4A: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140DD7D4E: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140DD7D53: 48 83 EC 28        sub         rsp,28h
  0000000140DD7D57: 4C 8B 4C 24 48     mov         r9,qword ptr [rsp+48h]
  0000000140DD7D5C: 44 8B 44 24 40     mov         r8d,dword ptr [rsp+40h]
  0000000140DD7D61: 8B 54 24 38        mov         edx,dword ptr [rsp+38h]
  0000000140DD7D65: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140DD7D6A: FF 15 78 36 03 01  call        qword ptr [0000000141E0B3E8h]
  0000000140DD7D70: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  0000000140DD7D74: 83 7C 24 40 00     cmp         dword ptr [rsp+40h],0
  0000000140DD7D79: 7D 06              jge         0000000140DD7D81
  0000000140DD7D7B: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000140DD7D7F: EB 35              jmp         0000000140DD7DB6
  0000000140DD7D81: 83 7C 24 38 00     cmp         dword ptr [rsp+38h],0
  0000000140DD7D86: 75 2A              jne         0000000140DD7DB2
  0000000140DD7D88: 83 7C 24 40 0F     cmp         dword ptr [rsp+40h],0Fh
  0000000140DD7D8D: 7D 18              jge         0000000140DD7DA7
  0000000140DD7D8F: 4C 8B 4C 24 48     mov         r9,qword ptr [rsp+48h]
  0000000140DD7D94: 44 8B 44 24 40     mov         r8d,dword ptr [rsp+40h]
  0000000140DD7D99: 8B 54 24 38        mov         edx,dword ptr [rsp+38h]
  0000000140DD7D9D: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140DD7DA2: E8 D9 FC FF FF     call        0000000140DD7A80
  0000000140DD7DA7: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000140DD7DAB: 83 E8 0F           sub         eax,0Fh
  0000000140DD7DAE: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  0000000140DD7DB2: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000140DD7DB6: 48 83 C4 28        add         rsp,28h
  0000000140DD7DBA: C3                 ret

  Summary

     1E02000 .text
