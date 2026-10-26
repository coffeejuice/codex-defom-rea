Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140C17070: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C17075: 48 83 EC 28        sub         rsp,28h
  0000000140C17079: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140C1707E: 48 8B 09           mov         rcx,qword ptr [rcx]
  0000000140C17081: FF 15 61 D2 1E 01  call        qword ptr [0000000141E042E8h]
  0000000140C17087: 0F B6 C0           movzx       eax,al
  0000000140C1708A: 85 C0              test        eax,eax
  0000000140C1708C: 75 12              jne         0000000140C170A0
  0000000140C1708E: 48 8B 54 24 30     mov         rdx,qword ptr [rsp+30h]
  0000000140C17093: 48 8B 12           mov         rdx,qword ptr [rdx]
  0000000140C17096: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140C1709B: E8 E0 70 FF FF     call        0000000140C0E180
  0000000140C170A0: 48 83 C4 28        add         rsp,28h
  0000000140C170A4: C3                 ret

  Summary

     1E02000 .text
