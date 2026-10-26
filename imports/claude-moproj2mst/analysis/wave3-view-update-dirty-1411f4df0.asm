Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001411F4DF0: 88 54 24 10        mov         byte ptr [rsp+10h],dl
  00000001411F4DF4: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001411F4DF9: 48 83 EC 38        sub         rsp,38h
  00000001411F4DFD: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001411F4E02: E8 F9 00 00 00     call        00000001411F4F00
  00000001411F4E07: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001411F4E0C: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  00000001411F4E12: 74 3B              je          00000001411F4E4F
  00000001411F4E14: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001411F4E19: E8 F2 CB FF FF     call        00000001411F1A10
  00000001411F4E1E: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001411F4E23: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  00000001411F4E29: 75 02              jne         00000001411F4E2D
  00000001411F4E2B: EB 22              jmp         00000001411F4E4F
  00000001411F4E2D: 48 8B 54 24 20     mov         rdx,qword ptr [rsp+20h]
  00000001411F4E32: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  00000001411F4E37: E8 F4 98 FF FF     call        00000001411EE730
  00000001411F4E3C: 0F B6 44 24 48     movzx       eax,byte ptr [rsp+48h]
  00000001411F4E41: 85 C0              test        eax,eax
  00000001411F4E43: 74 0A              je          00000001411F4E4F
  00000001411F4E45: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001411F4E4A: E8 31 00 00 00     call        00000001411F4E80
  00000001411F4E4F: 48 83 C4 38        add         rsp,38h
  00000001411F4E53: C3                 ret

  Summary

     1E02000 .text
