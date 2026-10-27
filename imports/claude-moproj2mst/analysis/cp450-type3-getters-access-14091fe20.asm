Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014091FE20: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014091FE25: 48 83 EC 28        sub         rsp,28h
  000000014091FE29: BA 01 00 00 00     mov         edx,1
  000000014091FE2E: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014091FE33: 48 8B 09           mov         rcx,qword ptr [rcx]
  000000014091FE36: FF 15 9C 44 4E 01  call        qword ptr [0000000141E042D8h]
  000000014091FE3C: 0F B6 C0           movzx       eax,al
  000000014091FE3F: 85 C0              test        eax,eax
  000000014091FE41: 74 0A              je          000000014091FE4D
  000000014091FE43: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014091FE48: E8 13 94 FF FF     call        0000000140919260
  000000014091FE4D: 48 83 C4 28        add         rsp,28h
  000000014091FE51: C3                 ret

  Summary

     1E02000 .text
