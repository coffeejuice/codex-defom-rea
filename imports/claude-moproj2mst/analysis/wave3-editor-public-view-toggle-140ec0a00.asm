Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140EC0A00: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140EC0A05: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140EC0A0A: 48 83 EC 38        sub         rsp,38h
  0000000140EC0A0E: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  0000000140EC0A13: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140EC0A18: E8 C3 58 FF FF     call        0000000140EB62E0
  0000000140EC0A1D: 83 7C 24 20 00     cmp         dword ptr [rsp+20h],0
  0000000140EC0A22: 7E 08              jle         0000000140EC0A2C
  0000000140EC0A24: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  0000000140EC0A29: 0F B6 00           movzx       eax,byte ptr [rax]
  0000000140EC0A2C: 48 83 C4 38        add         rsp,38h
  0000000140EC0A30: C3                 ret

  Summary

     1E02000 .text
