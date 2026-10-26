Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001411F1AB0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001411F1AB5: 48 83 EC 28        sub         rsp,28h
  00000001411F1AB9: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001411F1ABE: E8 2D FF FF FF     call        00000001411F19F0
  00000001411F1AC3: 48 85 C0           test        rax,rax
  00000001411F1AC6: 75 04              jne         00000001411F1ACC
  00000001411F1AC8: 33 C0              xor         eax,eax
  00000001411F1ACA: EB 0D              jmp         00000001411F1AD9
  00000001411F1ACC: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001411F1AD1: E8 1A FF FF FF     call        00000001411F19F0
  00000001411F1AD6: 8B 40 08           mov         eax,dword ptr [rax+8]
  00000001411F1AD9: 48 83 C4 28        add         rsp,28h
  00000001411F1ADD: C3                 ret

  Summary

     1E02000 .text
