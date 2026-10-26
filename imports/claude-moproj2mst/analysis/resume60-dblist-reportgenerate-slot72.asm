Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001407A0500: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001407A0505: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001407A050A: 48 81 EC 98 00 00  sub         rsp,98h
                    00
  00000001407A0511: 48 C7 44 24 68 FE  mov         qword ptr [rsp+68h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001407A051A: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001407A0522: E8 99 A2 C6 00     call        000000014140A7C0
  00000001407A0527: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  00000001407A0530: 48 C7 44 24 28 00  mov         qword ptr [rsp+28h],0
                    00 00 00
  00000001407A0539: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  00000001407A053E: 48 8B 8C 24 A8 00  mov         rcx,qword ptr [rsp+00000000000000A8h]
                    00 00
  00000001407A0546: FF 15 6C C0 66 01  call        qword ptr [0000000141E0C5B8h]
  00000001407A054C: 90                 nop
  00000001407A054D: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001407A0552: FF 15 E0 BF 66 01  call        qword ptr [0000000141E0C538h]
  00000001407A0558: 0F B6 C0           movzx       eax,al
  00000001407A055B: 85 C0              test        eax,eax
  00000001407A055D: 0F 85 31 01 00 00  jne         00000001407A0694
  00000001407A0563: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  00000001407A0568: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001407A056D: FF 15 3D C0 66 01  call        qword ptr [0000000141E0C5B0h]
  00000001407A0573: 90                 nop
  00000001407A0574: B9 60 00 00 00     mov         ecx,60h
  00000001407A0579: E8 3E E5 CC 00     call        000000014146EABC
  00000001407A057E: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001407A0583: 48 83 7C 24 48 00  cmp         qword ptr [rsp+48h],0
  00000001407A0589: 74 11              je          00000001407A059C
  00000001407A058B: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  00000001407A0590: E8 4B BF 01 00     call        00000001407BC4E0
  00000001407A0595: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  00000001407A059A: EB 09              jmp         00000001407A05A5
  00000001407A059C: 48 C7 44 24 70 00  mov         qword ptr [rsp+70h],0
                    00 00 00
  00000001407A05A5: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  00000001407A05AA: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001407A05AF: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001407A05B4: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001407A05B9: 48 8B 54 24 28     mov         rdx,qword ptr [rsp+28h]
  00000001407A05BE: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001407A05C6: E8 F5 A0 C6 00     call        000000014140A6C0
  00000001407A05CB: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001407A05D3: 48 81 C1 80 00 00  add         rcx,80h
                    00
  00000001407A05DA: E8 01 45 86 FF     call        0000000140004AE0
  00000001407A05DF: 8B D0              mov         edx,eax
  00000001407A05E1: 83 EA 01           sub         edx,1
  00000001407A05E4: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001407A05EC: 48 81 C1 80 00 00  add         rcx,80h
                    00
  00000001407A05F3: E8 B8 AF 9A FF     call        000000014014B5B0
  00000001407A05F8: 48 8B 00           mov         rax,qword ptr [rax]
  00000001407A05FB: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001407A0600: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  00000001407A0606: 74 34              je          00000001407A063C
  00000001407A0608: 48 8D 44 24 50     lea         rax,[rsp+50h]
  00000001407A060D: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  00000001407A0612: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  00000001407A0617: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  00000001407A061C: FF 15 76 C0 66 01  call        qword ptr [0000000141E0C698h]
  00000001407A0622: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  00000001407A0627: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001407A062C: 48 8B 00           mov         rax,qword ptr [rax]
  00000001407A062F: 48 8B 54 24 78     mov         rdx,qword ptr [rsp+78h]
  00000001407A0634: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001407A0639: FF 50 28           call        qword ptr [rax+28h]
  00000001407A063C: 48 8D 54 24 60     lea         rdx,[rsp+60h]
  00000001407A0641: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001407A0646: FF 15 4C BF 66 01  call        qword ptr [0000000141E0C598h]
  00000001407A064C: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  00000001407A0654: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001407A065C: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  00000001407A0664: 48 8B 94 24 88 00  mov         rdx,qword ptr [rsp+0000000000000088h]
                    00 00
  00000001407A066C: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001407A0671: FF 15 E9 BE 66 01  call        qword ptr [0000000141E0C560h]
  00000001407A0677: 90                 nop
  00000001407A0678: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  00000001407A067D: FF 15 25 C0 66 01  call        qword ptr [0000000141E0C6A8h]
  00000001407A0683: 90                 nop
  00000001407A0684: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  00000001407A0689: FF 15 21 C0 66 01  call        qword ptr [0000000141E0C6B0h]
  00000001407A068F: E9 B9 FE FF FF     jmp         00000001407A054D
  00000001407A0694: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001407A0699: FF 15 09 C0 66 01  call        qword ptr [0000000141E0C6A8h]
  00000001407A069F: 90                 nop
  00000001407A06A0: 48 8B 8C 24 A8 00  mov         rcx,qword ptr [rsp+00000000000000A8h]
                    00 00
  00000001407A06A8: FF 15 02 C0 66 01  call        qword ptr [0000000141E0C6B0h]
  00000001407A06AE: 48 81 C4 98 00 00  add         rsp,98h
                    00
  00000001407A06B5: C3                 ret
  00000001407A06B6: CC                 int         3

  Summary

     1E02000 .text
