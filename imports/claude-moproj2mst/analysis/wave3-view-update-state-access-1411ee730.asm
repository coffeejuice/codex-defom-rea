Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001411EE730: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001411EE735: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001411EE73A: 48 83 EC 18        sub         rsp,18h
  00000001411EE73E: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  00000001411EE744: 74 46              je          00000001411EE78C
  00000001411EE746: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001411EE74B: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  00000001411EE750: 8B 40 04           mov         eax,dword ptr [rax+4]
  00000001411EE753: 89 41 04           mov         dword ptr [rcx+4],eax
  00000001411EE756: C7 04 24 00 00 00  mov         dword ptr [rsp],0
                    00
  00000001411EE75D: EB 09              jmp         00000001411EE768
  00000001411EE75F: 8B 04 24           mov         eax,dword ptr [rsp]
  00000001411EE762: 83 C0 01           add         eax,1
  00000001411EE765: 89 04 24           mov         dword ptr [rsp],eax
  00000001411EE768: 83 3C 24 2E        cmp         dword ptr [rsp],2Eh
  00000001411EE76C: 7D 1E              jge         00000001411EE78C
  00000001411EE76E: 4C 63 04 24        movsxd      r8,dword ptr [rsp]
  00000001411EE772: 48 63 14 24        movsxd      rdx,dword ptr [rsp]
  00000001411EE776: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001411EE77B: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  00000001411EE780: 42 0F B6 44 00 08  movzx       eax,byte ptr [rax+r8+8]
  00000001411EE786: 88 44 11 08        mov         byte ptr [rcx+rdx+8],al
  00000001411EE78A: EB D3              jmp         00000001411EE75F
  00000001411EE78C: 48 83 C4 18        add         rsp,18h
  00000001411EE790: C3                 ret

  Summary

     1E02000 .text
