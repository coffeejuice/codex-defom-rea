Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014015D5E0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014015D5E5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014015D5EA: 48 83 EC 38        sub         rsp,38h
  000000014015D5EE: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014015D5F3: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  000000014015D5F8: 48 8B 00           mov         rax,qword ptr [rax]
  000000014015D5FB: 48 39 01           cmp         qword ptr [rcx],rax
  000000014015D5FE: 74 68              je          000000014015D668
  000000014015D600: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  000000014015D605: 48 8B 00           mov         rax,qword ptr [rax]
  000000014015D608: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014015D60D: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000014015D612: FF 15 E0 6C CA 01  call        qword ptr [0000000141E042F8h]
  000000014015D618: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014015D61D: 48 8B 09           mov         rcx,qword ptr [rcx]
  000000014015D620: FF 15 C2 6C CA 01  call        qword ptr [0000000141E042E8h]
  000000014015D626: 0F B6 C0           movzx       eax,al
  000000014015D629: 85 C0              test        eax,eax
  000000014015D62B: 75 12              jne         000000014015D63F
  000000014015D62D: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  000000014015D632: 48 8B 12           mov         rdx,qword ptr [rdx]
  000000014015D635: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014015D63A: E8 11 80 EC FF     call        0000000140025650
  000000014015D63F: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014015D644: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000014015D649: 48 89 01           mov         qword ptr [rcx],rax
  000000014015D64C: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014015D651: 48 8B 00           mov         rax,qword ptr [rax]
  000000014015D654: 8B 40 10           mov         eax,dword ptr [rax+10h]
  000000014015D657: 83 E0 01           and         eax,1
  000000014015D65A: 85 C0              test        eax,eax
  000000014015D65C: 75 0A              jne         000000014015D668
  000000014015D65E: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014015D663: E8 C8 FB FF FF     call        000000014015D230
  000000014015D668: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014015D66D: 48 83 C4 38        add         rsp,38h
  000000014015D671: C3                 ret

  Summary

     1E02000 .text
