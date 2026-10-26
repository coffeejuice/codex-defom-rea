Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D7AB70: 88 54 24 10        mov         byte ptr [rsp+10h],dl
  0000000140D7AB74: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D7AB79: 48 83 EC 38        sub         rsp,38h
  0000000140D7AB7D: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140D7AB82: E8 A9 6F FC FF     call        0000000140D41B30
  0000000140D7AB87: 83 F8 02           cmp         eax,2
  0000000140D7AB8A: 75 09              jne         0000000140D7AB95
  0000000140D7AB8C: 0F B6 44 24 48     movzx       eax,byte ptr [rsp+48h]
  0000000140D7AB91: 85 C0              test        eax,eax
  0000000140D7AB93: 74 14              je          0000000140D7ABA9
  0000000140D7AB95: C6 44 24 20 01     mov         byte ptr [rsp+20h],1
  0000000140D7AB9A: 0F B6 54 24 20     movzx       edx,byte ptr [rsp+20h]
  0000000140D7AB9F: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140D7ABA4: E8 47 FE FF FF     call        0000000140D7A9F0
  0000000140D7ABA9: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140D7ABAE: 48 83 C4 38        add         rsp,38h
  0000000140D7ABB2: C3                 ret

  Summary

     1E02000 .text
