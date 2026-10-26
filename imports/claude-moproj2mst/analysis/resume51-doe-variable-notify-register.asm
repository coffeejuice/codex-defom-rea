Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001402CFBE0: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  00000001402CFBE5: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  00000001402CFBEA: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001402CFBEF: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001402CFBF4: 48 81 EC 08 01 00  sub         rsp,108h
                    00
  00000001402CFBFB: 48 C7 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  00000001402CFC07: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001402CFC0C: E8 CF FD EB FF     call        000000014018F9E0
  00000001402CFC11: 90                 nop
  00000001402CFC12: B9 10 00 00 00     mov         ecx,10h
  00000001402CFC17: E8 A0 EE 19 01     call        000000014146EABC
  00000001402CFC1C: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  00000001402CFC21: 48 83 7C 24 60 00  cmp         qword ptr [rsp+60h],0
  00000001402CFC27: 74 14              je          00000001402CFC3D
  00000001402CFC29: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001402CFC2E: E8 2D 07 00 00     call        00000001402D0360
  00000001402CFC33: 48 89 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],rax
                    00 00
  00000001402CFC3B: EB 0C              jmp         00000001402CFC49
  00000001402CFC3D: 48 C7 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],0
                    00 00 00 00 00 00
  00000001402CFC49: 48 8B 84 24 A8 00  mov         rax,qword ptr [rsp+00000000000000A8h]
                    00 00
  00000001402CFC51: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  00000001402CFC56: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  00000001402CFC5B: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001402CFC60: 48 8B 94 24 28 01  mov         rdx,qword ptr [rsp+0000000000000128h]
                    00 00
  00000001402CFC68: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000001402CFC6D: FF 15 0D 48 B3 01  call        qword ptr [0000000141E04480h]
  00000001402CFC73: 48 89 84 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],rax
                    00 00
  00000001402CFC7B: 48 8B 84 24 B0 00  mov         rax,qword ptr [rsp+00000000000000B0h]
                    00 00
  00000001402CFC83: 48 89 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],rax
                    00 00
  00000001402CFC8B: 41 B9 FF FF FF FF  mov         r9d,0FFFFFFFFh
  00000001402CFC91: 41 B8 01 00 00 00  mov         r8d,1
  00000001402CFC97: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  00000001402CFC9C: 48 8B 8C 24 B8 00  mov         rcx,qword ptr [rsp+00000000000000B8h]
                    00 00
  00000001402CFCA4: FF 15 76 46 B3 01  call        qword ptr [0000000141E04320h]
  00000001402CFCAA: 90                 nop
  00000001402CFCAB: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000001402CFCB0: FF 15 2A 5D B3 01  call        qword ptr [0000000141E059E0h]
  00000001402CFCB6: 48 8D 15 0B 72 00  lea         rdx,[00000001422D6EC8h]
                    02
  00000001402CFCBD: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  00000001402CFCC2: FF 15 B8 47 B3 01  call        qword ptr [0000000141E04480h]
  00000001402CFCC8: 90                 nop
  00000001402CFCC9: 41 B9 01 00 00 00  mov         r9d,1
  00000001402CFCCF: 45 33 C0           xor         r8d,r8d
  00000001402CFCD2: 48 8D 54 24 70     lea         rdx,[rsp+70h]
  00000001402CFCD7: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000001402CFCDC: FF 15 B6 57 B3 01  call        qword ptr [0000000141E05498h]
  00000001402CFCE2: 8B D0              mov         edx,eax
  00000001402CFCE4: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000001402CFCE9: FF 15 B9 49 B3 01  call        qword ptr [0000000141E046A8h]
  00000001402CFCEF: 90                 nop
  00000001402CFCF0: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  00000001402CFCF5: FF 15 E5 5C B3 01  call        qword ptr [0000000141E059E0h]
  00000001402CFCFB: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000001402CFD00: FF 15 C2 42 B3 01  call        qword ptr [0000000141E03FC8h]
  00000001402CFD06: 0F B6 C0           movzx       eax,al
  00000001402CFD09: 85 C0              test        eax,eax
  00000001402CFD0B: 74 13              je          00000001402CFD20
  00000001402CFD0D: 48 8B 94 24 28 01  mov         rdx,qword ptr [rsp+0000000000000128h]
                    00 00
  00000001402CFD15: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000001402CFD1A: FF 15 38 41 B3 01  call        qword ptr [0000000141E03E58h]
  00000001402CFD20: 48 8B 94 24 20 01  mov         rdx,qword ptr [rsp+0000000000000120h]
                    00 00
  00000001402CFD28: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  00000001402CFD2D: E8 EE CB 9E 00     call        0000000140CBC920
  00000001402CFD32: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  00000001402CFD37: 48 83 C1 08        add         rcx,8
  00000001402CFD3B: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  00000001402CFD40: FF 15 2A 47 B3 01  call        qword ptr [0000000141E04470h]
  00000001402CFD46: 48 8B 94 24 18 01  mov         rdx,qword ptr [rsp+0000000000000118h]
                    00 00
  00000001402CFD4E: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  00000001402CFD56: FF 15 24 47 B3 01  call        qword ptr [0000000141E04480h]
  00000001402CFD5C: 48 89 84 24 C0 00  mov         qword ptr [rsp+00000000000000C0h],rax
                    00 00
  00000001402CFD64: 48 8B 84 24 C0 00  mov         rax,qword ptr [rsp+00000000000000C0h]
                    00 00
  00000001402CFD6C: 48 89 84 24 C8 00  mov         qword ptr [rsp+00000000000000C8h],rax
                    00 00
  00000001402CFD74: 48 8B 8C 24 10 01  mov         rcx,qword ptr [rsp+0000000000000110h]
                    00 00
  00000001402CFD7C: 48 81 C1 88 00 00  add         rcx,88h
                    00
  00000001402CFD83: 48 8B 94 24 C8 00  mov         rdx,qword ptr [rsp+00000000000000C8h]
                    00 00
  00000001402CFD8B: E8 90 0B DA FF     call        0000000140070920
  00000001402CFD90: 88 44 24 78        mov         byte ptr [rsp+78h],al
  00000001402CFD94: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  00000001402CFD9C: FF 15 3E 5C B3 01  call        qword ptr [0000000141E059E0h]
  00000001402CFDA2: 44 0F B6 5C 24 78  movzx       r11d,byte ptr [rsp+78h]
  00000001402CFDA8: 45 85 DB           test        r11d,r11d
  00000001402CFDAB: 74 66              je          00000001402CFE13
  00000001402CFDAD: 48 8B 94 24 18 01  mov         rdx,qword ptr [rsp+0000000000000118h]
                    00 00
  00000001402CFDB5: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  00000001402CFDBD: FF 15 BD 46 B3 01  call        qword ptr [0000000141E04480h]
  00000001402CFDC3: 48 89 84 24 D0 00  mov         qword ptr [rsp+00000000000000D0h],rax
                    00 00
  00000001402CFDCB: 48 8B 84 24 D0 00  mov         rax,qword ptr [rsp+00000000000000D0h]
                    00 00
  00000001402CFDD3: 48 89 84 24 D8 00  mov         qword ptr [rsp+00000000000000D8h],rax
                    00 00
  00000001402CFDDB: 48 8B 8C 24 10 01  mov         rcx,qword ptr [rsp+0000000000000110h]
                    00 00
  00000001402CFDE3: 48 81 C1 88 00 00  add         rcx,88h
                    00
  00000001402CFDEA: 48 8B 94 24 D8 00  mov         rdx,qword ptr [rsp+00000000000000D8h]
                    00 00
  00000001402CFDF2: E8 F9 05 00 00     call        00000001402D03F0
  00000001402CFDF7: 48 8B D0           mov         rdx,rax
  00000001402CFDFA: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001402CFDFF: E8 DC D7 E8 FF     call        000000014015D5E0
  00000001402CFE04: 90                 nop
  00000001402CFE05: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  00000001402CFE0D: FF 15 CD 5B B3 01  call        qword ptr [0000000141E059E0h]
  00000001402CFE13: 0F B6 84 24 30 01  movzx       eax,byte ptr [rsp+0000000000000130h]
                    00 00
  00000001402CFE1B: 85 C0              test        eax,eax
  00000001402CFE1D: 74 16              je          00000001402CFE35
  00000001402CFE1F: 4C 8D 44 24 38     lea         r8,[rsp+38h]
  00000001402CFE24: 33 D2              xor         edx,edx
  00000001402CFE26: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001402CFE2B: E8 20 08 00 00     call        00000001402D0650
  00000001402CFE30: E9 9D 00 00 00     jmp         00000001402CFED2
  00000001402CFE35: C6 44 24 48 00     mov         byte ptr [rsp+48h],0
  00000001402CFE3A: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001402CFE3F: E8 9C 4C D3 FF     call        0000000140004AE0
  00000001402CFE44: 89 44 24 44        mov         dword ptr [rsp+44h],eax
  00000001402CFE48: C7 44 24 40 00 00  mov         dword ptr [rsp+40h],0
                    00 00
  00000001402CFE50: EB 0B              jmp         00000001402CFE5D
  00000001402CFE52: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  00000001402CFE56: 83 C0 01           add         eax,1
  00000001402CFE59: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  00000001402CFE5D: 8B 44 24 44        mov         eax,dword ptr [rsp+44h]
  00000001402CFE61: 39 44 24 40        cmp         dword ptr [rsp+40h],eax
  00000001402CFE65: 7D 53              jge         00000001402CFEBA
  00000001402CFE67: 8B 54 24 40        mov         edx,dword ptr [rsp+40h]
  00000001402CFE6B: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001402CFE70: E8 9B EF 01 00     call        00000001402EEE10
  00000001402CFE75: 48 8B 00           mov         rax,qword ptr [rax]
  00000001402CFE78: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000001402CFE7D: 48 8B 94 24 20 01  mov         rdx,qword ptr [rsp+0000000000000120h]
                    00 00
  00000001402CFE85: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001402CFE8A: E8 D1 73 D9 FF     call        0000000140067260
  00000001402CFE8F: 0F B6 C0           movzx       eax,al
  00000001402CFE92: 85 C0              test        eax,eax
  00000001402CFE94: 74 22              je          00000001402CFEB8
  00000001402CFE96: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001402CFE9B: 48 83 C1 08        add         rcx,8
  00000001402CFE9F: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  00000001402CFEA4: FF 15 5E 40 B3 01  call        qword ptr [0000000141E03F08h]
  00000001402CFEAA: 0F B6 C0           movzx       eax,al
  00000001402CFEAD: 85 C0              test        eax,eax
  00000001402CFEAF: 74 07              je          00000001402CFEB8
  00000001402CFEB1: C6 44 24 48 01     mov         byte ptr [rsp+48h],1
  00000001402CFEB6: EB 02              jmp         00000001402CFEBA
  00000001402CFEB8: EB 98              jmp         00000001402CFE52
  00000001402CFEBA: 0F B6 44 24 48     movzx       eax,byte ptr [rsp+48h]
  00000001402CFEBF: 85 C0              test        eax,eax
  00000001402CFEC1: 75 0F              jne         00000001402CFED2
  00000001402CFEC3: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  00000001402CFEC8: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001402CFECD: E8 AE 06 00 00     call        00000001402D0580
  00000001402CFED2: 48 8B 94 24 18 01  mov         rdx,qword ptr [rsp+0000000000000118h]
                    00 00
  00000001402CFEDA: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  00000001402CFEE2: FF 15 98 45 B3 01  call        qword ptr [0000000141E04480h]
  00000001402CFEE8: 48 89 84 24 E0 00  mov         qword ptr [rsp+00000000000000E0h],rax
                    00 00
  00000001402CFEF0: 48 8B 84 24 E0 00  mov         rax,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001402CFEF8: 48 89 84 24 E8 00  mov         qword ptr [rsp+00000000000000E8h],rax
                    00 00
  00000001402CFF00: 48 8B 8C 24 10 01  mov         rcx,qword ptr [rsp+0000000000000110h]
                    00 00
  00000001402CFF08: 48 81 C1 88 00 00  add         rcx,88h
                    00
  00000001402CFF0F: 48 8B 94 24 E8 00  mov         rdx,qword ptr [rsp+00000000000000E8h]
                    00 00
  00000001402CFF17: E8 D4 04 00 00     call        00000001402D03F0
  00000001402CFF1C: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  00000001402CFF21: 48 8B C8           mov         rcx,rax
  00000001402CFF24: E8 B7 D6 E8 FF     call        000000014015D5E0
  00000001402CFF29: 90                 nop
  00000001402CFF2A: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  00000001402CFF32: FF 15 A8 5A B3 01  call        qword ptr [0000000141E059E0h]
  00000001402CFF38: 48 8B 94 24 18 01  mov         rdx,qword ptr [rsp+0000000000000118h]
                    00 00
  00000001402CFF40: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  00000001402CFF48: FF 15 32 45 B3 01  call        qword ptr [0000000141E04480h]
  00000001402CFF4E: 48 89 84 24 F0 00  mov         qword ptr [rsp+00000000000000F0h],rax
                    00 00
  00000001402CFF56: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  00000001402CFF5E: 48 89 84 24 F8 00  mov         qword ptr [rsp+00000000000000F8h],rax
                    00 00
  00000001402CFF66: 48 8B 8C 24 10 01  mov         rcx,qword ptr [rsp+0000000000000110h]
                    00 00
  00000001402CFF6E: 48 81 C1 88 00 00  add         rcx,88h
                    00
  00000001402CFF75: 48 8B 94 24 F8 00  mov         rdx,qword ptr [rsp+00000000000000F8h]
                    00 00
  00000001402CFF7D: E8 6E 04 00 00     call        00000001402D03F0
  00000001402CFF82: 48 8B D0           mov         rdx,rax
  00000001402CFF85: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001402CFF8A: E8 51 D6 E8 FF     call        000000014015D5E0
  00000001402CFF8F: 90                 nop
  00000001402CFF90: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  00000001402CFF98: FF 15 42 5A B3 01  call        qword ptr [0000000141E059E0h]
  00000001402CFF9E: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001402CFFA3: E8 38 4B D3 FF     call        0000000140004AE0
  00000001402CFFA8: 89 44 24 30        mov         dword ptr [rsp+30h],eax
  00000001402CFFAC: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000001402CFFB1: FF 15 29 5A B3 01  call        qword ptr [0000000141E059E0h]
  00000001402CFFB7: 90                 nop
  00000001402CFFB8: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001402CFFBD: E8 DE 81 F3 FF     call        00000001402081A0
  00000001402CFFC2: 48 81 C4 08 01 00  add         rsp,108h
                    00
  00000001402CFFC9: C3                 ret

  Summary

     1E02000 .text
