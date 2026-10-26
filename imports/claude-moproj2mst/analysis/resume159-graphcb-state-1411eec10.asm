Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

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
  00000001411EEC4D: C6 44 08 08 00     mov         byte ptr [rax+rcx+8],0
  00000001411EEC52: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  00000001411EEC57: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001411EEC5C: E8 1F FE FF FF     call        00000001411EEA80
  00000001411EEC61: 48 83 C4 28        add         rsp,28h
  00000001411EEC65: C3                 ret

  Summary

     1E02000 .text
