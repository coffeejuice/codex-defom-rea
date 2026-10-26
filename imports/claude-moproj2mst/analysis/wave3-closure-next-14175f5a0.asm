Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014175F5A0: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  000000014175F5A5: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  000000014175F5AA: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  000000014175F5AE: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014175F5B3: 48 83 EC 28        sub         rsp,28h
  000000014175F5B7: 4C 8B 4C 24 48     mov         r9,qword ptr [rsp+48h]
  000000014175F5BC: 44 8B 44 24 40     mov         r8d,dword ptr [rsp+40h]
  000000014175F5C1: 8B 54 24 38        mov         edx,dword ptr [rsp+38h]
  000000014175F5C5: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014175F5CA: FF 15 A8 50 6A 00  call        qword ptr [0000000141E04678h]
  000000014175F5D0: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  000000014175F5D4: 83 7C 24 40 00     cmp         dword ptr [rsp+40h],0
  000000014175F5D9: 7D 06              jge         000000014175F5E1
  000000014175F5DB: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  000000014175F5DF: EB 35              jmp         000000014175F616
  000000014175F5E1: 83 7C 24 38 00     cmp         dword ptr [rsp+38h],0
  000000014175F5E6: 75 2A              jne         000000014175F612
  000000014175F5E8: 83 7C 24 40 03     cmp         dword ptr [rsp+40h],3
  000000014175F5ED: 7D 18              jge         000000014175F607
  000000014175F5EF: 4C 8B 4C 24 48     mov         r9,qword ptr [rsp+48h]
  000000014175F5F4: 44 8B 44 24 40     mov         r8d,dword ptr [rsp+40h]
  000000014175F5F9: 8B 54 24 38        mov         edx,dword ptr [rsp+38h]
  000000014175F5FD: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014175F602: E8 49 FE FF FF     call        000000014175F450
  000000014175F607: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  000000014175F60B: 83 E8 03           sub         eax,3
  000000014175F60E: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  000000014175F612: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  000000014175F616: 48 83 C4 28        add         rsp,28h
  000000014175F61A: C3                 ret

  Summary

     1E02000 .text
