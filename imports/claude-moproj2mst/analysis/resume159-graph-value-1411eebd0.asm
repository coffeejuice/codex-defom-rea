Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001411EEBD0: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  00000001411EEBD4: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001411EEBD9: 83 7C 24 10 00     cmp         dword ptr [rsp+10h],0
  00000001411EEBDE: 7D 04              jge         00000001411EEBE4
  00000001411EEBE0: 33 C0              xor         eax,eax
  00000001411EEBE2: EB 1C              jmp         00000001411EEC00
  00000001411EEBE4: 48 63 4C 24 10     movsxd      rcx,dword ptr [rsp+10h]
  00000001411EEBE9: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  00000001411EEBEE: 0F B6 44 08 08     movzx       eax,byte ptr [rax+rcx+8]
  00000001411EEBF3: 85 C0              test        eax,eax
  00000001411EEBF5: 74 07              je          00000001411EEBFE
  00000001411EEBF7: B8 02 00 00 00     mov         eax,2
  00000001411EEBFC: EB 02              jmp         00000001411EEC00
  00000001411EEBFE: 33 C0              xor         eax,eax
  00000001411EEC00: F3 C3              rep ret
  00000001411EEC02: CC                 int         3
  00000001411EEC03: CC                 int         3
  00000001411EEC04: CC                 int         3
  00000001411EEC05: CC                 int         3
  00000001411EEC06: CC                 int         3
  00000001411EEC07: CC                 int         3
  00000001411EEC08: CC                 int         3
  00000001411EEC09: CC                 int         3
  00000001411EEC0A: CC                 int         3
  00000001411EEC0B: CC                 int         3
  00000001411EEC0C: CC                 int         3
  00000001411EEC0D: CC                 int         3
  00000001411EEC0E: CC                 int         3
  00000001411EEC0F: CC                 int         3
  00000001411EEC10: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  00000001411EEC15: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  00000001411EEC19: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001411EEC1E: 48 83 EC 28        sub         rsp,28h
  00000001411EEC22: 83 7C 24 38 00     cmp         dword ptr [rsp+38h],0
  00000001411EEC27: 7D 02              jge         00000001411EEC2B
  00000001411EEC29: EB 36              jmp         00000001411EEC61
  00000001411EEC2B: 83 7C 24 40 02     cmp         dword ptr [rsp+40h],2
  00000001411EEC30: 75 11              jne         00000001411EEC43
  00000001411EEC32: 48 63 4C 24 38     movsxd      rcx,dword ptr [rsp+38h]
  00000001411EEC37: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001411EEC3C: C6 44 08 08 01     mov         byte ptr [rax+rcx+8],1
  00000001411EEC41: EB 0F              jmp         00000001411EEC52
  00000001411EEC43: 48 63 4C 24 38     movsxd      rcx,dword ptr [rsp+38h]
  00000001411EEC48: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001411EEC4D: C6
  00000001411EEC4E: 44
  00000001411EEC4F: 08

  Summary

     1E02000 .text
