Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001412075C0: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  00000001412075C4: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001412075C9: 83 7C 24 10 00     cmp         dword ptr [rsp+10h],0
  00000001412075CE: 7E 07              jle         00000001412075D7
  00000001412075D0: 83 7C 24 10 2B     cmp         dword ptr [rsp+10h],2Bh
  00000001412075D5: 7C 02              jl          00000001412075D9
  00000001412075D7: EB 60              jmp         0000000141207639
  00000001412075D9: 48 63 4C 24 10     movsxd      rcx,dword ptr [rsp+10h]
  00000001412075DE: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  00000001412075E3: 83 BC 88 78 42 00  cmp         dword ptr [rax+rcx*4+0000000000004278h],0
                    00 00
  00000001412075EB: 7E 4C              jle         0000000141207639
  00000001412075ED: 48 63 4C 24 10     movsxd      rcx,dword ptr [rsp+10h]
  00000001412075F2: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  00000001412075F7: 8B 94 88 78 42 00  mov         edx,dword ptr [rax+rcx*4+0000000000004278h]
                    00
  00000001412075FE: 83 EA 01           sub         edx,1
  0000000141207601: 48 63 4C 24 10     movsxd      rcx,dword ptr [rsp+10h]
  0000000141207606: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  000000014120760B: 89 94 88 78 42 00  mov         dword ptr [rax+rcx*4+0000000000004278h],edx
                    00
  0000000141207612: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  0000000141207617: 83 B8 78 42 00 00  cmp         dword ptr [rax+0000000000004278h],0
                    00
  000000014120761E: 7E 19              jle         0000000141207639
  0000000141207620: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  0000000141207625: 8B 88 78 42 00 00  mov         ecx,dword ptr [rax+0000000000004278h]
  000000014120762B: 83 E9 01           sub         ecx,1
  000000014120762E: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  0000000141207633: 89 88 78 42 00 00  mov         dword ptr [rax+0000000000004278h],ecx
  0000000141207639: F3 C3              rep ret
  000000014120763B: CC                 int         3
  000000014120763C: CC                 int         3
  000000014120763D: CC                 int         3
  000000014120763E: CC                 int         3
  000000014120763F: CC                 int         3
  0000000141207640: CC                 int         3
  0000000141207641: CC                 int         3
  0000000141207642: CC                 int         3
  0000000141207643: CC                 int         3
  0000000141207644: CC                 int         3
  0000000141207645: CC                 int         3
  0000000141207646: CC                 int         3
  0000000141207647: CC                 int         3
  0000000141207648: CC                 int         3
  0000000141207649: CC                 int         3
  000000014120764A: CC                 int         3
  000000014120764B: CC                 int         3
  000000014120764C: CC                 int         3
  000000014120764D: CC                 int         3
  000000014120764E: CC                 int         3
  000000014120764F: CC                 int         3
  0000000141207650: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000141207654: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000141207659: 48 83 EC 18        sub         rsp,18h
  000000014120765D: 83 7C 24 28 00     cmp         dword ptr [rsp+28h],0
  0000000141207662: 7E 07              jle         000000014120766B
  0000000141207664: 83 7C 24 28 2B     cmp         dword ptr [rsp+28h],2Bh
  0000000141207669: 7C 04              jl          000000014120766F
  000000014120766B: 32 C0              xor         al,al
  000000014120766D: EB 27              jmp         0000000141207696
  000000014120766F: 48 63 4C 24 28     movsxd      rcx,dword ptr [rsp+28h]
  0000000141207674: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000141207679: 83
  000000014120767A: BC 88 78 42 00     mov         esp,427888h
  000000014120767F: 00

  Summary

     1E02000 .text
