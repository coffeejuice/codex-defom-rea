Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001407A0150: 44 88 44 24 18     mov         byte ptr [rsp+18h],r8b
  00000001407A0155: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001407A015A: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001407A015F: 48 81 EC 98 00 00  sub         rsp,98h
                    00
  00000001407A0166: 48 C7 44 24 58 FE  mov         qword ptr [rsp+58h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001407A016F: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  00000001407A0174: 48 8B 8C 24 A8 00  mov         rcx,qword ptr [rsp+00000000000000A8h]
                    00 00
  00000001407A017C: FF 15 36 C4 66 01  call        qword ptr [0000000141E0C5B8h]
  00000001407A0182: 90                 nop
  00000001407A0183: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001407A0188: FF 15 AA C3 66 01  call        qword ptr [0000000141E0C538h]
  00000001407A018E: 0F B6 C0           movzx       eax,al
  00000001407A0191: 85 C0              test        eax,eax
  00000001407A0193: 0F 85 01 01 00 00  jne         00000001407A029A
  00000001407A0199: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  00000001407A019E: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001407A01A3: FF 15 07 C4 66 01  call        qword ptr [0000000141E0C5B0h]
  00000001407A01A9: 90                 nop
  00000001407A01AA: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000001407A01AF: FF 15 83 C3 66 01  call        qword ptr [0000000141E0C538h]
  00000001407A01B5: 0F B6 C0           movzx       eax,al
  00000001407A01B8: 85 C0              test        eax,eax
  00000001407A01BA: 0F 85 88 00 00 00  jne         00000001407A0248
  00000001407A01C0: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  00000001407A01C5: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000001407A01CA: FF 15 D8 C3 66 01  call        qword ptr [0000000141E0C5A8h]
  00000001407A01D0: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  00000001407A01D5: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  00000001407A01DA: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  00000001407A01DF: 48 8D 15 E2 E1 EF  lea         rdx,[000000014269E3C8h]
                    01
  00000001407A01E6: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  00000001407A01EB: FF 15 77 42 66 01  call        qword ptr [0000000141E04468h]
  00000001407A01F1: 88 44 24 30        mov         byte ptr [rsp+30h],al
  00000001407A01F5: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  00000001407A01FA: FF 15 E0 57 66 01  call        qword ptr [0000000141E059E0h]
  00000001407A0200: 44 0F B6 5C 24 30  movzx       r11d,byte ptr [rsp+30h]
  00000001407A0206: 45 85 DB           test        r11d,r11d
  00000001407A0209: 74 3D              je          00000001407A0248
  00000001407A020B: 48 8D 44 24 40     lea         rax,[rsp+40h]
  00000001407A0210: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001407A0215: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  00000001407A021A: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  00000001407A021F: FF 15 73 C4 66 01  call        qword ptr [0000000141E0C698h]
  00000001407A0225: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  00000001407A022A: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001407A0232: 48 8B 00           mov         rax,qword ptr [rax]
  00000001407A0235: 48 8B 54 24 70     mov         rdx,qword ptr [rsp+70h]
  00000001407A023A: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001407A0242: FF 90 40 02 00 00  call        qword ptr [rax+0000000000000240h]
  00000001407A0248: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  00000001407A024D: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001407A0252: FF 15 40 C3 66 01  call        qword ptr [0000000141E0C598h]
  00000001407A0258: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  00000001407A025D: 48 8B 44 24 78     mov         rax,qword ptr [rsp+78h]
  00000001407A0262: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  00000001407A026A: 48 8B 94 24 80 00  mov         rdx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001407A0272: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001407A0277: FF 15 E3 C2 66 01  call        qword ptr [0000000141E0C560h]
  00000001407A027D: 90                 nop
  00000001407A027E: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  00000001407A0283: FF 15 1F C4 66 01  call        qword ptr [0000000141E0C6A8h]
  00000001407A0289: 90                 nop
  00000001407A028A: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000001407A028F: FF 15 1B C4 66 01  call        qword ptr [0000000141E0C6B0h]
  00000001407A0295: E9 E9 FE FF FF     jmp         00000001407A0183
  00000001407A029A: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001407A029F: FF 15 03 C4 66 01  call        qword ptr [0000000141E0C6A8h]
  00000001407A02A5: 90                 nop
  00000001407A02A6: 48 8B 8C 24 A8 00  mov         rcx,qword ptr [rsp+00000000000000A8h]
                    00 00
  00000001407A02AE: FF 15 FC C3 66 01  call        qword ptr [0000000141E0C6B0h]
  00000001407A02B4: 48 81 C4 98 00 00  add         rsp,98h
                    00
  00000001407A02BB: C3                 ret
  00000001407A02BC: CC                 int         3

  Summary

     1E02000 .text
