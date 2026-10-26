Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140414030: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140414035: 48 83 EC 38        sub         rsp,38h
  0000000140414039: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014041403E: E8 9D FF FF FF     call        0000000140413FE0
  0000000140414043: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140414048: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  000000014041404E: 75 04              jne         0000000140414054
  0000000140414050: 33 C0              xor         eax,eax
  0000000140414052: EB 20              jmp         0000000140414074
  0000000140414054: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140414059: E8 52 C5 87 00     call        0000000140C905B0
  000000014041405E: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140414063: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140414069: 75 04              jne         000000014041406F
  000000014041406B: 33 C0              xor         eax,eax
  000000014041406D: EB 05              jmp         0000000140414074
  000000014041406F: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140414074: 48 83 C4 38        add         rsp,38h
  0000000140414078: C3                 ret

  Summary

     1E02000 .text
