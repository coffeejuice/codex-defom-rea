Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000141207560: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000141207564: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000141207569: 83 7C 24 10 00     cmp         dword ptr [rsp+10h],0
  000000014120756E: 7E 07              jle         0000000141207577
  0000000141207570: 83 7C 24 10 2B     cmp         dword ptr [rsp+10h],2Bh
  0000000141207575: 7C 02              jl          0000000141207579
  0000000141207577: EB 3E              jmp         00000001412075B7
  0000000141207579: 48 63 4C 24 10     movsxd      rcx,dword ptr [rsp+10h]
  000000014120757E: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  0000000141207583: 8B 94 88 78 42 00  mov         edx,dword ptr [rax+rcx*4+0000000000004278h]
                    00
  000000014120758A: 83 C2 01           add         edx,1
  000000014120758D: 48 63 4C 24 10     movsxd      rcx,dword ptr [rsp+10h]
  0000000141207592: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  0000000141207597: 89 94 88 78 42 00  mov         dword ptr [rax+rcx*4+0000000000004278h],edx
                    00
  000000014120759E: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  00000001412075A3: 8B 88 78 42 00 00  mov         ecx,dword ptr [rax+0000000000004278h]
  00000001412075A9: 83 C1 01           add         ecx,1
  00000001412075AC: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  00000001412075B1: 89 88 78 42 00 00  mov         dword ptr [rax+0000000000004278h],ecx
  00000001412075B7: F3 C3              rep ret
  00000001412075B9: CC                 int         3
  00000001412075BA: CC                 int         3
  00000001412075BB: CC                 int         3
  00000001412075BC: CC                 int         3
  00000001412075BD: CC                 int         3
  00000001412075BE: CC                 int         3
  00000001412075BF: CC                 int         3
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

  Summary

     1E02000 .text
