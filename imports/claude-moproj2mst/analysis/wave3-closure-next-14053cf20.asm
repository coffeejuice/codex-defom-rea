Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014053CF20: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  000000014053CF25: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  000000014053CF2A: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  000000014053CF2E: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014053CF33: 48 83 EC 28        sub         rsp,28h
  000000014053CF37: 4C 8B 4C 24 48     mov         r9,qword ptr [rsp+48h]
  000000014053CF3C: 44 8B 44 24 40     mov         r8d,dword ptr [rsp+40h]
  000000014053CF41: 8B 54 24 38        mov         edx,dword ptr [rsp+38h]
  000000014053CF45: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014053CF4A: FF 15 28 77 8C 01  call        qword ptr [0000000141E04678h]
  000000014053CF50: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  000000014053CF54: 83 7C 24 40 00     cmp         dword ptr [rsp+40h],0
  000000014053CF59: 7D 06              jge         000000014053CF61
  000000014053CF5B: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  000000014053CF5F: EB 35              jmp         000000014053CF96
  000000014053CF61: 83 7C 24 38 00     cmp         dword ptr [rsp+38h],0
  000000014053CF66: 75 2A              jne         000000014053CF92
  000000014053CF68: 83 7C 24 40 05     cmp         dword ptr [rsp+40h],5
  000000014053CF6D: 7D 18              jge         000000014053CF87
  000000014053CF6F: 4C 8B 4C 24 48     mov         r9,qword ptr [rsp+48h]
  000000014053CF74: 44 8B 44 24 40     mov         r8d,dword ptr [rsp+40h]
  000000014053CF79: 8B 54 24 38        mov         edx,dword ptr [rsp+38h]
  000000014053CF7D: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014053CF82: E8 F9 FD FF FF     call        000000014053CD80
  000000014053CF87: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  000000014053CF8B: 83 E8 05           sub         eax,5
  000000014053CF8E: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  000000014053CF92: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  000000014053CF96: 48 83 C4 28        add         rsp,28h
  000000014053CF9A: C3                 ret

  Summary

     1E02000 .text
