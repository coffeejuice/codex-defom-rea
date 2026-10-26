Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140025650: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140025655: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014002565A: 48 83 EC 28        sub         rsp,28h
  000000014002565E: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140025663: 48 63 48 0C        movsxd      rcx,dword ptr [rax+0Ch]
  0000000140025667: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  000000014002566C: 4C 8D 44 C8 18     lea         r8,[rax+rcx*8+18h]
  0000000140025671: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140025676: 48 63 48 08        movsxd      rcx,dword ptr [rax+8]
  000000014002567A: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  000000014002567F: 48 8D 54 C8 18     lea         rdx,[rax+rcx*8+18h]
  0000000140025684: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140025689: E8 B2 01 00 00     call        0000000140025840
  000000014002568E: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  0000000140025693: FF 15 5F EB DD 01  call        qword ptr [0000000141E041F8h]
  0000000140025699: 48 83 C4 28        add         rsp,28h
  000000014002569D: C3                 ret

  Summary

     1E02000 .text
