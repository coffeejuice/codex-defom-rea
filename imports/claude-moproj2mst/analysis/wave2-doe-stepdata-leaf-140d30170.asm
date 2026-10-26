Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D30170: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140D30174: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D30179: 48 83 EC 28        sub         rsp,28h
  0000000140D3017D: 83 7C 24 38 02     cmp         dword ptr [rsp+38h],2
  0000000140D30182: 75 1C              jne         0000000140D301A0
  0000000140D30184: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140D30189: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140D3018E: F2 0F 10 80 80 00  movsd       xmm0,mmword ptr [rax+0000000000000080h]
                    00 00
  0000000140D30196: F2 0F 11 81 58 04  movsd       mmword ptr [rcx+0000000000000458h],xmm0
                    00 00
  0000000140D3019E: EB 17              jmp         0000000140D301B7
  0000000140D301A0: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140D301A5: E8 26 00 00 00     call        0000000140D301D0
  0000000140D301AA: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140D301AF: F2 0F 11 80 58 04  movsd       mmword ptr [rax+0000000000000458h],xmm0
                    00 00
  0000000140D301B7: 48 83 C4 28        add         rsp,28h
  0000000140D301BB: C3                 ret
  0000000140D301BC: CC                 int         3

  Summary

     1E02000 .text
