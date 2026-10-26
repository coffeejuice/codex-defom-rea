Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D7AB20: 88 54 24 10        mov         byte ptr [rsp+10h],dl
  0000000140D7AB24: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D7AB29: 48 83 EC 28        sub         rsp,28h
  0000000140D7AB2D: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140D7AB32: E8 F9 6F FC FF     call        0000000140D41B30
  0000000140D7AB37: 83 F8 02           cmp         eax,2
  0000000140D7AB3A: 75 09              jne         0000000140D7AB45
  0000000140D7AB3C: 0F B6 44 24 38     movzx       eax,byte ptr [rsp+38h]
  0000000140D7AB41: 85 C0              test        eax,eax
  0000000140D7AB43: 74 0C              je          0000000140D7AB51
  0000000140D7AB45: 33 D2              xor         edx,edx
  0000000140D7AB47: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140D7AB4C: E8 9F FE FF FF     call        0000000140D7A9F0
  0000000140D7AB51: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140D7AB56: 48 83 C4 28        add         rsp,28h
  0000000140D7AB5A: C3                 ret

  Summary

     1E02000 .text
