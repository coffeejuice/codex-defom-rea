Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067134D90: 48 85 D2           test        rdx,rdx
  0000000067134D93: 0F 84 D9 02 00 00  je          0000000067135072
  0000000067134D99: 48 8B C4           mov         rax,rsp
  0000000067134D9C: 57                 push        rdi
  0000000067134D9D: 48 81 EC 30 01 00  sub         rsp,130h
                    00
  0000000067134DA4: 48 C7 84 24 10 01  mov         qword ptr [rsp+0000000000000110h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  0000000067134DB0: 48 89 58 08        mov         qword ptr [rax+8],rbx
  0000000067134DB4: 48 89 70 18        mov         qword ptr [rax+18h],rsi
  0000000067134DB8: 49 8B D8           mov         rbx,r8
  0000000067134DBB: 48 8B F2           mov         rsi,rdx
  0000000067134DBE: 8B F9              mov         edi,ecx
  0000000067134DC0: 48 85 DB           test        rbx,rbx
  0000000067134DC3: 0F 84 95 02 00 00  je          000000006713505E
  0000000067134DC9: 85 C9              test        ecx,ecx
  0000000067134DCB: 0F 85 65 02 00 00  jne         0000000067135036
  0000000067134DD1: 8D 51 28           lea         edx,[rcx+28h]
  0000000067134DD4: 48 8B CB           mov         rcx,rbx
  0000000067134DD7: FF 15 1B 39 07 00  call        qword ptr [00000000671A86F8h]
  0000000067134DDD: 48 85 C0           test        rax,rax
  0000000067134DE0: 0F 84 42 02 00 00  je          0000000067135028
  0000000067134DE6: 0F B6 0B           movzx       ecx,byte ptr [rbx]
  0000000067134DE9: 80 F9 30           cmp         cl,30h
  0000000067134DEC: 0F 8C 36 02 00 00  jl          0000000067135028
  0000000067134DF2: 80 F9 33           cmp         cl,33h
  0000000067134DF5: 0F 8F 2D 02 00 00  jg          0000000067135028
  0000000067134DFB: 2B C3              sub         eax,ebx
  0000000067134DFD: 48 8D 53 01        lea         rdx,[rbx+1]
  0000000067134E01: 44 8D 40 FF        lea         r8d,[rax-1]
  0000000067134E05: 48 8D 8C 24 48 01  lea         rcx,[rsp+0000000000000148h]
                    00 00
  0000000067134E0D: E8 2E 87 EF FF     call        000000006702D540
  0000000067134E12: 90                 nop
  0000000067134E13: 33 C9              xor         ecx,ecx
  0000000067134E15: 48 89 8C 24 F0 00  mov         qword ptr [rsp+00000000000000F0h],rcx
                    00 00
  0000000067134E1D: 48 89 8C 24 F8 00  mov         qword ptr [rsp+00000000000000F8h],rcx
                    00 00
  0000000067134E25: 48 89 8C 24 E0 00  mov         qword ptr [rsp+00000000000000E0h],rcx
                    00 00
  0000000067134E2D: 48 89 8C 24 E8 00  mov         qword ptr [rsp+00000000000000E8h],rcx
                    00 00
  0000000067134E35: 48 89 8C 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],rcx
                    00 00
  0000000067134E3D: 48 89 8C 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],rcx
                    00 00
  0000000067134E45: 48 89 8C 24 D0 00  mov         qword ptr [rsp+00000000000000D0h],rcx
                    00 00
  0000000067134E4D: 48 89 8C 24 D8 00  mov         qword ptr [rsp+00000000000000D8h],rcx
                    00 00
  0000000067134E55: 48 89 8C 24 80 00  mov         qword ptr [rsp+0000000000000080h],rcx
                    00 00
  0000000067134E5D: 48 89 8C 24 88 00  mov         qword ptr [rsp+0000000000000088h],rcx
                    00 00
  0000000067134E65: 48 89 8C 24 90 00  mov         qword ptr [rsp+0000000000000090h],rcx
                    00 00
  0000000067134E6D: 48 89 8C 24 98 00  mov         qword ptr [rsp+0000000000000098h],rcx
                    00 00
  0000000067134E75: 48 89 8C 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],rcx
                    00 00
  0000000067134E7D: 48 89 8C 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],rcx
                    00 00
  0000000067134E85: 48 89 8C 24 C0 00  mov         qword ptr [rsp+00000000000000C0h],rcx
                    00 00
  0000000067134E8D: 48 89 8C 24 C8 00  mov         qword ptr [rsp+00000000000000C8h],rcx
                    00 00
  0000000067134E95: 48 89 8C 24 00 01  mov         qword ptr [rsp+0000000000000100h],rcx
                    00 00
  0000000067134E9D: 48 89 8C 24 08 01  mov         qword ptr [rsp+0000000000000108h],rcx
                    00 00
  0000000067134EA5: 48 89 4C 24 70     mov         qword ptr [rsp+70h],rcx
  0000000067134EAA: 48 89 4C 24 78     mov         qword ptr [rsp+78h],rcx
  0000000067134EAF: 48 8B 84 24 48 01  mov         rax,qword ptr [rsp+0000000000000148h]
                    00 00
  0000000067134EB7: 0F 28 84 24 F0 00  movaps      xmm0,xmmword ptr [rsp+00000000000000F0h]
                    00 00
  0000000067134EBF: 66 0F 7F 84 24 F0  movdqa      xmmword ptr [rsp+00000000000000F0h],xmm0
                    00 00 00
  0000000067134EC8: 0F 28 8C 24 E0 00  movaps      xmm1,xmmword ptr [rsp+00000000000000E0h]
                    00 00
  0000000067134ED0: 66 0F 7F 8C 24 E0  movdqa      xmmword ptr [rsp+00000000000000E0h],xmm1
                    00 00 00
  0000000067134ED9: 0F 28 84 24 B0 00  movaps      xmm0,xmmword ptr [rsp+00000000000000B0h]
                    00 00
  0000000067134EE1: 66 0F 7F 84 24 B0  movdqa      xmmword ptr [rsp+00000000000000B0h],xmm0
                    00 00 00
  0000000067134EEA: 0F 28 8C 24 D0 00  movaps      xmm1,xmmword ptr [rsp+00000000000000D0h]
                    00 00
  0000000067134EF2: 66 0F 7F 8C 24 D0  movdqa      xmmword ptr [rsp+00000000000000D0h],xmm1
                    00 00 00
  0000000067134EFB: 0F 28 84 24 80 00  movaps      xmm0,xmmword ptr [rsp+0000000000000080h]
                    00 00
  0000000067134F03: 66 0F 7F 84 24 80  movdqa      xmmword ptr [rsp+0000000000000080h],xmm0
                    00 00 00
  0000000067134F0C: 0F 28 8C 24 90 00  movaps      xmm1,xmmword ptr [rsp+0000000000000090h]
                    00 00
  0000000067134F14: 66 0F 7F 8C 24 90  movdqa      xmmword ptr [rsp+0000000000000090h],xmm1
                    00 00 00
  0000000067134F1D: 0F 28 84 24 A0 00  movaps      xmm0,xmmword ptr [rsp+00000000000000A0h]
                    00 00
  0000000067134F25: 66 0F 7F 84 24 A0  movdqa      xmmword ptr [rsp+00000000000000A0h],xmm0
                    00 00 00
  0000000067134F2E: 0F 28 8C 24 C0 00  movaps      xmm1,xmmword ptr [rsp+00000000000000C0h]
                    00 00
  0000000067134F36: 66 0F 7F 8C 24 C0  movdqa      xmmword ptr [rsp+00000000000000C0h],xmm1
                    00 00 00
  0000000067134F3F: 0F 28 84 24 00 01  movaps      xmm0,xmmword ptr [rsp+0000000000000100h]
                    00 00
  0000000067134F47: 66 0F 7F 84 24 00  movdqa      xmmword ptr [rsp+0000000000000100h],xmm0
                    01 00 00
  0000000067134F50: 0F 28 4C 24 70     movaps      xmm1,xmmword ptr [rsp+70h]
  0000000067134F55: 66 0F 7F 8C 24 20  movdqa      xmmword ptr [rsp+0000000000000120h],xmm1
                    01 00 00
  0000000067134F5E: 48 89 4C 24 70     mov         qword ptr [rsp+70h],rcx
  0000000067134F63: 48 89 4C 24 78     mov         qword ptr [rsp+78h],rcx
  0000000067134F68: 0F 28 44 24 70     movaps      xmm0,xmmword ptr [rsp+70h]
  0000000067134F6D: 66 0F 7F 44 24 70  movdqa      xmmword ptr [rsp+70h],xmm0
  0000000067134F73: 48 8D 8C 24 F0 00  lea         rcx,[rsp+00000000000000F0h]
                    00 00
  0000000067134F7B: 48 89 4C 24 68     mov         qword ptr [rsp+68h],rcx
  0000000067134F80: 48 8D 8C 24 E0 00  lea         rcx,[rsp+00000000000000E0h]
                    00 00
  0000000067134F88: 48 89 4C 24 60     mov         qword ptr [rsp+60h],rcx
  0000000067134F8D: 48 8D 8C 24 B0 00  lea         rcx,[rsp+00000000000000B0h]
                    00 00
  0000000067134F95: 48 89 4C 24 58     mov         qword ptr [rsp+58h],rcx
  0000000067134F9A: 48 8D 8C 24 D0 00  lea         rcx,[rsp+00000000000000D0h]
                    00 00
  0000000067134FA2: 48 89 4C 24 50     mov         qword ptr [rsp+50h],rcx
  0000000067134FA7: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  0000000067134FAF: 48 89 4C 24 48     mov         qword ptr [rsp+48h],rcx
  0000000067134FB4: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  0000000067134FBC: 48 89 4C 24 40     mov         qword ptr [rsp+40h],rcx
  0000000067134FC1: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  0000000067134FC9: 48 89 4C 24 38     mov         qword ptr [rsp+38h],rcx
  0000000067134FCE: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  0000000067134FD6: 48 89 4C 24 30     mov         qword ptr [rsp+30h],rcx
  0000000067134FDB: 48 8D 8C 24 00 01  lea         rcx,[rsp+0000000000000100h]
                    00 00
  0000000067134FE3: 48 89 4C 24 28     mov         qword ptr [rsp+28h],rcx
  0000000067134FE8: 48 8D 8C 24 20 01  lea         rcx,[rsp+0000000000000120h]
                    00 00
  0000000067134FF0: 48 89 4C 24 20     mov         qword ptr [rsp+20h],rcx
  0000000067134FF5: 4C 8D 4C 24 70     lea         r9,[rsp+70h]
  0000000067134FFA: 44 8D 47 02        lea         r8d,[rdi+2]
  0000000067134FFE: 48 8B 50 10        mov         rdx,qword ptr [rax+10h]
  0000000067135002: 48 8B CE           mov         rcx,rsi
  0000000067135005: E8 D6 CD FE FF     call        0000000067121DE0
  000000006713500A: 90                 nop
  000000006713500B: 48 8B 84 24 48 01  mov         rax,qword ptr [rsp+0000000000000148h]
                    00 00
  0000000067135013: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  0000000067135017: 75 45              jne         000000006713505E
  0000000067135019: 48 8B 8C 24 48 01  mov         rcx,qword ptr [rsp+0000000000000148h]
                    00 00
  0000000067135021: E8 8A B3 EE FF     call        00000000670203B0
  0000000067135026: EB 36              jmp         000000006713505E
  0000000067135028: 48 8D 0D B9 8B 10  lea         rcx,[000000006723DBE8h]
                    00
  000000006713502F: E8 AC 7E EE FF     call        000000006701CEE0
  0000000067135034: EB 28              jmp         000000006713505E
  0000000067135036: B9 18 00 00 00     mov         ecx,18h
  000000006713503B: E8 46 3A 04 00     call        0000000067178A86
  0000000067135040: 48 89 84 24 48 01  mov         qword ptr [rsp+0000000000000148h],rax
                    00 00
  0000000067135048: 48 85 C0           test        rax,rax
  000000006713504B: 74 11              je          000000006713505E
  000000006713504D: 4C 8B CB           mov         r9,rbx
  0000000067135050: 4C 8B C6           mov         r8,rsi
  0000000067135053: 8B D7              mov         edx,edi
  0000000067135055: 48 8B C8           mov         rcx,rax
  0000000067135058: E8 13 F8 FF FF     call        0000000067134870
  000000006713505D: 90                 nop
  000000006713505E: 4C 8D 9C 24 30 01  lea         r11,[rsp+0000000000000130h]
                    00 00
  0000000067135066: 49 8B 5B 10        mov         rbx,qword ptr [r11+10h]
  000000006713506A: 49 8B 73 20        mov         rsi,qword ptr [r11+20h]
  000000006713506E: 49 8B E3           mov         rsp,r11
  0000000067135071: 5F                 pop         rdi
  0000000067135072: F3 C3              rep ret

  Summary

      1A7000 .text
