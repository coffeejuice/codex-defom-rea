Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001408188F0: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  00000001408188F4: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001408188F9: 48 83 EC 28        sub         rsp,28h
  00000001408188FD: 83 7C 24 38 00     cmp         dword ptr [rsp+38h],0
  0000000140818902: 7C 46              jl          000000014081894A
  0000000140818904: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140818909: FF 15 01 B9 5E 01  call        qword ptr [0000000141E04210h]
  000000014081890F: 39 44 24 38        cmp         dword ptr [rsp+38h],eax
  0000000140818913: 7D 35              jge         000000014081894A
  0000000140818915: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014081891A: E8 B1 50 94 FF     call        000000014015D9D0
  000000014081891F: 8B 54 24 38        mov         edx,dword ptr [rsp+38h]
  0000000140818923: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140818928: FF 15 DA B8 5E 01  call        qword ptr [0000000141E04208h]
  000000014081892E: 48 8B D0           mov         rdx,rax
  0000000140818931: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140818936: E8 E5 1D B8 00     call        000000014139A720
  000000014081893B: 8B 54 24 38        mov         edx,dword ptr [rsp+38h]
  000000014081893F: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140818944: FF 15 56 CF 5E 01  call        qword ptr [0000000141E058A0h]
  000000014081894A: 48 83 C4 28        add         rsp,28h
  000000014081894E: C3                 ret

  Summary

     1E02000 .text
