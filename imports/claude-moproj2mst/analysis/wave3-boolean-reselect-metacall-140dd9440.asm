Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140DD9440: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  0000000140DD9445: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  0000000140DD944A: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140DD944E: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140DD9453: 48 83 EC 28        sub         rsp,28h
  0000000140DD9457: 4C 8B 4C 24 48     mov         r9,qword ptr [rsp+48h]
  0000000140DD945C: 44 8B 44 24 40     mov         r8d,dword ptr [rsp+40h]
  0000000140DD9461: 8B 54 24 38        mov         edx,dword ptr [rsp+38h]
  0000000140DD9465: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140DD946A: FF 15 58 26 03 01  call        qword ptr [0000000141E0BAC8h]
  0000000140DD9470: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  0000000140DD9474: 83 7C 24 40 00     cmp         dword ptr [rsp+40h],0
  0000000140DD9479: 7D 06              jge         0000000140DD9481
  0000000140DD947B: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000140DD947F: EB 35              jmp         0000000140DD94B6
  0000000140DD9481: 83 7C 24 38 00     cmp         dword ptr [rsp+38h],0
  0000000140DD9486: 75 2A              jne         0000000140DD94B2
  0000000140DD9488: 83 7C 24 40 10     cmp         dword ptr [rsp+40h],10h
  0000000140DD948D: 7D 18              jge         0000000140DD94A7
  0000000140DD948F: 4C 8B 4C 24 48     mov         r9,qword ptr [rsp+48h]
  0000000140DD9494: 44 8B 44 24 40     mov         r8d,dword ptr [rsp+40h]
  0000000140DD9499: 8B 54 24 38        mov         edx,dword ptr [rsp+38h]
  0000000140DD949D: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140DD94A2: E8 09 FD FF FF     call        0000000140DD91B0
  0000000140DD94A7: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000140DD94AB: 83 E8 10           sub         eax,10h
  0000000140DD94AE: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  0000000140DD94B2: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000140DD94B6: 48 83 C4 28        add         rsp,28h
  0000000140DD94BA: C3                 ret

  Summary

     1E02000 .text
