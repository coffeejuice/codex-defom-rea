Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001402D03F0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001402D03F5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001402D03FA: 48 81 EC C8 00 00  sub         rsp,0C8h
                    00
  00000001402D0401: 48 C7 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  00000001402D040D: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  00000001402D0415: E8 16 03 00 00     call        00000001402D0730
  00000001402D041A: 4C 8B 84 24 D8 00  mov         r8,qword ptr [rsp+00000000000000D8h]
                    00 00
  00000001402D0422: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  00000001402D0427: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  00000001402D042F: E8 BC 06 DA FF     call        0000000140070AF0
  00000001402D0434: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  00000001402D043C: 48 8B 84 24 D0 00  mov         rax,qword ptr [rsp+00000000000000D0h]
                    00 00
  00000001402D0444: 48 8B 00           mov         rax,qword ptr [rax]
  00000001402D0447: 48 39 84 24 90 00  cmp         qword ptr [rsp+0000000000000090h],rax
                    00 00
  00000001402D044F: 75 6C              jne         00000001402D04BD
  00000001402D0451: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  00000001402D0459: E8 82 F5 EB FF     call        000000014018F9E0
  00000001402D045E: 48 89 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],rax
                    00 00
  00000001402D0466: 48 8B 84 24 A8 00  mov         rax,qword ptr [rsp+00000000000000A8h]
                    00 00
  00000001402D046E: 48 89 84 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],rax
                    00 00
  00000001402D0476: 48 8B 84 24 B0 00  mov         rax,qword ptr [rsp+00000000000000B0h]
                    00 00
  00000001402D047E: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001402D0483: 4C 8B 8C 24 D8 00  mov         r9,qword ptr [rsp+00000000000000D8h]
                    00 00
  00000001402D048B: 4C 8D 44 24 30     lea         r8,[rsp+30h]
  00000001402D0490: 48 8B 94 24 D0 00  mov         rdx,qword ptr [rsp+00000000000000D0h]
                    00 00
  00000001402D0498: 48 8B 12           mov         rdx,qword ptr [rdx]
  00000001402D049B: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  00000001402D04A3: E8 18 04 00 00     call        00000001402D08C0
  00000001402D04A8: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  00000001402D04B0: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  00000001402D04B8: E8 E3 7C F3 FF     call        00000001402081A0
  00000001402D04BD: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001402D04C5: E8 96 C4 0F 01     call        00000001413CC960
  00000001402D04CA: 48 83 C0 08        add         rax,8
  00000001402D04CE: 48 81 C4 C8 00 00  add         rsp,0C8h
                    00
  00000001402D04D5: C3                 ret
  00000001402D04D6: CC                 int         3
  00000001402D04D7: CC                 int         3
  00000001402D04D8: CC                 int         3
  00000001402D04D9: CC                 int         3
  00000001402D04DA: CC                 int         3
  00000001402D04DB: CC                 int         3
  00000001402D04DC: CC                 int         3
  00000001402D04DD: CC                 int         3
  00000001402D04DE: CC                 int         3
  00000001402D04DF: CC                 int         3
  00000001402D04E0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001402D04E5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001402D04EA: 48 83 EC 48        sub         rsp,48h
  00000001402D04EE: 33 C0              xor         eax,eax
  00000001402D04F0: 83 F8 01           cmp         eax,1
  00000001402D04F3: 74 69              je          00000001402D055E
  00000001402D04F5: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  00000001402D04FA: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001402D04FF: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000001402D0504: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  00000001402D0508: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001402D050D: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  00000001402D0512: 48 39 44 24 28     cmp         qword ptr [rsp+28h],rax
  00000001402D0517: 74 45              je          00000001402D055E
  00000001402D0519: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  00000001402D051E: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001402D0523: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000001402D0528: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  00000001402D052C: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001402D0531: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001402D0536: E8 25 C4 0F 01     call        00000001413CC960
  00000001402D053B: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001402D0540: 33

  Summary

     1E02000 .text
