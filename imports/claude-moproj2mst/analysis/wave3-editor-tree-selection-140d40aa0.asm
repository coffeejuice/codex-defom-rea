Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D40AA0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140D40AA5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D40AAA: 48 83 EC 58        sub         rsp,58h
  0000000140D40AAE: 48 C7 44 24 38 FE  mov         qword ptr [rsp+38h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140D40AB7: 48 83 7C 24 68 00  cmp         qword ptr [rsp+68h],0
  0000000140D40ABD: 75 10              jne         0000000140D40ACF
  0000000140D40ABF: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140D40AC4: FF 15 76 AB 0C 01  call        qword ptr [0000000141E0B640h]
  0000000140D40ACA: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  0000000140D40ACF: B9 30 00 00 00     mov         ecx,30h
  0000000140D40AD4: E8 E3 DF 72 00     call        000000014146EABC
  0000000140D40AD9: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140D40ADE: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  0000000140D40AE4: 74 11              je          0000000140D40AF7
  0000000140D40AE6: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140D40AEB: E8 50 7E 09 00     call        0000000140DD8940
  0000000140D40AF0: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140D40AF5: EB 09              jmp         0000000140D40B00
  0000000140D40AF7: 48 C7 44 24 40 00  mov         qword ptr [rsp+40h],0
                    00 00 00
  0000000140D40B00: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140D40B05: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140D40B0A: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140D40B0F: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140D40B14: 48 8B 54 24 20     mov         rdx,qword ptr [rsp+20h]
  0000000140D40B19: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  0000000140D40B1E: FF 15 94 AA 0C 01  call        qword ptr [0000000141E0B5B8h]
  0000000140D40B24: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140D40B29: 48 83 C4 58        add         rsp,58h
  0000000140D40B2D: C3                 ret

  Summary

     1E02000 .text
