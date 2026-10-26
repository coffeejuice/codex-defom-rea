Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140131230: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140131235: 48 83 EC 38        sub         rsp,38h
  0000000140131239: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014013123E: E8 AD F8 FF FF     call        0000000140130AF0
  0000000140131243: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140131248: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  000000014013124E: 74 39              je          0000000140131289
  0000000140131250: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140131255: E8 36 3C F2 FF     call        0000000140054E90
  000000014013125A: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000014013125F: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  0000000140131265: 74 22              je          0000000140131289
  0000000140131267: 33 D2              xor         edx,edx
  0000000140131269: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000014013126E: E8 CD 3C F2 FF     call        0000000140054F40
  0000000140131273: 48 8B D0           mov         rdx,rax
  0000000140131276: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  000000014013127B: 48 8B 00           mov         rax,qword ptr [rax]
  000000014013127E: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140131283: FF 90 60 02 00 00  call        qword ptr [rax+0000000000000260h]
  0000000140131289: 48 83 C4 38        add         rsp,38h
  000000014013128D: C3                 ret

  Summary

     1E02000 .text
