Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001411F1B80: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001411F1B85: 48 83 EC 28        sub         rsp,28h
  00000001411F1B89: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001411F1B8E: E8 5D FE FF FF     call        00000001411F19F0
  00000001411F1B93: 48 85 C0           test        rax,rax
  00000001411F1B96: 75 04              jne         00000001411F1B9C
  00000001411F1B98: 32 C0              xor         al,al
  00000001411F1B9A: EB 12              jmp         00000001411F1BAE
  00000001411F1B9C: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001411F1BA1: E8 4A FE FF FF     call        00000001411F19F0
  00000001411F1BA6: 48 8B C8           mov         rcx,rax
  00000001411F1BA9: E8 F2 33 64 FF     call        0000000140834FA0
  00000001411F1BAE: 48 83 C4 28        add         rsp,28h
  00000001411F1BB2: C3                 ret

  Summary

     1E02000 .text
