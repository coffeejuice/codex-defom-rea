Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001409E49D0: 44 89 4C 24 20     mov         dword ptr [rsp+20h],r9d
  00000001409E49D5: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  00000001409E49DA: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  00000001409E49DE: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001409E49E3: 48 83 EC 28        sub         rsp,28h
  00000001409E49E7: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001409E49EC: E8 EF F7 FF FF     call        00000001409E41E0
  00000001409E49F1: 39 44 24 48        cmp         dword ptr [rsp+48h],eax
  00000001409E49F5: 74 07              je          00000001409E49FE
  00000001409E49F7: 83 7C 24 48 FF     cmp         dword ptr [rsp+48h],0FFFFFFFFh
  00000001409E49FC: 75 39              jne         00000001409E4A37
  00000001409E49FE: 8B 54 24 38        mov         edx,dword ptr [rsp+38h]
  00000001409E4A02: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001409E4A07: E8 64 F8 FF FF     call        00000001409E4270
  00000001409E4A0C: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001409E4A11: E8 CA F7 FF FF     call        00000001409E41E0
  00000001409E4A16: 48 63 C8           movsxd      rcx,eax
  00000001409E4A19: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001409E4A1E: 8B 14 88           mov         edx,dword ptr [rax+rcx*4]
  00000001409E4A21: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001409E4A26: E8 A5 F8 FF FF     call        00000001409E42D0
  00000001409E4A2B: 33 D2              xor         edx,edx
  00000001409E4A2D: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001409E4A32: E8 89 FC FF FF     call        00000001409E46C0
  00000001409E4A37: 48 83 C4 28        add         rsp,28h
  00000001409E4A3B: C3                 ret

  Summary

     1E02000 .text
