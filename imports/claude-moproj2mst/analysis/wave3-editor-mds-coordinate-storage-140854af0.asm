Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140854AF0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140854AF5: 48 83 EC 38        sub         rsp,38h
  0000000140854AF9: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140854AFE: E8 3D E7 FF FF     call        0000000140853240
  0000000140854B03: 45 33 C9           xor         r9d,r9d
  0000000140854B06: 4C 8B C0           mov         r8,rax
  0000000140854B09: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140854B0E: 8B 90 2C 01 00 00  mov         edx,dword ptr [rax+000000000000012Ch]
  0000000140854B14: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140854B19: E8 E2 F6 FF FF     call        0000000140854200
  0000000140854B1E: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140854B23: 48 83 B8 30 01 00  cmp         qword ptr [rax+0000000000000130h],0
                    00 00
  0000000140854B2B: 74 1E              je          0000000140854B4B
  0000000140854B2D: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140854B35: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  0000000140854B3A: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140854B3F: 48 8B 89 30 01 00  mov         rcx,qword ptr [rcx+0000000000000130h]
                    00
  0000000140854B46: E8 55 91 FD FF     call        000000014082DCA0
  0000000140854B4B: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140854B50: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  0000000140854B54: 48 83 C4 38        add         rsp,38h
  0000000140854B58: C3                 ret

  Summary

     1E02000 .text
