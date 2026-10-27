Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000141247ED0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000141247ED5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000141247EDA: 48 8B 4C 24 08     mov         rcx,qword ptr [rsp+8]
  0000000141247EDF: 48 8B 44 24 10     mov         rax,qword ptr [rsp+10h]
  0000000141247EE4: 0F B6 00           movzx       eax,byte ptr [rax]
  0000000141247EE7: 88 81 F9 00 00 00  mov         byte ptr [rcx+00000000000000F9h],al
  0000000141247EED: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  0000000141247EF2: 0F B6 80 F9 00 00  movzx       eax,byte ptr [rax+00000000000000F9h]
                    00
  0000000141247EF9: 85 C0              test        eax,eax
  0000000141247EFB: 75 1E              jne         0000000141247F1B
  0000000141247EFD: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  0000000141247F02: C7 80 FC 00 00 00  mov         dword ptr [rax+00000000000000FCh],0FFFFFFFFh
                    FF FF FF FF
  0000000141247F0C: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  0000000141247F11: C7 80 00 01 00 00  mov         dword ptr [rax+0000000000000100h],0FFFFFFFFh
                    FF FF FF FF
  0000000141247F1B: F3 C3              rep ret
  0000000141247F1D: CC                 int         3
  0000000141247F1E: CC                 int         3
  0000000141247F1F: CC                 int         3
  0000000141247F20: CC                 int         3
  0000000141247F21: CC                 int         3
  0000000141247F22: CC                 int         3
  0000000141247F23: CC                 int         3
  0000000141247F24: CC                 int         3
  0000000141247F25: CC                 int         3
  0000000141247F26: CC                 int         3
  0000000141247F27: CC                 int         3
  0000000141247F28: CC                 int         3
  0000000141247F29: CC                 int         3
  0000000141247F2A: CC                 int         3
  0000000141247F2B: CC                 int         3
  0000000141247F2C: CC                 int         3
  0000000141247F2D: CC                 int         3
  0000000141247F2E: CC                 int         3
  0000000141247F2F: CC                 int         3

  Summary

     1E02000 .text
