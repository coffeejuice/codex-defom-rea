Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000141204DE0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000141204DE5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000141204DEA: 48 83 EC 18        sub         rsp,18h
  0000000141204DEE: C7 04 24 00 00 00  mov         dword ptr [rsp],0
                    00
  0000000141204DF5: EB 09              jmp         0000000141204E00
  0000000141204DF7: 8B 04 24           mov         eax,dword ptr [rsp]
  0000000141204DFA: 83 C0 01           add         eax,1
  0000000141204DFD: 89 04 24           mov         dword ptr [rsp],eax
  0000000141204E00: 83 3C 24 03        cmp         dword ptr [rsp],3
  0000000141204E04: 7D 20              jge         0000000141204E26
  0000000141204E06: 4C 63 04 24        movsxd      r8,dword ptr [rsp]
  0000000141204E0A: 48 63 14 24        movsxd      rdx,dword ptr [rsp]
  0000000141204E0E: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000141204E13: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000141204E18: F2 42 0F 10 04 C0  movsd       xmm0,mmword ptr [rax+r8*8]
  0000000141204E1E: F2 0F 11 44 D1 30  movsd       mmword ptr [rcx+rdx*8+30h],xmm0
  0000000141204E24: EB D1              jmp         0000000141204DF7
  0000000141204E26: 48 83 C4 18        add         rsp,18h
  0000000141204E2A: C3                 ret

  Summary

     1E02000 .text
