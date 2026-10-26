Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140918FC0: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140918FC4: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140918FC9: 48 83 EC 28        sub         rsp,28h
  0000000140918FCD: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140918FD2: E8 89 F0 FF FF     call        0000000140918060
  0000000140918FD7: 8B 44 24 38        mov         eax,dword ptr [rsp+38h]
  0000000140918FDB: 83 E0 01           and         eax,1
  0000000140918FDE: 85 C0              test        eax,eax
  0000000140918FE0: 74 0A              je          0000000140918FEC
  0000000140918FE2: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140918FE7: E8 7C 6C BC 00     call        00000001414DFC68
  0000000140918FEC: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140918FF1: 48 83 C4 28        add         rsp,28h
  0000000140918FF5: C3                 ret

  Summary

     1E02000 .text
