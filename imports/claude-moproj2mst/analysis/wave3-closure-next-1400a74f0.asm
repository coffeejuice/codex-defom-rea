Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400A74F0: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  00000001400A74F4: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400A74F9: 48 83 EC 38        sub         rsp,38h
  00000001400A74FD: 48 C7 44 24 28 00  mov         qword ptr [rsp+28h],0
                    00 00 00
  00000001400A7506: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001400A750B: E8 50 FE FF FF     call        00000001400A7360
  00000001400A7510: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  00000001400A7514: 83 7C 24 48 00     cmp         dword ptr [rsp+48h],0
  00000001400A7519: 7C 24              jl          00000001400A753F
  00000001400A751B: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  00000001400A751F: 39 44 24 48        cmp         dword ptr [rsp+48h],eax
  00000001400A7523: 7D 1A              jge         00000001400A753F
  00000001400A7525: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001400A752A: 48 83 C1 58        add         rcx,58h
  00000001400A752E: 8B 54 24 48        mov         edx,dword ptr [rsp+48h]
  00000001400A7532: E8 79 40 0A 00     call        000000014014B5B0
  00000001400A7537: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400A753A: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001400A753F: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  00000001400A7544: 48 83 C4 38        add         rsp,38h
  00000001400A7548: C3                 ret

  Summary

     1E02000 .text
