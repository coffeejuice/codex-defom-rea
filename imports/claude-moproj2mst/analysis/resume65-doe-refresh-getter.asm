Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400196D0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400196D5: 48 83 EC 48        sub         rsp,48h
  00000001400196D9: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001400196E2: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000001400196E7: 48 83 B8 58 04 00  cmp         qword ptr [rax+0000000000000458h],0
                    00 00
  00000001400196EF: 75 4E              jne         000000014001973F
  00000001400196F1: B9 B8 00 00 00     mov         ecx,0B8h
  00000001400196F6: E8 C1 53 45 01     call        000000014146EABC
  00000001400196FB: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140019700: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  0000000140019706: 74 13              je          000000014001971B
  0000000140019708: 33 D2              xor         edx,edx
  000000014001970A: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  000000014001970F: E8 DC 1F 0D 00     call        00000001400EB6F0
  0000000140019714: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140019719: EB 09              jmp         0000000140019724
  000000014001971B: 48 C7 44 24 38 00  mov         qword ptr [rsp+38h],0
                    00 00 00
  0000000140019724: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140019729: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014001972E: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140019733: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140019738: 48 89 81 58 04 00  mov         qword ptr [rcx+0000000000000458h],rax
                    00
  000000014001973F: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140019744: 48 8B 80 58 04 00  mov         rax,qword ptr [rax+0000000000000458h]
                    00
  000000014001974B: 48 83 C4 48        add         rsp,48h
  000000014001974F: C3                 ret

  Summary

     1E02000 .text
