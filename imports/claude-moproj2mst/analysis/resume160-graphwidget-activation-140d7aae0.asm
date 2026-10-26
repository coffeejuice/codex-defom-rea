Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D7AAE0: 88 54 24 10        mov         byte ptr [rsp+10h],dl
  0000000140D7AAE4: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D7AAE9: 48 83 EC 28        sub         rsp,28h
  0000000140D7AAED: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140D7AAF2: E8 39 70 FC FF     call        0000000140D41B30
  0000000140D7AAF7: 83 F8 01           cmp         eax,1
  0000000140D7AAFA: 75 09              jne         0000000140D7AB05
  0000000140D7AAFC: 0F B6 44 24 38     movzx       eax,byte ptr [rsp+38h]
  0000000140D7AB01: 85 C0              test        eax,eax
  0000000140D7AB03: 74 0A              je          0000000140D7AB0F
  0000000140D7AB05: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140D7AB0A: E8 81 D9 FF FF     call        0000000140D78490
  0000000140D7AB0F: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140D7AB14: 48 83 C4 28        add         rsp,28h
  0000000140D7AB18: C3                 ret

  Summary

     1E02000 .text
