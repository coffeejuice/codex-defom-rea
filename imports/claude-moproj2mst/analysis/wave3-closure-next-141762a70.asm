Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000141762A70: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  0000000141762A75: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  0000000141762A7A: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000141762A7E: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000141762A83: 48 83 EC 28        sub         rsp,28h
  0000000141762A87: 4C 8B 4C 24 48     mov         r9,qword ptr [rsp+48h]
  0000000141762A8C: 44 8B 44 24 40     mov         r8d,dword ptr [rsp+40h]
  0000000141762A91: 8B 54 24 38        mov         edx,dword ptr [rsp+38h]
  0000000141762A95: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000141762A9A: FF 15 C0 8F 6A 00  call        qword ptr [0000000141E0BA60h]
  0000000141762AA0: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  0000000141762AA4: 83 7C 24 40 00     cmp         dword ptr [rsp+40h],0
  0000000141762AA9: 7D 06              jge         0000000141762AB1
  0000000141762AAB: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000141762AAF: EB 35              jmp         0000000141762AE6
  0000000141762AB1: 83 7C 24 38 00     cmp         dword ptr [rsp+38h],0
  0000000141762AB6: 75 2A              jne         0000000141762AE2
  0000000141762AB8: 83 7C 24 40 13     cmp         dword ptr [rsp+40h],13h
  0000000141762ABD: 7D 18              jge         0000000141762AD7
  0000000141762ABF: 4C 8B 4C 24 48     mov         r9,qword ptr [rsp+48h]
  0000000141762AC4: 44 8B 44 24 40     mov         r8d,dword ptr [rsp+40h]
  0000000141762AC9: 8B 54 24 38        mov         edx,dword ptr [rsp+38h]
  0000000141762ACD: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000141762AD2: E8 C9 FC FF FF     call        00000001417627A0
  0000000141762AD7: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000141762ADB: 83 E8 13           sub         eax,13h
  0000000141762ADE: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  0000000141762AE2: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000141762AE6: 48 83 C4 28        add         rsp,28h
  0000000141762AEA: C3                 ret

  Summary

     1E02000 .text
