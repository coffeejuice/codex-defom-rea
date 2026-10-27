Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000141249420: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000141249425: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  000000014124942A: 8B 80 68 84 00 00  mov         eax,dword ptr [rax+0000000000008468h]
  0000000141249430: C3                 ret
  0000000141249431: CC                 int         3
  0000000141249432: CC                 int         3
  0000000141249433: CC                 int         3
  0000000141249434: CC                 int         3
  0000000141249435: CC                 int         3
  0000000141249436: CC                 int         3
  0000000141249437: CC                 int         3
  0000000141249438: CC                 int         3
  0000000141249439: CC                 int         3
  000000014124943A: CC                 int         3
  000000014124943B: CC                 int         3
  000000014124943C: CC                 int         3
  000000014124943D: CC                 int         3
  000000014124943E: CC                 int         3
  000000014124943F: CC                 int         3
  0000000141249440: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000141249445: 48 83 EC 48        sub         rsp,48h
  0000000141249449: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000141249451: C7 44 24 30 00 00  mov         dword ptr [rsp+30h],0
                    00 00
  0000000141249459: EB 0B              jmp         0000000141249466
  000000014124945B: 8B 44 24 30        mov         eax,dword ptr [rsp+30h]
  000000014124945F: 83 C0 01           add         eax,1
  0000000141249462: 89 44 24 30        mov         dword ptr [rsp+30h],eax
  0000000141249466: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014124946B: E8 B0 FF FF FF     call        0000000141249420
  0000000141249470: 39 44 24 30        cmp         dword ptr [rsp+30h],eax
  0000000141249474: 7D 37              jge         00000001412494AD
  0000000141249476: 48 63 4C 24 30     movsxd      rcx,dword ptr [rsp+30h]
  000000014124947B: 48
  000000014124947C: 69
  000000014124947D: C9                 leave
  000000014124947E: 08 01              or          byte ptr [rcx],al

  Summary

     1E02000 .text
