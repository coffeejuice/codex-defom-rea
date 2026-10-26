Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000650A8F90: 33 C0              xor         eax,eax
  00000000650A8F92: 48 39 41 10        cmp         qword ptr [rcx+10h],rax
  00000000650A8F96: 0F 94 C0           sete        al
  00000000650A8F99: C3                 ret
  00000000650A8F9A: CC                 int         3
  00000000650A8F9B: CC                 int         3
  00000000650A8F9C: CC                 int         3
  00000000650A8F9D: CC                 int         3
  00000000650A8F9E: CC                 int         3
  00000000650A8F9F: CC                 int         3
  00000000650A8FA0: 48 8B 41 10        mov         rax,qword ptr [rcx+10h]
  00000000650A8FA4: 48 85 C0           test        rax,rax
  00000000650A8FA7: 74 04              je          00000000650A8FAD
  00000000650A8FA9: 8B 40 04           mov         eax,dword ptr [rax+4]
  00000000650A8FAC: C3                 ret
  00000000650A8FAD: 33 C0              xor         eax,eax
  00000000650A8FAF: C3                 ret
  00000000650A8FB0: 40 53              push        rbx
  00000000650A8FB2: 48 83 EC 20        sub         rsp,20h
  00000000650A8FB6: 48 83 79 10 00     cmp         qword ptr [rcx+10h],0
  00000000650A8FBB: 48 8B DA           mov         rbx,rdx
  00000000650A8FBE: 74 12              je          00000000650A8FD2
  00000000650A8FC0: 48 8B 41 10        mov         rax,qword ptr [rcx+10h]
  00000000650A8FC4: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000000650A8FC9: 44 8B 40 08        mov         r8d,dword ptr [rax+8]
  00000000650A8FCD: 8B 50 04           mov         edx,dword ptr [rax+4]

  Summary

      695000 .text
