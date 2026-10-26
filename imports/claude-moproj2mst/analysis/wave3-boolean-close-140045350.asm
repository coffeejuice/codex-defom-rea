Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140045350: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140045354: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140045359: 48 83 EC 28        sub         rsp,28h
  000000014004535D: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140045362: E8 D9 EC FF FF     call        0000000140044040
  0000000140045367: 8B 44 24 38        mov         eax,dword ptr [rsp+38h]
  000000014004536B: 83 E0 01           and         eax,1
  000000014004536E: 85 C0              test        eax,eax
  0000000140045370: 74 0A              je          000000014004537C
  0000000140045372: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140045377: E8 EC A8 49 01     call        00000001414DFC68
  000000014004537C: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140045381: 48 83 C4 28        add         rsp,28h
  0000000140045385: C3                 ret

  Summary

     1E02000 .text
