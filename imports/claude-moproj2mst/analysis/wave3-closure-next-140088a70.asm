Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140088A70: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140088A75: 48 83 EC 28        sub         rsp,28h
  0000000140088A79: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140088A7E: 48 83 38 00        cmp         qword ptr [rax],0
  0000000140088A82: 75 02              jne         0000000140088A86
  0000000140088A84: EB 2B              jmp         0000000140088AB1
  0000000140088A86: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140088A8B: 48 8B 09           mov         rcx,qword ptr [rcx]
  0000000140088A8E: 48 83 C1 68        add         rcx,68h
  0000000140088A92: FF 15 50 B8 D7 01  call        qword ptr [0000000141E042E8h]
  0000000140088A98: 0F B6 C0           movzx       eax,al
  0000000140088A9B: 85 C0              test        eax,eax
  0000000140088A9D: 75 12              jne         0000000140088AB1
  0000000140088A9F: 48 8B 54 24 30     mov         rdx,qword ptr [rsp+30h]
  0000000140088AA4: 48 8B 12           mov         rdx,qword ptr [rdx]
  0000000140088AA7: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140088AAC: E8 FF ED FD FF     call        00000001400678B0
  0000000140088AB1: 48 83 C4 28        add         rsp,28h
  0000000140088AB5: C3                 ret

  Summary

     1E02000 .text
