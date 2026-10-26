Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140896360: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  0000000140896365: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014089636A: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014089636F: 48 83 EC 58        sub         rsp,58h
  0000000140896373: C6 44 24 30 01     mov         byte ptr [rsp+30h],1
  0000000140896378: 48 C7 44 24 28 00  mov         qword ptr [rsp+28h],0
                    00 00 00
  0000000140896381: C7 44 24 34 00 00  mov         dword ptr [rsp+34h],0
                    00 00
  0000000140896389: 48 83 7C 24 68 00  cmp         qword ptr [rsp+68h],0
  000000014089638F: 74 10              je          00000001408963A1
  0000000140896391: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  0000000140896396: E8 E5 A7 BD 00     call        0000000141470B80
  000000014089639B: 48 83 F8 01        cmp         rax,1
  000000014089639F: 73 0A              jae         00000001408963AB
  00000001408963A1: B8 01 00 00 00     mov         eax,1
  00000001408963A6: E9 9F 00 00 00     jmp         000000014089644A
  00000001408963AB: 48 8D 15 D6 69 F1  lea         rdx,[00000001427ACD88h]
                    01
  00000001408963B2: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  00000001408963B7: E8 1C 99 BD 00     call        000000014146FCD8
  00000001408963BC: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001408963C1: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  00000001408963C7: 75 07              jne         00000001408963D0
  00000001408963C9: B8 01 00 00 00     mov         eax,1
  00000001408963CE: EB 7A              jmp         000000014089644A
  00000001408963D0: C7 44 24 40 00 00  mov         dword ptr [rsp+40h],0
                    00 00
  00000001408963D8: 4C 8D 44 24 40     lea         r8,[rsp+40h]
  00000001408963DD: 48 8B 54 24 28     mov         rdx,qword ptr [rsp+28h]
  00000001408963E2: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001408963E7: E8 74 F1 FF FF     call        0000000140895560
  00000001408963EC: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001408963F1: 48 81 C1 80 13 00  add         rcx,1380h
                    00
  00000001408963F8: 8B 54 24 70        mov         edx,dword ptr [rsp+70h]
  00000001408963FC: E8 0F 89 FB FF     call        000000014084ED10
  0000000140896401: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140896406: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  000000014089640C: 75 11              jne         000000014089641F
  000000014089640E: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140896413: E8 10 97 BD 00     call        000000014146FB28
  0000000140896418: B8 03 00 00 00     mov         eax,3
  000000014089641D: EB 2B              jmp         000000014089644A
  000000014089641F: 44 8B 4C 24 70     mov         r9d,dword ptr [rsp+70h]
  0000000140896424: 4C 8B 44 24 38     mov         r8,qword ptr [rsp+38h]
  0000000140896429: 48 8B 54 24 28     mov         rdx,qword ptr [rsp+28h]
  000000014089642E: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140896433: E8 F8 8A 0E 00     call        000000014097EF30
  0000000140896438: 89 44 24 34        mov         dword ptr [rsp+34h],eax
  000000014089643C: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140896441: E8 E2 96 BD 00     call        000000014146FB28
  0000000140896446: 8B 44 24 34        mov         eax,dword ptr [rsp+34h]
  000000014089644A: 48 83 C4 58        add         rsp,58h
  000000014089644E: C3                 ret

  Summary

     1E02000 .text
