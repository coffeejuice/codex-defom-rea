Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014030A2B0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014030A2B5: 48 83 EC 38        sub         rsp,38h
  000000014030A2B9: 48 C7 44 24 20 00  mov         qword ptr [rsp+20h],0
                    00 00 00
  000000014030A2C2: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014030A2C7: E8 C4 FF FF FF     call        000000014030A290
  000000014030A2CC: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000014030A2D1: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  000000014030A2D7: 74 18              je          000000014030A2F1
  000000014030A2D9: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  000000014030A2DE: 48 8B 00           mov         rax,qword ptr [rax]
  000000014030A2E1: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  000000014030A2E6: FF 90 68 02 00 00  call        qword ptr [rax+0000000000000268h]
  000000014030A2EC: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014030A2F1: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000014030A2F6: 48 83 C4 38        add         rsp,38h
  000000014030A2FA: C3                 ret

  Summary

     1E02000 .text
