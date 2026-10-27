Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001405E0E90: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001405E0E95: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001405E0E9A: 48 83 EC 38        sub         rsp,38h
  00000001405E0E9E: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001405E0EA3: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  00000001405E0EA8: 48 8B 00           mov         rax,qword ptr [rax]
  00000001405E0EAB: 48 39 01           cmp         qword ptr [rcx],rax
  00000001405E0EAE: 74 68              je          00000001405E0F18
  00000001405E0EB0: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  00000001405E0EB5: 48 8B 00           mov         rax,qword ptr [rax]
  00000001405E0EB8: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001405E0EBD: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001405E0EC2: FF 15 30 34 82 01  call        qword ptr [0000000141E042F8h]
  00000001405E0EC8: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001405E0ECD: 48 8B 09           mov         rcx,qword ptr [rcx]
  00000001405E0ED0: FF 15 12 34 82 01  call        qword ptr [0000000141E042E8h]
  00000001405E0ED6: 0F B6 C0           movzx       eax,al
  00000001405E0ED9: 85 C0              test        eax,eax
  00000001405E0EDB: 75 12              jne         00000001405E0EEF
  00000001405E0EDD: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  00000001405E0EE2: 48 8B 12           mov         rdx,qword ptr [rdx]
  00000001405E0EE5: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001405E0EEA: E8 61 47 A4 FF     call        0000000140025650
  00000001405E0EEF: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001405E0EF4: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000001405E0EF9: 48 89 01           mov         qword ptr [rcx],rax
  00000001405E0EFC: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001405E0F01: 48 8B 00           mov         rax,qword ptr [rax]
  00000001405E0F04: 8B 40 10           mov         eax,dword ptr [rax+10h]
  00000001405E0F07: 83 E0 01           and         eax,1
  00000001405E0F0A: 85 C0              test        eax,eax
  00000001405E0F0C: 75 0A              jne         00000001405E0F18
  00000001405E0F0E: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001405E0F13: E8 88 FC B5 FF     call        0000000140140BA0
  00000001405E0F18: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001405E0F1D: 48 83 C4 38        add         rsp,38h
  00000001405E0F21: C3                 ret

  Summary

     1E02000 .text
