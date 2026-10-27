Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000141247D90: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000141247D95: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000141247D9A: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  0000000141247D9F: 83 B8 FC 00 00 00  cmp         dword ptr [rax+00000000000000FCh],0FFFFFFFFh
                    FF
  0000000141247DA6: 75 14              jne         0000000141247DBC
  0000000141247DA8: 48 8B 4C 24 08     mov         rcx,qword ptr [rsp+8]
  0000000141247DAD: 48 8B 44 24 10     mov         rax,qword ptr [rsp+10h]
  0000000141247DB2: 8B 00              mov         eax,dword ptr [rax]
  0000000141247DB4: 89 81 FC 00 00 00  mov         dword ptr [rcx+00000000000000FCh],eax
  0000000141247DBA: EB 20              jmp         0000000141247DDC
  0000000141247DBC: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  0000000141247DC1: 83 B8 00 01 00 00  cmp         dword ptr [rax+0000000000000100h],0FFFFFFFFh
                    FF
  0000000141247DC8: 75 12              jne         0000000141247DDC
  0000000141247DCA: 48 8B 4C 24 08     mov         rcx,qword ptr [rsp+8]
  0000000141247DCF: 48 8B 44 24 10     mov         rax,qword ptr [rsp+10h]
  0000000141247DD4: 8B 00              mov         eax,dword ptr [rax]
  0000000141247DD6: 89 81 00 01 00 00  mov         dword ptr [rcx+0000000000000100h],eax
  0000000141247DDC: F3 C3              rep ret
  0000000141247DDE: CC                 int         3
  0000000141247DDF: CC                 int         3
  0000000141247DE0: CC                 int         3
  0000000141247DE1: CC                 int         3
  0000000141247DE2: CC                 int         3
  0000000141247DE3: CC                 int         3
  0000000141247DE4: CC                 int         3
  0000000141247DE5: CC                 int         3
  0000000141247DE6: CC                 int         3
  0000000141247DE7: CC                 int         3
  0000000141247DE8: CC                 int         3
  0000000141247DE9: CC                 int         3
  0000000141247DEA: CC                 int         3
  0000000141247DEB: CC                 int         3
  0000000141247DEC: CC                 int         3
  0000000141247DED: CC                 int         3
  0000000141247DEE: CC                 int         3
  0000000141247DEF: CC                 int         3

  Summary

     1E02000 .text
