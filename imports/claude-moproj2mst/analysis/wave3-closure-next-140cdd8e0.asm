Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140CDD8E0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140CDD8E5: 48 83 EC 28        sub         rsp,28h
  0000000140CDD8E9: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140CDD8EE: 48 83 B8 70 19 00  cmp         qword ptr [rax+0000000000001970h],0
                    00 00
  0000000140CDD8F6: 75 1F              jne         0000000140CDD917
  0000000140CDD8F8: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140CDD8FD: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140CDD900: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140CDD905: FF 50 58           call        qword ptr [rax+58h]
  0000000140CDD908: 4C 8B D8           mov         r11,rax
  0000000140CDD90B: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140CDD910: 4C 89 98 70 19 00  mov         qword ptr [rax+0000000000001970h],r11
                    00
  0000000140CDD917: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140CDD91C: 48 8B 80 70 19 00  mov         rax,qword ptr [rax+0000000000001970h]
                    00
  0000000140CDD923: 48 83 C4 28        add         rsp,28h
  0000000140CDD927: C3                 ret

  Summary

     1E02000 .text
