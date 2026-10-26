Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400251D0: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  00000001400251D5: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001400251DA: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400251DF: 48 83 EC 38        sub         rsp,38h
  00000001400251E3: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001400251E8: FF 15 F2 EF DD 01  call        qword ptr [0000000141E041E0h]
  00000001400251EE: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001400251F3: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001400251F8: FF 15 F2 EF DD 01  call        qword ptr [0000000141E041F0h]
  00000001400251FE: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140025203: 48 8B 54 24 20     mov         rdx,qword ptr [rsp+20h]
  0000000140025208: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  000000014002520D: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140025212: 48 83 E8 08        sub         rax,8
  0000000140025216: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014002521B: 48 3B D1           cmp         rdx,rcx
  000000014002521E: 74 2E              je          000000014002524E
  0000000140025220: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140025225: E8 A6 34 36 01     call        00000001413886D0
  000000014002522A: 4C 8B D8           mov         r11,rax
  000000014002522D: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140025232: 8B 00              mov         eax,dword ptr [rax]
  0000000140025234: 41 39 03           cmp         dword ptr [r11],eax
  0000000140025237: 75 13              jne         000000014002524C
  0000000140025239: B2 01              mov         dl,1
  000000014002523B: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  0000000140025240: E8 1B D6 FD FF     call        0000000140002860
  0000000140025245: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  000000014002524A: EB 13              jmp         000000014002525F
  000000014002524C: EB B5              jmp         0000000140025203
  000000014002524E: 33 D2              xor         edx,edx
  0000000140025250: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  0000000140025255: E8 06 D6 FD FF     call        0000000140002860
  000000014002525A: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  000000014002525F: 48 83 C4 38        add         rsp,38h
  0000000140025263: C3                 ret

  Summary

     1E02000 .text
