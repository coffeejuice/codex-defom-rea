Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000141760B40: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000141760B45: 48 83 EC 38        sub         rsp,38h
  0000000141760B49: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000141760B4E: 0F B6 80 A0 00 00  movzx       eax,byte ptr [rax+00000000000000A0h]
                    00
  0000000141760B55: 85 C0              test        eax,eax
  0000000141760B57: 75 65              jne         0000000141760BBE
  0000000141760B59: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000141760B5E: 0F B6 80 A1 00 00  movzx       eax,byte ptr [rax+00000000000000A1h]
                    00
  0000000141760B65: 85 C0              test        eax,eax
  0000000141760B67: 75 55              jne         0000000141760BBE
  0000000141760B69: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141760B6E: E8 5D EF FF FF     call        000000014175FAD0
  0000000141760B73: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000141760B78: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000141760B7E: 75 02              jne         0000000141760B82
  0000000141760B80: EB 3C              jmp         0000000141760BBE
  0000000141760B82: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000141760B87: E8 E4 04 53 FF     call        0000000140C91070
  0000000141760B8C: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000141760B91: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  0000000141760B97: 74 0F              je          0000000141760BA8
  0000000141760B99: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  0000000141760B9E: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000141760BA3: E8 28 3C 60 FF     call        0000000140D647D0
  0000000141760BA8: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000141760BAD: C7 40 18 00 00 00  mov         dword ptr [rax+18h],0
                    00
  0000000141760BB4: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141760BB9: E8 D2 ED FF FF     call        000000014175F990
  0000000141760BBE: 48 83 C4 38        add         rsp,38h
  0000000141760BC2: C3                 ret

  Summary

     1E02000 .text
