Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D40B40: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140D40B45: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D40B4A: 48 83 EC 58        sub         rsp,58h
  0000000140D40B4E: 48 C7 44 24 38 FE  mov         qword ptr [rsp+38h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140D40B57: 48 83 7C 24 68 00  cmp         qword ptr [rsp+68h],0
  0000000140D40B5D: 75 10              jne         0000000140D40B6F
  0000000140D40B5F: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140D40B64: FF 15 D6 AA 0C 01  call        qword ptr [0000000141E0B640h]
  0000000140D40B6A: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  0000000140D40B6F: B9 30 00 00 00     mov         ecx,30h
  0000000140D40B74: E8 43 DF 72 00     call        000000014146EABC
  0000000140D40B79: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140D40B7E: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  0000000140D40B84: 74 11              je          0000000140D40B97
  0000000140D40B86: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140D40B8B: E8 80 7F 09 00     call        0000000140DD8B10
  0000000140D40B90: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140D40B95: EB 09              jmp         0000000140D40BA0
  0000000140D40B97: 48 C7 44 24 40 00  mov         qword ptr [rsp+40h],0
                    00 00 00
  0000000140D40BA0: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140D40BA5: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140D40BAA: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140D40BAF: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140D40BB4: 48 8B 54 24 20     mov         rdx,qword ptr [rsp+20h]
  0000000140D40BB9: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  0000000140D40BBE: FF 15 F4 A9 0C 01  call        qword ptr [0000000141E0B5B8h]
  0000000140D40BC4: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140D40BC9: 48 83 C4 58        add         rsp,58h
  0000000140D40BCD: C3                 ret

  Summary

     1E02000 .text
