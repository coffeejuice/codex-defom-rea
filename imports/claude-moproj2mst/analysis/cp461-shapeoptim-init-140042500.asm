Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140042500: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140042505: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014004250A: 48 83 EC 38        sub         rsp,38h
  000000014004250E: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140042513: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  0000000140042518: 48 8B 00           mov         rax,qword ptr [rax]
  000000014004251B: 48 39 01           cmp         qword ptr [rcx],rax
  000000014004251E: 74 68              je          0000000140042588
  0000000140042520: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  0000000140042525: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140042528: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014004252D: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140042532: FF 15 C0 1D DC 01  call        qword ptr [0000000141E042F8h]
  0000000140042538: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014004253D: 48 8B 09           mov         rcx,qword ptr [rcx]
  0000000140042540: FF 15 A2 1D DC 01  call        qword ptr [0000000141E042E8h]
  0000000140042546: 0F B6 C0           movzx       eax,al
  0000000140042549: 85 C0              test        eax,eax
  000000014004254B: 75 12              jne         000000014004255F
  000000014004254D: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  0000000140042552: 48 8B 12           mov         rdx,qword ptr [rdx]
  0000000140042555: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014004255A: E8 F1 30 FE FF     call        0000000140025650
  000000014004255F: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140042564: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140042569: 48 89 01           mov         qword ptr [rcx],rax
  000000014004256C: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140042571: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140042574: 8B 40 10           mov         eax,dword ptr [rax+10h]
  0000000140042577: 83 E0 01           and         eax,1
  000000014004257A: 85 C0              test        eax,eax
  000000014004257C: 75 0A              jne         0000000140042588
  000000014004257E: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140042583: E8 98 2D FE FF     call        0000000140025320
  0000000140042588: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014004258D: 48 83 C4 38        add         rsp,38h
  0000000140042591: C3                 ret

  Summary

     1E02000 .text
