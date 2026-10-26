Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000654C1D20: 48 89 5C 24 10     mov         qword ptr [rsp+10h],rbx
  00000000654C1D25: 48 89 6C 24 18     mov         qword ptr [rsp+18h],rbp
  00000000654C1D2A: 56                 push        rsi
  00000000654C1D2B: 57                 push        rdi
  00000000654C1D2C: 41 54              push        r12
  00000000654C1D2E: 48 83 EC 20        sub         rsp,20h
  00000000654C1D32: 48 8B 59 08        mov         rbx,qword ptr [rcx+8]
  00000000654C1D36: 4C 8B E1           mov         r12,rcx
  00000000654C1D39: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000000654C1D3E: 41 8B F9           mov         edi,r9d
  00000000654C1D41: 41 8B F0           mov         esi,r8d
  00000000654C1D44: 48 8B EA           mov         rbp,rdx
  00000000654C1D47: FF 15 2B 4A 1D 00  call        qword ptr [0000000065696778h]
  00000000654C1D4D: 48 8B 8B B0 03 00  mov         rcx,qword ptr [rbx+00000000000003B0h]
                    00
  00000000654C1D54: 4C 8B 10           mov         r10,qword ptr [rax]
  00000000654C1D57: 4C 89 93 08 04 00  mov         qword ptr [rbx+0000000000000408h],r10
                    00
  00000000654C1D5E: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000654C1D61: FF 50 20           call        qword ptr [rax+20h]
  00000000654C1D64: 33 D2              xor         edx,edx
  00000000654C1D66: 48 8B CB           mov         rcx,rbx
  00000000654C1D69: E8 42 19 FE FF     call        00000000654A36B0
  00000000654C1D6E: 44 8B CF           mov         r9d,edi
  00000000654C1D71: 44 8B C6           mov         r8d,esi
  00000000654C1D74: 48 8B D5           mov         rdx,rbp
  00000000654C1D77: 49 8B CC           mov         rcx,r12
  00000000654C1D7A: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  00000000654C1D7F: 48 8B 6C 24 50     mov         rbp,qword ptr [rsp+50h]
  00000000654C1D84: 48 83 C4 20        add         rsp,20h
  00000000654C1D88: 41 5C              pop         r12
  00000000654C1D8A: 5F                 pop         rdi
  00000000654C1D8B: 5E                 pop         rsi
  00000000654C1D8C: E9 7F 3E FE FF     jmp         00000000654A5C10

  Summary

      695000 .text
