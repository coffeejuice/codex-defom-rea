Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014003F680: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014003F685: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014003F68A: 48 83 EC 28        sub         rsp,28h
  000000014003F68E: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014003F693: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  000000014003F698: 48 8B 00           mov         rax,qword ptr [rax]
  000000014003F69B: 48 89 01           mov         qword ptr [rcx],rax
  000000014003F69E: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014003F6A3: 48 8B 09           mov         rcx,qword ptr [rcx]
  000000014003F6A6: FF 15 4C 4C DC 01  call        qword ptr [0000000141E042F8h]
  000000014003F6AC: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014003F6B1: 48 8B 00           mov         rax,qword ptr [rax]
  000000014003F6B4: 8B 40 10           mov         eax,dword ptr [rax+10h]
  000000014003F6B7: 83 E0 01           and         eax,1
  000000014003F6BA: 85 C0              test        eax,eax
  000000014003F6BC: 75 0A              jne         000000014003F6C8
  000000014003F6BE: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014003F6C3: E8 58 5C FE FF     call        0000000140025320
  000000014003F6C8: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014003F6CD: 48 83 C4 28        add         rsp,28h
  000000014003F6D1: C3                 ret

  Summary

     1E02000 .text
