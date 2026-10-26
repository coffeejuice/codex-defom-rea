Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000141204E80: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000141204E85: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000141204E8A: 48 83 EC 18        sub         rsp,18h
  0000000141204E8E: C7 04 24 00 00 00  mov         dword ptr [rsp],0
                    00
  0000000141204E95: EB 09              jmp         0000000141204EA0
  0000000141204E97: 8B 04 24           mov         eax,dword ptr [rsp]
  0000000141204E9A: 83 C0 01           add         eax,1
  0000000141204E9D: 89 04 24           mov         dword ptr [rsp],eax
  0000000141204EA0: 83 3C 24 03        cmp         dword ptr [rsp],3
  0000000141204EA4: 7D 20              jge         0000000141204EC6
  0000000141204EA6: 4C 63 04 24        movsxd      r8,dword ptr [rsp]
  0000000141204EAA: 48 63 14 24        movsxd      rdx,dword ptr [rsp]
  0000000141204EAE: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000141204EB3: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000141204EB8: F2 42 0F 10 04 C0  movsd       xmm0,mmword ptr [rax+r8*8]
  0000000141204EBE: F2 0F 11 44 D1 48  movsd       mmword ptr [rcx+rdx*8+48h],xmm0
  0000000141204EC4: EB D1              jmp         0000000141204E97
  0000000141204EC6: 48 83 C4 18        add         rsp,18h
  0000000141204ECA: C3                 ret

  Summary

     1E02000 .text
