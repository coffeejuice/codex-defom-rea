Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140032350: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140032355: 48 83 EC 38        sub         rsp,38h
  0000000140032359: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014003235E: 83 B8 78 02 00 00  cmp         dword ptr [rax+0000000000000278h],2
                    02
  0000000140032365: 75 14              jne         000000014003237B
  0000000140032367: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014003236C: 48 83 B8 80 02 00  cmp         qword ptr [rax+0000000000000280h],0
                    00 00
  0000000140032374: 74 05              je          000000014003237B
  0000000140032376: E9 95 00 00 00     jmp         0000000140032410
  000000014003237B: 48 C7 44 24 20 00  mov         qword ptr [rsp+20h],0
                    00 00 00
  0000000140032384: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140032389: 83 B8 D8 03 00 00  cmp         dword ptr [rax+00000000000003D8h],3
                    03
  0000000140032390: 75 11              jne         00000001400323A3
  0000000140032392: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140032397: E8 74 67 FE FF     call        0000000140018B10
  000000014003239C: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001400323A1: EB 41              jmp         00000001400323E4
  00000001400323A3: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001400323A8: 8B 90 D8 03 00 00  mov         edx,dword ptr [rax+00000000000003D8h]
  00000001400323AE: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001400323B3: 48 8B 89 80 04 00  mov         rcx,qword ptr [rcx+0000000000000480h]
                    00
  00000001400323BA: FF 15 60 94 DD 01  call        qword ptr [0000000141E0B820h]
  00000001400323C0: 48 8B C8           mov         rcx,rax
  00000001400323C3: E8 28 D5 00 00     call        000000014003F8F0
  00000001400323C8: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001400323CD: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  00000001400323D3: 74 0F              je          00000001400323E4
  00000001400323D5: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  00000001400323DA: E8 11 D2 00 00     call        000000014003F5F0
  00000001400323DF: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001400323E4: 48 8B 54 24 20     mov         rdx,qword ptr [rsp+20h]
  00000001400323E9: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001400323EE: 48 8B 89 78 04 00  mov         rcx,qword ptr [rcx+0000000000000478h]
                    00
  00000001400323F5: E8 16 08 CC 00     call        0000000140CF2C10
  00000001400323FA: 4C 8B 5C 24 40     mov         r11,qword ptr [rsp+40h]
  00000001400323FF: 41 8B 93 D8 03 00  mov         edx,dword ptr [r11+00000000000003D8h]
                    00
  0000000140032406: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014003240B: E8 D0 66 FE FF     call        0000000140018AE0
  0000000140032410: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140032415: E8 E6 01 00 00     call        0000000140032600
  000000014003241A: 48 83 C4 38        add         rsp,38h
  000000014003241E: C3                 ret

  Summary

     1E02000 .text
