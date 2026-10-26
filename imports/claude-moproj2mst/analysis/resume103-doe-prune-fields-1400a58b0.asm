Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400A58B0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400A58B5: 48 83 EC 28        sub         rsp,28h
  00000001400A58B9: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001400A58BE: E8 CD FF FF FF     call        00000001400A5890
  00000001400A58C3: 48 85 C0           test        rax,rax
  00000001400A58C6: 75 04              jne         00000001400A58CC
  00000001400A58C8: 33 C0              xor         eax,eax
  00000001400A58CA: EB 12              jmp         00000001400A58DE
  00000001400A58CC: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001400A58D1: E8 BA FF FF FF     call        00000001400A5890
  00000001400A58D6: 48 8B C8           mov         rcx,rax
  00000001400A58D9: E8 A2 F3 F9 FF     call        0000000140044C80
  00000001400A58DE: 48 83 C4 28        add         rsp,28h
  00000001400A58E2: C3                 ret

  Summary

     1E02000 .text
