Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000141761F40: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000141761F45: 48 83 EC 28        sub         rsp,28h
  0000000141761F49: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000141761F4E: 48 83 38 00        cmp         qword ptr [rax],0
  0000000141761F52: 75 02              jne         0000000141761F56
  0000000141761F54: EB 2B              jmp         0000000141761F81
  0000000141761F56: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000141761F5B: 48 8B 09           mov         rcx,qword ptr [rcx]
  0000000141761F5E: 48 83 C1 68        add         rcx,68h
  0000000141761F62: FF 15 80 23 6A 00  call        qword ptr [0000000141E042E8h]
  0000000141761F68: 0F B6 C0           movzx       eax,al
  0000000141761F6B: 85 C0              test        eax,eax
  0000000141761F6D: 75 12              jne         0000000141761F81
  0000000141761F6F: 48 8B 54 24 30     mov         rdx,qword ptr [rsp+30h]
  0000000141761F74: 48 8B 12           mov         rdx,qword ptr [rdx]
  0000000141761F77: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000141761F7C: E8 8F 03 00 00     call        0000000141762310
  0000000141761F81: 48 83 C4 28        add         rsp,28h
  0000000141761F85: C3                 ret

  Summary

     1E02000 .text
