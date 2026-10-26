Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140B3E8E0: 44 89 4C 24 20     mov         dword ptr [rsp+20h],r9d
  0000000140B3E8E5: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  0000000140B3E8EA: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140B3E8EE: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140B3E8F3: 48 83 EC 48        sub         rsp,48h
  0000000140B3E8F7: 48 C7 44 24 28 00  mov         qword ptr [rsp+28h],0
                    00 00 00
  0000000140B3E900: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140B3E905: E8 B6 02 00 00     call        0000000140B3EBC0
  0000000140B3E90A: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  0000000140B3E90E: C7 44 24 30 00 00  mov         dword ptr [rsp+30h],0
                    00 00
  0000000140B3E916: EB 0B              jmp         0000000140B3E923
  0000000140B3E918: 8B 44 24 30        mov         eax,dword ptr [rsp+30h]
  0000000140B3E91C: 83 C0 01           add         eax,1
  0000000140B3E91F: 89 44 24 30        mov         dword ptr [rsp+30h],eax
  0000000140B3E923: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  0000000140B3E927: 39 44 24 30        cmp         dword ptr [rsp+30h],eax
  0000000140B3E92B: 7D 37              jge         0000000140B3E964
  0000000140B3E92D: 48 63 54 24 30     movsxd      rdx,dword ptr [rsp+30h]
  0000000140B3E932: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140B3E937: E8 04 FA D2 FF     call        000000014086E340
  0000000140B3E93C: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140B3E93F: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140B3E944: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140B3E949: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140B3E94C: 44 8B 4C 24 68     mov         r9d,dword ptr [rsp+68h]
  0000000140B3E951: 4C 8B 44 24 60     mov         r8,qword ptr [rsp+60h]
  0000000140B3E956: 8B 54 24 58        mov         edx,dword ptr [rsp+58h]
  0000000140B3E95A: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140B3E95F: FF 50 10           call        qword ptr [rax+10h]
  0000000140B3E962: EB B4              jmp         0000000140B3E918
  0000000140B3E964: 48 83 C4 48        add         rsp,48h
  0000000140B3E968: C3                 ret

  Summary

     1E02000 .text
