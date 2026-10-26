Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000141762310: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000141762315: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014176231A: 48 83 EC 48        sub         rsp,48h
  000000014176231E: 33 C0              xor         eax,eax
  0000000141762320: 83 F8 01           cmp         eax,1
  0000000141762323: 74 69              je          000000014176238E
  0000000141762325: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  000000014176232A: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014176232F: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000141762334: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  0000000141762338: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000014176233D: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  0000000141762342: 48 39 44 24 28     cmp         qword ptr [rsp+28h],rax
  0000000141762347: 74 45              je          000000014176238E
  0000000141762349: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  000000014176234E: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000141762353: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000141762358: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  000000014176235C: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000141762361: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000141762366: E8 45 2E C6 FF     call        00000001413C51B0
  000000014176236B: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000141762370: 33 D2              xor         edx,edx
  0000000141762372: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000141762377: E8 04 0C 8A FE     call        0000000140002F80
  000000014176237C: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000141762381: 48 83 C1 08        add         rcx,8
  0000000141762385: 33 D2              xor         edx,edx
  0000000141762387: E8 04 54 AE FF     call        0000000141247790
  000000014176238C: EB AF              jmp         000000014176233D
  000000014176238E: E8 DD FC FF FF     call        0000000141762070
  0000000141762393: 8B D0              mov         edx,eax
  0000000141762395: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  000000014176239A: FF 15 40 1F 6A 00  call        qword ptr [0000000141E042E0h]
  00000001417623A0: 48 83 C4 48        add         rsp,48h
  00000001417623A4: C3                 ret

  Summary

     1E02000 .text
