Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140C35580: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  0000000140C35585: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  0000000140C3558A: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140C3558F: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C35594: 48 83 EC 38        sub         rsp,38h
  0000000140C35598: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140C355A0: 83 7C 24 50 0E     cmp         dword ptr [rsp+50h],0Eh
  0000000140C355A5: 75 3E              jne         0000000140C355E5
  0000000140C355A7: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140C355AC: 48 83 E8 10        sub         rax,10h
  0000000140C355B0: 48 85 C0           test        rax,rax
  0000000140C355B3: 74 14              je          0000000140C355C9
  0000000140C355B5: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140C355BA: 48 83 E8 10        sub         rax,10h
  0000000140C355BE: 48 83 C0 10        add         rax,10h
  0000000140C355C2: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140C355C7: EB 09              jmp         0000000140C355D2
  0000000140C355C9: 48 C7 44 24 28 00  mov         qword ptr [rsp+28h],0
                    00 00 00
  0000000140C355D2: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140C355D7: 48 83 E9 10        sub         rcx,10h
  0000000140C355DB: 48 8B 54 24 28     mov         rdx,qword ptr [rsp+28h]
  0000000140C355E0: E8 BB 40 01 00     call        0000000140C496A0
  0000000140C355E5: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  0000000140C355EA: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  0000000140C355EF: FF 15 BB 03 1D 01  call        qword ptr [0000000141E059B0h]
  0000000140C355F5: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  0000000140C355F9: 83 C8 01           or          eax,1
  0000000140C355FC: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  0000000140C35600: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  0000000140C35605: 48 83 C4 38        add         rsp,38h
  0000000140C35609: C3                 ret

  Summary

     1E02000 .text
