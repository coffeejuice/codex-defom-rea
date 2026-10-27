Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140088AC0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140088AC5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140088ACA: 48 83 EC 38        sub         rsp,38h
  0000000140088ACE: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140088AD3: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  0000000140088AD8: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140088ADB: 48 39 01           cmp         qword ptr [rcx],rax
  0000000140088ADE: 74 68              je          0000000140088B48
  0000000140088AE0: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  0000000140088AE5: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140088AE8: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140088AED: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140088AF2: FF 15 00 B8 D7 01  call        qword ptr [0000000141E042F8h]
  0000000140088AF8: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140088AFD: 48 8B 09           mov         rcx,qword ptr [rcx]
  0000000140088B00: FF 15 E2 B7 D7 01  call        qword ptr [0000000141E042E8h]
  0000000140088B06: 0F B6 C0           movzx       eax,al
  0000000140088B09: 85 C0              test        eax,eax
  0000000140088B0B: 75 12              jne         0000000140088B1F
  0000000140088B0D: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  0000000140088B12: 48 8B 12           mov         rdx,qword ptr [rdx]
  0000000140088B15: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140088B1A: E8 71 C3 F7 FF     call        0000000140004E90
  0000000140088B1F: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140088B24: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140088B29: 48 89 01           mov         qword ptr [rcx],rax
  0000000140088B2C: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140088B31: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140088B34: 8B 40 10           mov         eax,dword ptr [rax+10h]
  0000000140088B37: 83 E0 01           and         eax,1
  0000000140088B3A: 85 C0              test        eax,eax
  0000000140088B3C: 75 0A              jne         0000000140088B48
  0000000140088B3E: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140088B43: E8 18 C1 F7 FF     call        0000000140004C60
  0000000140088B48: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140088B4D: 48 83 C4 38        add         rsp,38h
  0000000140088B51: C3                 ret

  Summary

     1E02000 .text
