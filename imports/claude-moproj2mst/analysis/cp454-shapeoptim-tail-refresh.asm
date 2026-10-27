Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140142CA0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140142CA5: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  0000000140142CAA: 48 83 78 40 00     cmp         qword ptr [rax+40h],0
  0000000140142CAF: 74 3A              je          0000000140142CEB
  0000000140142CB1: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  0000000140142CB6: 48 8B 40 40        mov         rax,qword ptr [rax+40h]
  0000000140142CBA: 48 8B 4C 24 08     mov         rcx,qword ptr [rsp+8]
  0000000140142CBF: 48 8B 49 38        mov         rcx,qword ptr [rcx+38h]
  0000000140142CC3: 8B 80 CC 00 00 00  mov         eax,dword ptr [rax+00000000000000CCh]
  0000000140142CC9: 89 41 08           mov         dword ptr [rcx+8],eax
  0000000140142CCC: 48 8B 4C 24 08     mov         rcx,qword ptr [rsp+8]
  0000000140142CD1: 48 8B 49 40        mov         rcx,qword ptr [rcx+40h]
  0000000140142CD5: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  0000000140142CDA: 48 8B 40 38        mov         rax,qword ptr [rax+38h]
  0000000140142CDE: F2 0F 10 81 E0 00  movsd       xmm0,mmword ptr [rcx+00000000000000E0h]
                    00 00
  0000000140142CE6: F2 0F 11 40 10     movsd       mmword ptr [rax+10h],xmm0
  0000000140142CEB: B0 01              mov         al,1
  0000000140142CED: C3                 ret

  Summary

     1E02000 .text
