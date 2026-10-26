Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D48BA0: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  0000000140D48BA5: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  0000000140D48BAA: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140D48BAE: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D48BB3: 48 83 EC 28        sub         rsp,28h
  0000000140D48BB7: 4C 8B 4C 24 48     mov         r9,qword ptr [rsp+48h]
  0000000140D48BBC: 44 8B 44 24 40     mov         r8d,dword ptr [rsp+40h]
  0000000140D48BC1: 8B 54 24 38        mov         edx,dword ptr [rsp+38h]
  0000000140D48BC5: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140D48BCA: FF 15 88 F5 0B 01  call        qword ptr [0000000141E08158h]
  0000000140D48BD0: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  0000000140D48BD4: 83 7C 24 40 00     cmp         dword ptr [rsp+40h],0
  0000000140D48BD9: 7D 06              jge         0000000140D48BE1
  0000000140D48BDB: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000140D48BDF: EB 35              jmp         0000000140D48C16
  0000000140D48BE1: 83 7C 24 38 00     cmp         dword ptr [rsp+38h],0
  0000000140D48BE6: 75 2A              jne         0000000140D48C12
  0000000140D48BE8: 83 7C 24 40 01     cmp         dword ptr [rsp+40h],1
  0000000140D48BED: 7D 18              jge         0000000140D48C07
  0000000140D48BEF: 4C 8B 4C 24 48     mov         r9,qword ptr [rsp+48h]
  0000000140D48BF4: 44 8B 44 24 40     mov         r8d,dword ptr [rsp+40h]
  0000000140D48BF9: 8B 54 24 38        mov         edx,dword ptr [rsp+38h]
  0000000140D48BFD: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140D48C02: E8 79 FE FF FF     call        0000000140D48A80
  0000000140D48C07: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000140D48C0B: 83 E8 01           sub         eax,1
  0000000140D48C0E: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  0000000140D48C12: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000140D48C16: 48 83 C4 28        add         rsp,28h
  0000000140D48C1A: C3                 ret

  Summary

     1E02000 .text
