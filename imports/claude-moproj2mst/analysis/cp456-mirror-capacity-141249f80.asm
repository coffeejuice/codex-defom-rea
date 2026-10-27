Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000141249F80: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  0000000141249F85: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000141249F8A: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000141249F8F: 48 83 EC 38        sub         rsp,38h
  0000000141249F93: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  0000000141249F98: 83 38 00           cmp         dword ptr [rax],0
  0000000141249F9B: 75 11              jne         0000000141249FAE
  0000000141249F9D: 48 8B 54 24 50     mov         rdx,qword ptr [rsp+50h]
  0000000141249FA2: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141249FA7: E8 64 00 00 00     call        000000014124A010
  0000000141249FAC: EB 2B              jmp         0000000141249FD9
  0000000141249FAE: 48 8B 54 24 48     mov         rdx,qword ptr [rsp+48h]
  0000000141249FB3: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141249FB8: E8 03 F5 FF FF     call        00000001412494C0
  0000000141249FBD: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000141249FC2: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  0000000141249FC8: 74 0F              je          0000000141249FD9
  0000000141249FCA: 48 8B 54 24 50     mov         rdx,qword ptr [rsp+50h]
  0000000141249FCF: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000141249FD4: E8 B7 DD FF FF     call        0000000141247D90
  0000000141249FD9: 48 8B 54 24 50     mov         rdx,qword ptr [rsp+50h]
  0000000141249FDE: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141249FE3: E8 D8 F4 FF FF     call        00000001412494C0
  0000000141249FE8: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000141249FED: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000141249FF3: 74 0F              je          000000014124A004
  0000000141249FF5: 48 8B 54 24 48     mov         rdx,qword ptr [rsp+48h]
  0000000141249FFA: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000141249FFF: E8 8C DD FF FF     call        0000000141247D90
  000000014124A004: 48 83 C4 38        add         rsp,38h
  000000014124A008: C3                 ret

  Summary

     1E02000 .text
