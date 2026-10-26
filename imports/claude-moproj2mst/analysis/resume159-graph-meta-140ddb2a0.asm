Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140DDB2A0: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  0000000140DDB2A5: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  0000000140DDB2AA: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140DDB2AE: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140DDB2B3: 48 83 EC 28        sub         rsp,28h
  0000000140DDB2B7: 4C 8B 4C 24 48     mov         r9,qword ptr [rsp+48h]
  0000000140DDB2BC: 44 8B 44 24 40     mov         r8d,dword ptr [rsp+40h]
  0000000140DDB2C1: 8B 54 24 38        mov         edx,dword ptr [rsp+38h]
  0000000140DDB2C5: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140DDB2CA: E8 D1 D8 F6 FF     call        0000000140D48BA0
  0000000140DDB2CF: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  0000000140DDB2D3: 83 7C 24 40 00     cmp         dword ptr [rsp+40h],0
  0000000140DDB2D8: 7D 06              jge         0000000140DDB2E0
  0000000140DDB2DA: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000140DDB2DE: EB 35              jmp         0000000140DDB315
  0000000140DDB2E0: 83 7C 24 38 00     cmp         dword ptr [rsp+38h],0
  0000000140DDB2E5: 75 2A              jne         0000000140DDB311
  0000000140DDB2E7: 83 7C 24 40 01     cmp         dword ptr [rsp+40h],1
  0000000140DDB2EC: 7D 18              jge         0000000140DDB306
  0000000140DDB2EE: 4C 8B 4C 24 48     mov         r9,qword ptr [rsp+48h]
  0000000140DDB2F3: 44 8B 44 24 40     mov         r8d,dword ptr [rsp+40h]
  0000000140DDB2F8: 8B 54 24 38        mov         edx,dword ptr [rsp+38h]
  0000000140DDB2FC: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140DDB301: E8 8A FE FF FF     call        0000000140DDB190
  0000000140DDB306: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000140DDB30A: 83 E8 01           sub         eax,1
  0000000140DDB30D: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  0000000140DDB311: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000140DDB315: 48 83 C4 28        add         rsp,28h
  0000000140DDB319: C3                 ret

  Summary

     1E02000 .text
