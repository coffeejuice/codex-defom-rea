Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140CFA040: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140CFA045: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140CFA04A: 48 83 EC 48        sub         rsp,48h
  0000000140CFA04E: 48 C7 44 24 38 FE  mov         qword ptr [rsp+38h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140CFA057: 48 C7 44 24 20 00  mov         qword ptr [rsp+20h],0
                    00 00 00
  0000000140CFA060: 48 C7 44 24 28 00  mov         qword ptr [rsp+28h],0
                    00 00 00
  0000000140CFA069: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140CFA06E: 48 81 C1 D8 01 00  add         rcx,1D8h
                    00
  0000000140CFA075: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  0000000140CFA07A: E8 A1 68 37 FF     call        0000000140070920
  0000000140CFA07F: 0F B6 C0           movzx       eax,al
  0000000140CFA082: 85 C0              test        eax,eax
  0000000140CFA084: 74 2A              je          0000000140CFA0B0
  0000000140CFA086: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140CFA08B: 48 81 C1 D8 01 00  add         rcx,1D8h
                    00
  0000000140CFA092: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  0000000140CFA097: E8 C4 B1 3B FF     call        00000001400B5260
  0000000140CFA09C: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140CFA0A1: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140CFA0A6: E8 55 81 01 00     call        0000000140D12200
  0000000140CFA0AB: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140CFA0B0: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140CFA0B5: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140CFA0BA: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000140CFA0BF: FF 15 1B B9 10 01  call        qword ptr [0000000141E059E0h]
  0000000140CFA0C5: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140CFA0CA: 48 83 C4 48        add         rsp,48h
  0000000140CFA0CE: C3                 ret

  Summary

     1E02000 .text
