Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400EB6F0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001400EB6F5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400EB6FA: 57                 push        rdi
  00000001400EB6FB: 48 81 EC D0 00 00  sub         rsp,0D0h
                    00
  00000001400EB702: 48 C7 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  00000001400EB70E: 48 8B 94 24 E8 00  mov         rdx,qword ptr [rsp+00000000000000E8h]
                    00 00
  00000001400EB716: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400EB71E: E8 2D C2 28 00     call        0000000140377950
  00000001400EB723: 90                 nop
  00000001400EB724: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400EB72C: 48 8D 05 05 06 0A  lea         rax,[000000014218BD38h]
                    02
  00000001400EB733: 48 89 01           mov         qword ptr [rcx],rax
  00000001400EB736: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400EB73E: 48 8D 05 B3 05 0A  lea         rax,[000000014218BCF8h]
                    02
  00000001400EB745: 48 89 41 10        mov         qword ptr [rcx+10h],rax
  00000001400EB749: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400EB751: 48 81 C1 90 00 00  add         rcx,90h
                    00
  00000001400EB758: E8 83 9C F7 FF     call        00000001400653E0
  00000001400EB75D: 90                 nop
  00000001400EB75E: 48 C7 44 24 38 98  mov         qword ptr [rsp+38h],98h
                    00 00 00
  00000001400EB767: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  00000001400EB76C: E8 4B 33 38 01     call        000000014146EABC
  00000001400EB771: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001400EB776: 48 83 7C 24 40 00  cmp         qword ptr [rsp+40h],0
  00000001400EB77C: 74 1D              je          00000001400EB79B
  00000001400EB77E: 48 8B 7C 24 40     mov         rdi,qword ptr [rsp+40h]
  00000001400EB783: 33 C0              xor         eax,eax
  00000001400EB785: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  00000001400EB78A: F3 AA              rep stos    byte ptr [rdi]
  00000001400EB78C: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001400EB791: 48 89 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],rax
                    00 00
  00000001400EB799: EB 0C              jmp         00000001400EB7A7
  00000001400EB79B: 48 C7 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],0
                    00 00 00 00 00 00
  00000001400EB7A7: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400EB7AF: 48 8B 84 24 98 00  mov         rax,qword ptr [rsp+0000000000000098h]
                    00 00
  00000001400EB7B7: 48 89 41 38        mov         qword ptr [rcx+38h],rax
  00000001400EB7BB: 48 8B 94 24 E0 00  mov         rdx,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400EB7C3: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400EB7CB: 48 8B 49 38        mov         rcx,qword ptr [rcx+38h]
  00000001400EB7CF: E8 CC 14 01 00     call        00000001400FCCA0
  00000001400EB7D4: 48 8B 94 24 E0 00  mov         rdx,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400EB7DC: 48 C7 42 40 00 00  mov         qword ptr [rdx+40h],0
                    00 00
  00000001400EB7E4: 48 8B 84 24 E0 00  mov         rax,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400EB7EC: C7 40 5C 00 00 00  mov         dword ptr [rax+5Ch],0
                    00
  00000001400EB7F3: 48 8B 84 24 E0 00  mov         rax,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400EB7FB: C6 80 98 00 00 00  mov         byte ptr [rax+0000000000000098h],0
                    00
  00000001400EB802: B9 20 00 00 00     mov         ecx,20h
  00000001400EB807: E8 B0 32 38 01     call        000000014146EABC
  00000001400EB80C: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000001400EB811: 48 83 7C 24 50 00  cmp         qword ptr [rsp+50h],0
  00000001400EB817: 74 31              je          00000001400EB84A
  00000001400EB819: 48 8B 94 24 E0 00  mov         rdx,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400EB821: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001400EB826: FF 15 D4 88 D1 01  call        qword ptr [0000000141E04100h]
  00000001400EB82C: 48 8B 54 24 50     mov         rdx,qword ptr [rsp+50h]
  00000001400EB831: 48 8D 05 40 03 01  lea         rax,[00000001420FBB78h]
                    02
  00000001400EB838: 48 89 02           mov         qword ptr [rdx],rax
  00000001400EB83B: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000001400EB840: 48 89 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],rax
                    00 00
  00000001400EB848: EB 0C              jmp         00000001400EB856
  00000001400EB84A: 48 C7 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],0
                    00 00 00 00 00 00
  00000001400EB856: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001400EB85E: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001400EB863: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400EB86B: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  00000001400EB870: 48 89 81 B0 00 00  mov         qword ptr [rcx+00000000000000B0h],rax
                    00
  00000001400EB877: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  00000001400EB87F: 4C 8D 0D B2 F5 09  lea         r9,[000000014218AE38h]
                    02
  00000001400EB886: 4C 8B 84 24 E0 00  mov         r8,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400EB88E: 48 8D 15 B3 F5 09  lea         rdx,[000000014218AE48h]
                    02
  00000001400EB895: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400EB89D: 48 8B 89 B0 00 00  mov         rcx,qword ptr [rcx+00000000000000B0h]
                    00
  00000001400EB8A4: FF 15 26 89 D1 01  call        qword ptr [0000000141E041D0h]
  00000001400EB8AA: E8 E1 66 83 00     call        0000000140921F90
  00000001400EB8AF: 89 44 24 34        mov         dword ptr [rsp+34h],eax
  00000001400EB8B3: C7 44 24 30 00 00  mov         dword ptr [rsp+30h],0
                    00 00
  00000001400EB8BB: EB 0B              jmp         00000001400EB8C8
  00000001400EB8BD: 8B 44 24 30        mov         eax,dword ptr [rsp+30h]
  00000001400EB8C1: 83 C0 01           add         eax,1
  00000001400EB8C4: 89 44 24 30        mov         dword ptr [rsp+30h],eax
  00000001400EB8C8: 8B 44 24 34        mov         eax,dword ptr [rsp+34h]
  00000001400EB8CC: 39 44 24 30        cmp         dword ptr [rsp+30h],eax
  00000001400EB8D0: 7D 7A              jge         00000001400EB94C
  00000001400EB8D2: 8B 4C 24 30        mov         ecx,dword ptr [rsp+30h]
  00000001400EB8D6: E8 C5 66 83 00     call        0000000140921FA0
  00000001400EB8DB: 8B C8              mov         ecx,eax
  00000001400EB8DD: E8 0E 67 83 00     call        0000000140921FF0
  00000001400EB8E2: 83 E0 04           and         eax,4
  00000001400EB8E5: 85 C0              test        eax,eax
  00000001400EB8E7: 74 5E              je          00000001400EB947
  00000001400EB8E9: 8B 4C 24 30        mov         ecx,dword ptr [rsp+30h]
  00000001400EB8ED: E8 AE 66 83 00     call        0000000140921FA0
  00000001400EB8F2: 8B C8              mov         ecx,eax
  00000001400EB8F4: E8 37 66 83 00     call        0000000140921F30
  00000001400EB8F9: 48 8B D0           mov         rdx,rax
  00000001400EB8FC: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  00000001400EB901: FF 15 79 8B D1 01  call        qword ptr [0000000141E04480h]
  00000001400EB907: 48 89 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],rax
                    00 00
  00000001400EB90F: 48 8B 84 24 A8 00  mov         rax,qword ptr [rsp+00000000000000A8h]
                    00 00
  00000001400EB917: 48 89 84 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],rax
                    00 00
  00000001400EB91F: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400EB927: 48 81 C1 90 00 00  add         rcx,90h
                    00
  00000001400EB92E: 48 8B 94 24 B0 00  mov         rdx,qword ptr [rsp+00000000000000B0h]
                    00 00
  00000001400EB936: E8 85 EA BD 00     call        0000000140CCA3C0
  00000001400EB93B: 90                 nop
  00000001400EB93C: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  00000001400EB941: FF 15 99 A0 D1 01  call        qword ptr [0000000141E059E0h]
  00000001400EB947: E9 71 FF FF FF     jmp         00000001400EB8BD
  00000001400EB94C: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400EB954: 48 81 C1 90 00 00  add         rcx,90h
                    00
  00000001400EB95B: E8 F0 12 01 00     call        00000001400FCC50
  00000001400EB960: B9 10 00 00 00     mov         ecx,10h
  00000001400EB965: E8 52 31 38 01     call        000000014146EABC
  00000001400EB96A: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  00000001400EB96F: 48 83 7C 24 68 00  cmp         qword ptr [rsp+68h],0
  00000001400EB975: 74 16              je          00000001400EB98D
  00000001400EB977: 33 D2              xor         edx,edx
  00000001400EB979: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  00000001400EB97E: E8 2D 37 01 00     call        00000001400FF0B0
  00000001400EB983: 48 89 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],rax
                    00 00
  00000001400EB98B: EB 0C              jmp         00000001400EB999
  00000001400EB98D: 48 C7 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],0
                    00 00 00 00 00 00
  00000001400EB999: 48 8B 84 24 B8 00  mov         rax,qword ptr [rsp+00000000000000B8h]
                    00 00
  00000001400EB9A1: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  00000001400EB9A6: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400EB9AE: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  00000001400EB9B3: 48 89 41 60        mov         qword ptr [rcx+60h],rax
  00000001400EB9B7: B9 10 00 00 00     mov         ecx,10h
  00000001400EB9BC: E8 FB 30 38 01     call        000000014146EABC
  00000001400EB9C1: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  00000001400EB9C6: 48 83 7C 24 78 00  cmp         qword ptr [rsp+78h],0
  00000001400EB9CC: 74 16              je          00000001400EB9E4
  00000001400EB9CE: 33 D2              xor         edx,edx
  00000001400EB9D0: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  00000001400EB9D5: E8 96 3B 01 00     call        00000001400FF570
  00000001400EB9DA: 48 89 84 24 C0 00  mov         qword ptr [rsp+00000000000000C0h],rax
                    00 00
  00000001400EB9E2: EB 0C              jmp         00000001400EB9F0
  00000001400EB9E4: 48 C7 84 24 C0 00  mov         qword ptr [rsp+00000000000000C0h],0
                    00 00 00 00 00 00
  00000001400EB9F0: 48 8B 84 24 C0 00  mov         rax,qword ptr [rsp+00000000000000C0h]
                    00 00
  00000001400EB9F8: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  00000001400EB9FD: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400EBA05: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  00000001400EBA0A: 48 89 41 68        mov         qword ptr [rcx+68h],rax
  00000001400EBA0E: B9 18 00 00 00     mov         ecx,18h
  00000001400EBA13: E8 A4 30 38 01     call        000000014146EABC
  00000001400EBA18: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  00000001400EBA20: 48 83 BC 24 88 00  cmp         qword ptr [rsp+0000000000000088h],0
                    00 00 00
  00000001400EBA29: 74 29              je          00000001400EBA54
  00000001400EBA2B: 48 8B 94 24 E0 00  mov         rdx,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400EBA33: 48 81 C2 90 00 00  add         rdx,90h
                    00
  00000001400EBA3A: 45 33 C0           xor         r8d,r8d
  00000001400EBA3D: 48 8B 8C 24 88 00  mov         rcx,qword ptr [rsp+0000000000000088h]
                    00 00
  00000001400EBA45: E8 F6 3D 01 00     call        00000001400FF840
  00000001400EBA4A: 48 89 84 24 C8 00  mov         qword ptr [rsp+00000000000000C8h],rax
                    00 00
  00000001400EBA52: EB 0C              jmp         00000001400EBA60
  00000001400EBA54: 48 C7 84 24 C8 00  mov         qword ptr [rsp+00000000000000C8h],0
                    00 00 00 00 00 00
  00000001400EBA60: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  00000001400EBA68: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  00000001400EBA70: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400EBA78: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001400EBA80: 48 89 41 70        mov         qword ptr [rcx+70h],rax
  00000001400EBA84: 48 8B 84 24 E0 00  mov         rax,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400EBA8C: 48 C7 40 78 00 00  mov         qword ptr [rax+78h],0
                    00 00
  00000001400EBA94: 48 8B 84 24 E0 00  mov         rax,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400EBA9C: 48 C7 80 80 00 00  mov         qword ptr [rax+0000000000000080h],0
                    00 00 00 00 00
  00000001400EBAA7: 48 8B 84 24 E0 00  mov         rax,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400EBAAF: 48 C7 80 88 00 00  mov         qword ptr [rax+0000000000000088h],0
                    00 00 00 00 00
  00000001400EBABA: 48 8B 84 24 E0 00  mov         rax,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400EBAC2: C7 80 A0 00 00 00  mov         dword ptr [rax+00000000000000A0h],0FFFFFFFFh
                    FF FF FF FF
  00000001400EBACC: 48 8B 84 24 E0 00  mov         rax,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400EBAD4: C7 80 A4 00 00 00  mov         dword ptr [rax+00000000000000A4h],0
                    00 00 00 00
  00000001400EBADE: 48 8B 84 24 E0 00  mov         rax,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400EBAE6: C7 80 9C 00 00 00  mov         dword ptr [rax+000000000000009Ch],0FFFFFFFFh
                    FF FF FF FF
  00000001400EBAF0: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400EBAF8: E8 E3 8C 00 00     call        00000001400F47E0
  00000001400EBAFD: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400EBB05: 48 8B 49 38        mov         rcx,qword ptr [rcx+38h]
  00000001400EBB09: 33 D2              xor         edx,edx
  00000001400EBB0B: 48 8B 49 18        mov         rcx,qword ptr [rcx+18h]
  00000001400EBB0F: FF 15 2B FF D1 01  call        qword ptr [0000000141E0BA40h]
  00000001400EBB15: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400EBB1D: 48 8B 49 38        mov         rcx,qword ptr [rcx+38h]
  00000001400EBB21: 33 D2              xor         edx,edx
  00000001400EBB23: 48 8B 49 20        mov         rcx,qword ptr [rcx+20h]
  00000001400EBB27: FF 15 13 FF D1 01  call        qword ptr [0000000141E0BA40h]
  00000001400EBB2D: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400EBB35: 48 8B 49 38        mov         rcx,qword ptr [rcx+38h]
  00000001400EBB39: 33 D2              xor         edx,edx
  00000001400EBB3B: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  00000001400EBB3F: FF 15 FB FE D1 01  call        qword ptr [0000000141E0BA40h]
  00000001400EBB45: 48 8B 84 24 E0 00  mov         rax,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400EBB4D: 48 8B 40 38        mov         rax,qword ptr [rax+38h]
  00000001400EBB51: 48 8B 40 18        mov         rax,qword ptr [rax+18h]
  00000001400EBB55: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400EBB5D: 48 8B 49 38        mov         rcx,qword ptr [rcx+38h]
  00000001400EBB61: 48 8B 49 18        mov         rcx,qword ptr [rcx+18h]
  00000001400EBB65: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400EBB68: 33 D2              xor         edx,edx
  00000001400EBB6A: FF 50 58           call        qword ptr [rax+58h]
  00000001400EBB6D: 48 8B 84 24 E0 00  mov         rax,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400EBB75: 48 8B 40 38        mov         rax,qword ptr [rax+38h]
  00000001400EBB79: 48 8B 40 20        mov         rax,qword ptr [rax+20h]
  00000001400EBB7D: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400EBB85: 48 8B 49 38        mov         rcx,qword ptr [rcx+38h]
  00000001400EBB89: 48 8B 49 20        mov         rcx,qword ptr [rcx+20h]
  00000001400EBB8D: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400EBB90: 33 D2              xor         edx,edx
  00000001400EBB92: FF 50 58           call        qword ptr [rax+58h]
  00000001400EBB95: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400EBB9D: 48 8B 49 38        mov         rcx,qword ptr [rcx+38h]
  00000001400EBBA1: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  00000001400EBBA9: 4C 8D 0D A8 F2 09  lea         r9,[000000014218AE58h]
                    02
  00000001400EBBB0: 4C 8B 84 24 E0 00  mov         r8,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400EBBB8: 48 8D 15 C9 F2 09  lea         rdx,[000000014218AE88h]
                    02
  00000001400EBBBF: 48 8B 89 90 00 00  mov         rcx,qword ptr [rcx+0000000000000090h]
                    00
  00000001400EBBC6: FF 15 04 86 D1 01  call        qword ptr [0000000141E041D0h]
  00000001400EBBCC: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400EBBD4: 48 8B 49 38        mov         rcx,qword ptr [rcx+38h]
  00000001400EBBD8: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  00000001400EBBE0: 4C 8D 0D C1 F2 09  lea         r9,[000000014218AEA8h]
                    02
  00000001400EBBE7: 4C 8B 84 24 E0 00  mov         r8,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400EBBEF: 48 8D 15 E2 F2 09  lea         rdx,[000000014218AED8h]
                    02
  00000001400EBBF6: 48 8B 89 90 00 00  mov         rcx,qword ptr [rcx+0000000000000090h]
                    00
  00000001400EBBFD: FF 15 CD 85 D1 01  call        qword ptr [0000000141E041D0h]
  00000001400EBC03: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400EBC0B: 48 8B 49 38        mov         rcx,qword ptr [rcx+38h]
  00000001400EBC0F: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  00000001400EBC17: 4C 8D 0D DA F2 09  lea         r9,[000000014218AEF8h]
                    02
  00000001400EBC1E: 4C 8B 84 24 E0 00  mov         r8,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400EBC26: 48 8D 15 FB F2 09  lea         rdx,[000000014218AF28h]
                    02
  00000001400EBC2D: 48 8B 89 90 00 00  mov         rcx,qword ptr [rcx+0000000000000090h]
                    00
  00000001400EBC34: FF 15 96 85 D1 01  call        qword ptr [0000000141E041D0h]
  00000001400EBC3A: C6 44 24 20 00     mov         byte ptr [rsp+20h],0
  00000001400EBC3F: 4C 8D 0D 0A F3 09  lea         r9,[000000014218AF50h]
                    02
  00000001400EBC46: 4C 8B 84 24 E0 00  mov         r8,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400EBC4E: 48 8D 15 0B F3 09  lea         rdx,[000000014218AF60h]
                    02
  00000001400EBC55: 48 8D 0D A4 AA C8  lea         rcx,[0000000143D76700h]
                    03
  00000001400EBC5C: E8 7F 3F 1E 00     call        00000001402CFBE0
  00000001400EBC61: 90                 nop
  00000001400EBC62: 48 8B 84 24 E0 00  mov         rax,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400EBC6A: 48 81 C4 D0 00 00  add         rsp,0D0h
                    00
  00000001400EBC71: 5F                 pop         rdi
  00000001400EBC72: C3                 ret

  Summary

     1E02000 .text
