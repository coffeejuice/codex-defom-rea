Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140032600: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140032605: 48 83 EC 28        sub         rsp,28h
  0000000140032609: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014003260E: 83 B8 70 02 00 00  cmp         dword ptr [rax+0000000000000270h],0
                    00
  0000000140032615: 74 7C              je          0000000140032693
  0000000140032617: 33 D2              xor         edx,edx
  0000000140032619: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014003261E: 48 8B 89 68 02 00  mov         rcx,qword ptr [rcx+0000000000000268h]
                    00
  0000000140032625: FF 15 3D 95 DD 01  call        qword ptr [0000000141E0BB68h]
  000000014003262B: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140032630: 83 B8 70 02 00 00  cmp         dword ptr [rax+0000000000000270h],2
                    02
  0000000140032637: 75 40              jne         0000000140032679
  0000000140032639: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014003263E: 48 8B 80 90 02 00  mov         rax,qword ptr [rax+0000000000000290h]
                    00
  0000000140032645: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014003264A: 48 8B 89 90 02 00  mov         rcx,qword ptr [rcx+0000000000000290h]
                    00
  0000000140032651: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140032654: 33 D2              xor         edx,edx
  0000000140032656: FF 50 58           call        qword ptr [rax+58h]
  0000000140032659: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014003265E: 48 8B 80 98 02 00  mov         rax,qword ptr [rax+0000000000000298h]
                    00
  0000000140032665: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014003266A: 48 8B 89 98 02 00  mov         rcx,qword ptr [rcx+0000000000000298h]
                    00
  0000000140032671: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140032674: 33 D2              xor         edx,edx
  0000000140032676: FF 50 58           call        qword ptr [rax+58h]
  0000000140032679: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014003267E: E8 FD 3E 00 00     call        0000000140036580
  0000000140032683: 4C 8B 5C 24 30     mov         r11,qword ptr [rsp+30h]
  0000000140032688: 41 C7 83 70 02 00  mov         dword ptr [r11+0000000000000270h],0
                    00 00 00 00 00
  0000000140032693: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140032698: 48 8B 00           mov         rax,qword ptr [rax]
  000000014003269B: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001400326A0: FF 90 A0 02 00 00  call        qword ptr [rax+00000000000002A0h]
  00000001400326A6: BA 01 00 00 00     mov         edx,1
  00000001400326AB: 48 8B C8           mov         rcx,rax
  00000001400326AE: FF 15 64 91 DD 01  call        qword ptr [0000000141E0B818h]
  00000001400326B4: 48 83 C4 28        add         rsp,28h
  00000001400326B8: C3                 ret

  Summary

     1E02000 .text
