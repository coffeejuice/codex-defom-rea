Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140919290: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140919295: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014091929A: 48 83 EC 28        sub         rsp,28h
  000000014091929E: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  00000001409192A3: 48 63 48 0C        movsxd      rcx,dword ptr [rax+0Ch]
  00000001409192A7: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  00000001409192AC: 4C 8D 44 C8 18     lea         r8,[rax+rcx*8+18h]
  00000001409192B1: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  00000001409192B6: 48 63 48 08        movsxd      rcx,dword ptr [rax+8]
  00000001409192BA: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  00000001409192BF: 48 8D 54 C8 18     lea         rdx,[rax+rcx*8+18h]
  00000001409192C4: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001409192C9: E8 32 01 00 00     call        0000000140919400
  00000001409192CE: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  00000001409192D3: FF 15 1F AF 4E 01  call        qword ptr [0000000141E041F8h]
  00000001409192D9: 48 83 C4 28        add         rsp,28h
  00000001409192DD: C3                 ret

  Summary

     1E02000 .text
