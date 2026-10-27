Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014124A010: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014124A015: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014124A01A: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  000000014124A01F: 83 B8 6C 84 00 00  cmp         dword ptr [rax+000000000000846Ch],0FFFFFFFFh
                    FF
  000000014124A026: 75 14              jne         000000014124A03C
  000000014124A028: 48 8B 4C 24 08     mov         rcx,qword ptr [rsp+8]
  000000014124A02D: 48 8B 44 24 10     mov         rax,qword ptr [rsp+10h]
  000000014124A032: 8B 00              mov         eax,dword ptr [rax]
  000000014124A034: 89 81 6C 84 00 00  mov         dword ptr [rcx+000000000000846Ch],eax
  000000014124A03A: EB 20              jmp         000000014124A05C
  000000014124A03C: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  000000014124A041: 83 B8 70 84 00 00  cmp         dword ptr [rax+0000000000008470h],0FFFFFFFFh
                    FF
  000000014124A048: 75 12              jne         000000014124A05C
  000000014124A04A: 48 8B 4C 24 08     mov         rcx,qword ptr [rsp+8]
  000000014124A04F: 48 8B 44 24 10     mov         rax,qword ptr [rsp+10h]
  000000014124A054: 8B 00              mov         eax,dword ptr [rax]
  000000014124A056: 89 81 70 84 00 00  mov         dword ptr [rcx+0000000000008470h],eax
  000000014124A05C: F3 C3              rep ret
  000000014124A05E: CC                 int         3
  000000014124A05F: CC                 int         3
  000000014124A060: CC                 int         3
  000000014124A061: CC                 int         3
  000000014124A062: CC                 int         3
  000000014124A063: CC                 int         3
  000000014124A064: CC                 int         3
  000000014124A065: CC                 int         3
  000000014124A066: CC                 int         3
  000000014124A067: CC                 int         3
  000000014124A068: CC                 int         3
  000000014124A069: CC                 int         3
  000000014124A06A: CC                 int         3
  000000014124A06B: CC                 int         3
  000000014124A06C: CC                 int         3
  000000014124A06D: CC                 int         3
  000000014124A06E: CC                 int         3
  000000014124A06F: CC                 int         3

  Summary

     1E02000 .text
