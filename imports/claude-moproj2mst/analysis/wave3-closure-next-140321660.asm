Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140321660: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140321665: 48 83 EC 38        sub         rsp,38h
  0000000140321669: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014032166E: E8 3D E4 FF FF     call        000000014031FAB0
  0000000140321673: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140321678: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  000000014032167E: 75 04              jne         0000000140321684
  0000000140321680: 32 C0              xor         al,al
  0000000140321682: EB 0A              jmp         000000014032168E
  0000000140321684: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140321689: E8 72 5E 0B 00     call        00000001403D7500
  000000014032168E: 48 83 C4 38        add         rsp,38h
  0000000140321692: C3                 ret

  Summary

     1E02000 .text
