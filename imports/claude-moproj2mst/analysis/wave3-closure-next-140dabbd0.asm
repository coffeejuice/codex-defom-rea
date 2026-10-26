Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140DABBD0: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  0000000140DABBD5: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140DABBD9: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140DABBDE: 48 83 EC 58        sub         rsp,58h
  0000000140DABBE2: 8B 44 24 68        mov         eax,dword ptr [rsp+68h]
  0000000140DABBE6: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  0000000140DABBEA: 83 7C 24 28 00     cmp         dword ptr [rsp+28h],0
  0000000140DABBEF: 7C 11              jl          0000000140DABC02
  0000000140DABBF1: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140DABBF6: FF 15 04 E6 05 01  call        qword ptr [0000000141E0A200h]
  0000000140DABBFC: 39 44 24 28        cmp         dword ptr [rsp+28h],eax
  0000000140DABC00: 7C 08              jl          0000000140DABC0A
  0000000140DABC02: C7 44 24 28 00 00  mov         dword ptr [rsp+28h],0
                    00 00
  0000000140DABC0A: 8B 54 24 28        mov         edx,dword ptr [rsp+28h]
  0000000140DABC0E: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140DABC13: FF 15 DF E5 05 01  call        qword ptr [0000000141E0A1F8h]
  0000000140DABC19: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140DABC1E: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  0000000140DABC23: 48 39 44 24 20     cmp         qword ptr [rsp+20h],rax
  0000000140DABC28: 75 02              jne         0000000140DABC2C
  0000000140DABC2A: EB 70              jmp         0000000140DABC9C
  0000000140DABC2C: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140DABC32: 74 4C              je          0000000140DABC80
  0000000140DABC34: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140DABC39: FF 15 71 FF 05 01  call        qword ptr [0000000141E0BBB0h]
  0000000140DABC3F: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140DABC44: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140DABC49: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140DABC4E: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140DABC53: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  0000000140DABC59: 74 1C              je          0000000140DABC77
  0000000140DABC5B: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140DABC60: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140DABC63: BA 01 00 00 00     mov         edx,1
  0000000140DABC68: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140DABC6D: FF 50 18           call        qword ptr [rax+18h]
  0000000140DABC70: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140DABC75: EB 09              jmp         0000000140DABC80
  0000000140DABC77: 48 C7 44 24 40 00  mov         qword ptr [rsp+40h],0
                    00 00 00
  0000000140DABC80: 48 83 7C 24 70 00  cmp         qword ptr [rsp+70h],0
  0000000140DABC86: 74 14              je          0000000140DABC9C
  0000000140DABC88: 4C 8B 44 24 70     mov         r8,qword ptr [rsp+70h]
  0000000140DABC8D: 8B 54 24 28        mov         edx,dword ptr [rsp+28h]
  0000000140DABC91: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140DABC96: FF 15 54 E5 05 01  call        qword ptr [0000000141E0A1F0h]
  0000000140DABC9C: 48 83 C4 58        add         rsp,58h
  0000000140DABCA0: C3                 ret

  Summary

     1E02000 .text
