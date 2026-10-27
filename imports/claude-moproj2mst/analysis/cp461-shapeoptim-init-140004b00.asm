Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140004B00: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140004B05: 48 83 EC 28        sub         rsp,28h
  0000000140004B09: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140004B0E: 48 8B 09           mov         rcx,qword ptr [rcx]
  0000000140004B11: FF 15 D1 F7 DF 01  call        qword ptr [0000000141E042E8h]
  0000000140004B17: 0F B6 C0           movzx       eax,al
  0000000140004B1A: 85 C0              test        eax,eax
  0000000140004B1C: 75 12              jne         0000000140004B30
  0000000140004B1E: 48 8B 54 24 30     mov         rdx,qword ptr [rsp+30h]
  0000000140004B23: 48 8B 12           mov         rdx,qword ptr [rdx]
  0000000140004B26: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140004B2B: E8 60 03 00 00     call        0000000140004E90
  0000000140004B30: 48 83 C4 28        add         rsp,28h
  0000000140004B34: C3                 ret

  Summary

     1E02000 .text
