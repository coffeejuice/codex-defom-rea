Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014031FAB0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014031FAB5: 48 83 EC 58        sub         rsp,58h
  000000014031FAB9: 48 C7 44 24 38 FE  mov         qword ptr [rsp+38h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000014031FAC2: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  000000014031FAC7: 48 83 B8 C8 04 00  cmp         qword ptr [rax+00000000000004C8h],0
                    00 00
  000000014031FACF: 75 4C              jne         000000014031FB1D
  000000014031FAD1: B9 F8 00 00 00     mov         ecx,0F8h
  000000014031FAD6: E8 E1 EF 14 01     call        000000014146EABC
  000000014031FADB: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014031FAE0: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  000000014031FAE6: 74 11              je          000000014031FAF9
  000000014031FAE8: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014031FAED: E8 CE 5A 0B 00     call        00000001403D55C0
  000000014031FAF2: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  000000014031FAF7: EB 09              jmp         000000014031FB02
  000000014031FAF9: 48 C7 44 24 40 00  mov         qword ptr [rsp+40h],0
                    00 00 00
  000000014031FB02: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014031FB07: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000014031FB0C: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000014031FB11: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  000000014031FB16: 48 89 81 C8 04 00  mov         qword ptr [rcx+00000000000004C8h],rax
                    00
  000000014031FB1D: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  000000014031FB22: 48 83 B8 C8 04 00  cmp         qword ptr [rax+00000000000004C8h],0
                    00 00
  000000014031FB2A: 74 35              je          000000014031FB61
  000000014031FB2C: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000014031FB31: E8 7A A7 FE FF     call        000000014030A2B0
  000000014031FB36: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014031FB3B: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  000000014031FB41: 74 1E              je          000000014031FB61
  000000014031FB43: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000014031FB48: E8 23 15 97 00     call        0000000140C91070
  000000014031FB4D: 48 8B 54 24 60     mov         rdx,qword ptr [rsp+60h]
  000000014031FB52: 48 8B 92 C8 04 00  mov         rdx,qword ptr [rdx+00000000000004C8h]
                    00
  000000014031FB59: 48 8B C8           mov         rcx,rax
  000000014031FB5C: E8 1F 4C A4 00     call        0000000140D64780
  000000014031FB61: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  000000014031FB66: 48 8B 80 C8 04 00  mov         rax,qword ptr [rax+00000000000004C8h]
                    00
  000000014031FB6D: 48 83 C4 58        add         rsp,58h
  000000014031FB71: C3                 ret

  Summary

     1E02000 .text
