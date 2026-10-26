Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000141202AF0: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  0000000141202AF5: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  0000000141202AFA: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000141202AFE: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000141202B03: 48 83 EC 28        sub         rsp,28h
  0000000141202B07: 4C 8B 4C 24 48     mov         r9,qword ptr [rsp+48h]
  0000000141202B0C: 44 8B 44 24 40     mov         r8d,dword ptr [rsp+40h]
  0000000141202B11: 8B 54 24 38        mov         edx,dword ptr [rsp+38h]
  0000000141202B15: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000141202B1A: FF 15 58 1B C0 00  call        qword ptr [0000000141E04678h]
  0000000141202B20: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  0000000141202B24: 83 7C 24 40 00     cmp         dword ptr [rsp+40h],0
  0000000141202B29: 7D 06              jge         0000000141202B31
  0000000141202B2B: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000141202B2F: EB 35              jmp         0000000141202B66
  0000000141202B31: 83 7C 24 38 00     cmp         dword ptr [rsp+38h],0
  0000000141202B36: 75 2A              jne         0000000141202B62
  0000000141202B38: 83 7C 24 40 24     cmp         dword ptr [rsp+40h],24h
  0000000141202B3D: 7D 18              jge         0000000141202B57
  0000000141202B3F: 4C 8B 4C 24 48     mov         r9,qword ptr [rsp+48h]
  0000000141202B44: 44 8B 44 24 40     mov         r8d,dword ptr [rsp+40h]
  0000000141202B49: 8B 54 24 38        mov         edx,dword ptr [rsp+38h]
  0000000141202B4D: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000141202B52: E8 39 F9 FF FF     call        0000000141202490
  0000000141202B57: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000141202B5B: 83 E8 24           sub         eax,24h
  0000000141202B5E: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  0000000141202B62: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000141202B66: 48 83 C4 28        add         rsp,28h
  0000000141202B6A: C3                 ret

  Summary

     1E02000 .text
