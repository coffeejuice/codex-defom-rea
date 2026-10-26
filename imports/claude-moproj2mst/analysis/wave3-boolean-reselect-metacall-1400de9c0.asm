Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400DE9C0: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  00000001400DE9C5: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  00000001400DE9CA: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  00000001400DE9CE: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400DE9D3: 48 83 EC 28        sub         rsp,28h
  00000001400DE9D7: 4C 8B 4C 24 48     mov         r9,qword ptr [rsp+48h]
  00000001400DE9DC: 44 8B 44 24 40     mov         r8d,dword ptr [rsp+40h]
  00000001400DE9E1: 8B 54 24 38        mov         edx,dword ptr [rsp+38h]
  00000001400DE9E5: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001400DE9EA: FF 15 D8 D0 D2 01  call        qword ptr [0000000141E0BAC8h]
  00000001400DE9F0: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  00000001400DE9F4: 83 7C 24 40 00     cmp         dword ptr [rsp+40h],0
  00000001400DE9F9: 7D 06              jge         00000001400DEA01
  00000001400DE9FB: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  00000001400DE9FF: EB 35              jmp         00000001400DEA36
  00000001400DEA01: 83 7C 24 38 00     cmp         dword ptr [rsp+38h],0
  00000001400DEA06: 75 2A              jne         00000001400DEA32
  00000001400DEA08: 83 7C 24 40 1D     cmp         dword ptr [rsp+40h],1Dh
  00000001400DEA0D: 7D 18              jge         00000001400DEA27
  00000001400DEA0F: 4C 8B 4C 24 48     mov         r9,qword ptr [rsp+48h]
  00000001400DEA14: 44 8B 44 24 40     mov         r8d,dword ptr [rsp+40h]
  00000001400DEA19: 8B 54 24 38        mov         edx,dword ptr [rsp+38h]
  00000001400DEA1D: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001400DEA22: E8 C9 FB FF FF     call        00000001400DE5F0
  00000001400DEA27: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  00000001400DEA2B: 83 E8 1D           sub         eax,1Dh
  00000001400DEA2E: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  00000001400DEA32: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  00000001400DEA36: 48 83 C4 28        add         rsp,28h
  00000001400DEA3A: C3                 ret

  Summary

     1E02000 .text
