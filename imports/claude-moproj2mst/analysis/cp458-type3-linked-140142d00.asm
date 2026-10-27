Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140142D00: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140142D05: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  0000000140142D0A: 48 83 78 40 00     cmp         qword ptr [rax+40h],0
  0000000140142D0F: 74 3A              je          0000000140142D4B
  0000000140142D11: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  0000000140142D16: 48 8B 40 38        mov         rax,qword ptr [rax+38h]
  0000000140142D1A: 48 8B 4C 24 08     mov         rcx,qword ptr [rsp+8]
  0000000140142D1F: 48 8B 49 40        mov         rcx,qword ptr [rcx+40h]
  0000000140142D23: 8B 40 08           mov         eax,dword ptr [rax+8]
  0000000140142D26: 89 81 CC 00 00 00  mov         dword ptr [rcx+00000000000000CCh],eax
  0000000140142D2C: 48 8B 4C 24 08     mov         rcx,qword ptr [rsp+8]
  0000000140142D31: 48 8B 49 38        mov         rcx,qword ptr [rcx+38h]
  0000000140142D35: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  0000000140142D3A: 48 8B 40 40        mov         rax,qword ptr [rax+40h]
  0000000140142D3E: F2 0F 10 41 10     movsd       xmm0,mmword ptr [rcx+10h]
  0000000140142D43: F2 0F 11 80 E0 00  movsd       mmword ptr [rax+00000000000000E0h],xmm0
                    00 00
  0000000140142D4B: B0 01              mov         al,1
  0000000140142D4D: C3                 ret

  Summary

     1E02000 .text
