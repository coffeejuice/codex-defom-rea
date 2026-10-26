Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file analysis\resume48-distortion-vt198-rea-session\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000141364140: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000141364145: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014136414A: 48 81 EC 98 00 00  sub         rsp,98h
                    00
  0000000141364151: 48 C7 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  000000014136415D: C7 84 24 84 00 00  mov         dword ptr [rsp+0000000000000084h],0
                    00 00 00 00 00
  0000000141364168: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  000000014136416D: E8 6E B8 E2 FE     call        000000014018F9E0
  0000000141364172: 90                 nop
  0000000141364173: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  0000000141364178: E8 23 16 D0 FE     call        00000001400657A0
  000000014136417D: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000141364185: E8 26 B1 28 FF     call        00000001405EF2B0
  000000014136418A: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  000000014136418F: 48 8B 54 24 38     mov         rdx,qword ptr [rsp+38h]
  0000000141364194: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014136419C: E8 7F B1 28 FF     call        00000001405EF320
  00000001413641A1: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001413641A6: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  00000001413641AE: 4C 8D 0D FB 9F 68  lea         r9,[00000001439EE1B0h]
                    02
  00000001413641B5: 4C 8D 05 A4 59 21  lea         r8,[0000000143579B60h]
                    02
  00000001413641BC: 33 D2              xor         edx,edx
  00000001413641BE: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001413641C3: E8 34 B7 10 00     call        000000014146F8FC
  00000001413641C8: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  00000001413641CD: C7 44 24 74 00 00  mov         dword ptr [rsp+74h],0
                    00 00
  00000001413641D5: 48 83 7C 24 78 00  cmp         qword ptr [rsp+78h],0
  00000001413641DB: 74 16              je          00000001413641F3
  00000001413641DD: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  00000001413641E2: E8 C9 84 E6 FE     call        00000001401CC6B0
  00000001413641E7: 48 8B C8           mov         rcx,rax
  00000001413641EA: E8 31 BD 0C 00     call        000000014142FF20
  00000001413641EF: 89 44 24 74        mov         dword ptr [rsp+74h],eax
  00000001413641F3: C7 44 24 70 00 00  mov         dword ptr [rsp+70h],0
                    00 00
  00000001413641FB: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  0000000141364200: E8 AB 84 E6 FE     call        00000001401CC6B0
  0000000141364205: 48 8B C8           mov         rcx,rax
  0000000141364208: E8 A3 0F 00 00     call        00000001413651B0
  000000014136420D: 89 44 24 30        mov         dword ptr [rsp+30h],eax
  0000000141364211: 83 7C 24 30 00     cmp         dword ptr [rsp+30h],0
  0000000141364216: 7E 6E              jle         0000000141364286
  0000000141364218: C7 84 24 80 00 00  mov         dword ptr [rsp+0000000000000080h],0
                    00 00 00 00 00
  0000000141364223: EB 11              jmp         0000000141364236
  0000000141364225: 8B 84 24 80 00 00  mov         eax,dword ptr [rsp+0000000000000080h]
                    00
  000000014136422C: 83 C0 01           add         eax,1
  000000014136422F: 89 84 24 80 00 00  mov         dword ptr [rsp+0000000000000080h],eax
                    00
  0000000141364236: 8B 44 24 74        mov         eax,dword ptr [rsp+74h]
  000000014136423A: 39 84 24 80 00 00  cmp         dword ptr [rsp+0000000000000080h],eax
                    00
  0000000141364241: 7D 43              jge         0000000141364286
  0000000141364243: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  0000000141364248: E8 63 84 E6 FE     call        00000001401CC6B0
  000000014136424D: 8B 94 24 80 00 00  mov         edx,dword ptr [rsp+0000000000000080h]
                    00
  0000000141364254: 48 8B C8           mov         rcx,rax
  0000000141364257: E8 B4 0F 00 00     call        0000000141365210
  000000014136425C: 89 44 24 70        mov         dword ptr [rsp+70h],eax
  0000000141364260: 8B 44 24 70        mov         eax,dword ptr [rsp+70h]
  0000000141364264: 89 44 24 48        mov         dword ptr [rsp+48h],eax
  0000000141364268: 8B 44 24 30        mov         eax,dword ptr [rsp+30h]
  000000014136426C: 89 44 24 4C        mov         dword ptr [rsp+4Ch],eax
  0000000141364270: C6 44 24 60 01     mov         byte ptr [rsp+60h],1
  0000000141364275: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  000000014136427A: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  000000014136427F: E8 2C 91 D4 FE     call        00000001400AD3B0
  0000000141364284: EB 9F              jmp         0000000141364225
  0000000141364286: 48 8D 54 24 68     lea         rdx,[rsp+68h]
  000000014136428B: 48 8B 8C 24 A8 00  mov         rcx,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000141364293: E8 F8 30 D0 FE     call        0000000140067390
  0000000141364298: 8B 84 24 84 00 00  mov         eax,dword ptr [rsp+0000000000000084h]
                    00
  000000014136429F: 83 C8 01           or          eax,1
  00000001413642A2: 89 84 24 84 00 00  mov         dword ptr [rsp+0000000000000084h],eax
                    00
  00000001413642A9: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000001413642AE: E8 7D BD 27 FF     call        00000001405E0030
  00000001413642B3: 48 8B 84 24 A8 00  mov         rax,qword ptr [rsp+00000000000000A8h]
                    00 00
  00000001413642BB: 48 81 C4 98 00 00  add         rsp,98h
                    00
  00000001413642C2: C3                 ret
  00000001413642C3: CC                 int         3

  Summary

     1E02000 .text
