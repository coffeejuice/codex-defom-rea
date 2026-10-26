Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140DD7A00: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  0000000140DD7A05: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  0000000140DD7A0A: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140DD7A0E: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140DD7A13: 48 83 EC 28        sub         rsp,28h
  0000000140DD7A17: 4C 8B 4C 24 48     mov         r9,qword ptr [rsp+48h]
  0000000140DD7A1C: 44 8B 44 24 40     mov         r8d,dword ptr [rsp+40h]
  0000000140DD7A21: 8B 54 24 38        mov         edx,dword ptr [rsp+38h]
  0000000140DD7A25: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140DD7A2A: FF 15 10 E1 02 01  call        qword ptr [0000000141E05B40h]
  0000000140DD7A30: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  0000000140DD7A34: 83 7C 24 40 00     cmp         dword ptr [rsp+40h],0
  0000000140DD7A39: 7D 06              jge         0000000140DD7A41
  0000000140DD7A3B: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000140DD7A3F: EB 35              jmp         0000000140DD7A76
  0000000140DD7A41: 83 7C 24 38 00     cmp         dword ptr [rsp+38h],0
  0000000140DD7A46: 75 2A              jne         0000000140DD7A72
  0000000140DD7A48: 83 7C 24 40 07     cmp         dword ptr [rsp+40h],7
  0000000140DD7A4D: 7D 18              jge         0000000140DD7A67
  0000000140DD7A4F: 4C 8B 4C 24 48     mov         r9,qword ptr [rsp+48h]
  0000000140DD7A54: 44 8B 44 24 40     mov         r8d,dword ptr [rsp+40h]
  0000000140DD7A59: 8B 54 24 38        mov         edx,dword ptr [rsp+38h]
  0000000140DD7A5D: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140DD7A62: E8 E9 FD FF FF     call        0000000140DD7850
  0000000140DD7A67: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000140DD7A6B: 83 E8 07           sub         eax,7
  0000000140DD7A6E: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  0000000140DD7A72: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000140DD7A76: 48 83 C4 28        add         rsp,28h
  0000000140DD7A7A: C3                 ret

  Summary

     1E02000 .text
