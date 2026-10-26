Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000141763360: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  0000000141763365: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  000000014176336A: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  000000014176336E: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000141763373: 48 83 EC 28        sub         rsp,28h
  0000000141763377: 4C 8B 4C 24 48     mov         r9,qword ptr [rsp+48h]
  000000014176337C: 44 8B 44 24 40     mov         r8d,dword ptr [rsp+40h]
  0000000141763381: 8B 54 24 38        mov         edx,dword ptr [rsp+38h]
  0000000141763385: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014176338A: FF 15 D0 86 6A 00  call        qword ptr [0000000141E0BA60h]
  0000000141763390: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  0000000141763394: 83 7C 24 40 00     cmp         dword ptr [rsp+40h],0
  0000000141763399: 7D 06              jge         00000001417633A1
  000000014176339B: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  000000014176339F: EB 35              jmp         00000001417633D6
  00000001417633A1: 83 7C 24 38 00     cmp         dword ptr [rsp+38h],0
  00000001417633A6: 75 2A              jne         00000001417633D2
  00000001417633A8: 83 7C 24 40 0B     cmp         dword ptr [rsp+40h],0Bh
  00000001417633AD: 7D 18              jge         00000001417633C7
  00000001417633AF: 4C 8B 4C 24 48     mov         r9,qword ptr [rsp+48h]
  00000001417633B4: 44 8B 44 24 40     mov         r8d,dword ptr [rsp+40h]
  00000001417633B9: 8B 54 24 38        mov         edx,dword ptr [rsp+38h]
  00000001417633BD: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001417633C2: E8 59 FD FF FF     call        0000000141763120
  00000001417633C7: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  00000001417633CB: 83 E8 0B           sub         eax,0Bh
  00000001417633CE: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  00000001417633D2: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  00000001417633D6: 48 83 C4 28        add         rsp,28h
  00000001417633DA: C3                 ret

  Summary

     1E02000 .text
