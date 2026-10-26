RANGE 0x1408169d0..0x140816acb SHA256 39ec8fac0a21adf717ace5ebe7b1a93fc979b50873fb5e81226e340eee6eb5e5
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001408169D0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001408169D5: 48 83 EC 68        sub         rsp,68h
  00000001408169D9: 48 C7 44 24 48 FE  mov         qword ptr [rsp+48h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001408169E2: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  00000001408169E7: 48 83 C1 30        add         rcx,30h
  00000001408169EB: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  00000001408169F0: E8 0B AE 43 00     call        0000000140C51800
  00000001408169F5: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  00000001408169FA: 48 83 C1 30        add         rcx,30h
  00000001408169FE: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  0000000140816A03: E8 68 51 5A 00     call        0000000140DBBB70
  0000000140816A08: 48 8B D0           mov         rdx,rax
  0000000140816A0B: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140816A10: E8 5B FA 90 FF     call        0000000140126470
  0000000140816A15: 0F B6 C0           movzx       eax,al
  0000000140816A18: 85 C0              test        eax,eax
  0000000140816A1A: 74 57              je          0000000140816A73
  0000000140816A1C: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140816A21: E8 DA 89 E3 FF     call        000000014064F400
  0000000140816A26: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140816A29: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140816A2E: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140816A33: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140816A38: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  0000000140816A3E: 74 16              je          0000000140816A56
  0000000140816A40: BA 01 00 00 00     mov         edx,1
  0000000140816A45: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140816A4A: E8 01 1D 00 00     call        0000000140818750
  0000000140816A4F: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140816A54: EB 09              jmp         0000000140816A5F
  0000000140816A56: 48 C7 44 24 50 00  mov         qword ptr [rsp+50h],0
                    00 00 00
  0000000140816A5F: 45 33 C0           xor         r8d,r8d
  0000000140816A62: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  0000000140816A67: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140816A6C: E8 AF 11 BC 00     call        00000001413D7C20
  0000000140816A71: EB 82              jmp         00000001408169F5
  0000000140816A73: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140816A78: E8 93 12 00 00     call        0000000140817D10
  0000000140816A7D: 90                 nop
  0000000140816A7E: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140816A83: 48 83 C1 38        add         rcx,38h
  0000000140816A87: E8 14 17 9F FF     call        00000001402081A0
  0000000140816A8C: 90                 nop
  0000000140816A8D: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140816A92: 48 83 C1 30        add         rcx,30h
  0000000140816A96: E8 05 17 9F FF     call        00000001402081A0
  0000000140816A9B: 90                 nop
  0000000140816A9C: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140816AA1: 48 83 C1 28        add         rcx,28h
  0000000140816AA5: E8 F6 16 9F FF     call        00000001402081A0
  0000000140816AAA: 90                 nop
  0000000140816AAB: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140816AB0: 48 83 C1 08        add         rcx,8
  0000000140816AB4: FF 15 26 EF 5E 01  call        qword ptr [0000000141E059E0h]
  0000000140816ABA: 90                 nop
  0000000140816ABB: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140816AC0: FF 15 1A EF 5E 01  call        qword ptr [0000000141E059E0h]
  0000000140816AC6: 48 83 C4 68        add         rsp,68h
  0000000140816ACA: C3                 ret

  Summary

     1E02000 .text

RANGE 0x140818750..0x140818786 SHA256 5bc5860db5db944084d0efca4858d5228f3cc07f3d83f33cc090772d0ab0cb20
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140818750: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140818754: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140818759: 48 83 EC 28        sub         rsp,28h
  000000014081875D: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140818762: E8 79 81 10 00     call        00000001409208E0
  0000000140818767: 8B 44 24 38        mov         eax,dword ptr [rsp+38h]
  000000014081876B: 83 E0 01           and         eax,1
  000000014081876E: 85 C0              test        eax,eax
  0000000140818770: 74 0A              je          000000014081877C
  0000000140818772: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140818777: E8 EC 74 CC 00     call        00000001414DFC68
  000000014081877C: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140818781: 48 83 C4 28        add         rsp,28h
  0000000140818785: C3                 ret

  Summary

     1E02000 .text

RANGE 0x1409208e0..0x140920a9e SHA256 d1933c038f136bc1059318dbbc6bc3161fc7a7c6be597db4e4bd79ce65da0dcb
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001409208E0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001409208E5: 48 83 EC 78        sub         rsp,78h
  00000001409208E9: 48 C7 44 24 50 FE  mov         qword ptr [rsp+50h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001409208F2: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001409208FA: 48 83 B8 98 00 00  cmp         qword ptr [rax+0000000000000098h],0
                    00 00
  0000000140920902: 74 45              je          0000000140920949
  0000000140920904: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  000000014092090C: 48 8B 80 98 00 00  mov         rax,qword ptr [rax+0000000000000098h]
                    00
  0000000140920913: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140920918: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  000000014092091D: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140920922: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140920928: 74 16              je          0000000140920940
  000000014092092A: BA 01 00 00 00     mov         edx,1
  000000014092092F: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140920934: E8 97 00 7E FF     call        00000001401009D0
  0000000140920939: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  000000014092093E: EB 09              jmp         0000000140920949
  0000000140920940: 48 C7 44 24 58 00  mov         qword ptr [rsp+58h],0
                    00 00 00
  0000000140920949: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140920951: 48 83 B8 88 00 00  cmp         qword ptr [rax+0000000000000088h],0
                    00 00
  0000000140920959: 74 45              je          00000001409209A0
  000000014092095B: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140920963: 48 8B 80 88 00 00  mov         rax,qword ptr [rax+0000000000000088h]
                    00
  000000014092096A: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  000000014092096F: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140920974: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140920979: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  000000014092097F: 74 16              je          0000000140920997
  0000000140920981: BA 01 00 00 00     mov         edx,1
  0000000140920986: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014092098B: E8 40 00 7E FF     call        00000001401009D0
  0000000140920990: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000140920995: EB 09              jmp         00000001409209A0
  0000000140920997: 48 C7 44 24 60 00  mov         qword ptr [rsp+60h],0
                    00 00 00
  00000001409209A0: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001409209A8: 48 83 B8 90 00 00  cmp         qword ptr [rax+0000000000000090h],0
                    00 00
  00000001409209B0: 74 45              je          00000001409209F7
  00000001409209B2: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001409209BA: 48 8B 80 90 00 00  mov         rax,qword ptr [rax+0000000000000090h]
                    00
  00000001409209C1: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001409209C6: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  00000001409209CB: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001409209D0: 48 83 7C 24 40 00  cmp         qword ptr [rsp+40h],0
  00000001409209D6: 74 16              je          00000001409209EE
  00000001409209D8: BA 01 00 00 00     mov         edx,1
  00000001409209DD: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001409209E2: E8 E9 FF 7D FF     call        00000001401009D0
  00000001409209E7: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  00000001409209EC: EB 09              jmp         00000001409209F7
  00000001409209EE: 48 C7 44 24 68 00  mov         qword ptr [rsp+68h],0
                    00 00 00
  00000001409209F7: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001409209FF: 48 83 C1 68        add         rcx,68h
  0000000140920A03: E8 98 77 8E FF     call        00000001402081A0
  0000000140920A08: 90                 nop
  0000000140920A09: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140920A11: 48 83 C1 60        add         rcx,60h
  0000000140920A15: E8 B6 3E 6E FF     call        00000001400048D0
  0000000140920A1A: 90                 nop
  0000000140920A1B: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140920A23: 48 83 C1 58        add         rcx,58h
  0000000140920A27: E8 74 77 8E FF     call        00000001402081A0
  0000000140920A2C: 90                 nop
  0000000140920A2D: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140920A35: 48 83 C1 50        add         rcx,50h
  0000000140920A39: E8 62 77 8E FF     call        00000001402081A0
  0000000140920A3E: 90                 nop
  0000000140920A3F: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140920A47: 48 83 C1 20        add         rcx,20h
  0000000140920A4B: FF 15 8F 4F 4E 01  call        qword ptr [0000000141E059E0h]
  0000000140920A51: 90                 nop
  0000000140920A52: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140920A5A: 48 83 C1 18        add         rcx,18h
  0000000140920A5E: FF 15 7C 4F 4E 01  call        qword ptr [0000000141E059E0h]
  0000000140920A64: 90                 nop
  0000000140920A65: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140920A6D: 48 83 C1 10        add         rcx,10h
  0000000140920A71: FF 15 69 4F 4E 01  call        qword ptr [0000000141E059E0h]
  0000000140920A77: 90                 nop
  0000000140920A78: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140920A80: 48 83 C1 08        add         rcx,8
  0000000140920A84: FF 15 56 4F 4E 01  call        qword ptr [0000000141E059E0h]
  0000000140920A8A: 90                 nop
  0000000140920A8B: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140920A93: FF 15 47 4F 4E 01  call        qword ptr [0000000141E059E0h]
  0000000140920A99: 48 83 C4 78        add         rsp,78h
  0000000140920A9D: C3                 ret

  Summary

     1E02000 .text

RANGE 0x1401009d0..0x140100a06 SHA256 f1b7baef7be6f4fde03356ac6fdce316d0ccf56752df68366d73dd61c1f2b1d3
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001401009D0: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  00000001401009D4: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001401009D9: 48 83 EC 28        sub         rsp,28h
  00000001401009DD: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001401009E2: E8 99 A7 79 00     call        000000014089B180
  00000001401009E7: 8B 44 24 38        mov         eax,dword ptr [rsp+38h]
  00000001401009EB: 83 E0 01           and         eax,1
  00000001401009EE: 85 C0              test        eax,eax
  00000001401009F0: 74 0A              je          00000001401009FC
  00000001401009F2: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001401009F7: E8 6C F2 3D 01     call        00000001414DFC68
  00000001401009FC: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140100A01: 48 83 C4 28        add         rsp,28h
  0000000140100A05: C3                 ret

  Summary

     1E02000 .text

RANGE 0x140816b40..0x140816dcd SHA256 47793f10e2ae415c7d5cbe88d3b6c593ad8c417cf2e8f6d70cea5273b8fba614
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140816B40: 44 89 4C 24 20     mov         dword ptr [rsp+20h],r9d
  0000000140816B45: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  0000000140816B4A: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140816B4E: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140816B53: 48 81 EC A8 00 00  sub         rsp,0A8h
                    00
  0000000140816B5A: 48 C7 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  0000000140816B66: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140816B6E: 48 83 C1 30        add         rcx,30h
  0000000140816B72: E8 69 DF 7E FF     call        0000000140004AE0
  0000000140816B77: 89 44 24 48        mov         dword ptr [rsp+48h],eax
  0000000140816B7B: C6 44 24 40 00     mov         byte ptr [rsp+40h],0
  0000000140816B80: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140816B88: 8B 84 24 D0 00 00  mov         eax,dword ptr [rsp+00000000000000D0h]
                    00
  0000000140816B8F: 89 41 24           mov         dword ptr [rcx+24h],eax
  0000000140816B92: 81 BC 24 B8 00 00  cmp         dword ptr [rsp+00000000000000B8h],131h
                    00 31 01 00 00
  0000000140816B9D: 0F 84 B0 00 00 00  je          0000000140816C53
  0000000140816BA3: C7 44 24 44 00 00  mov         dword ptr [rsp+44h],0
                    00 00
  0000000140816BAB: EB 0B              jmp         0000000140816BB8
  0000000140816BAD: 8B 44 24 44        mov         eax,dword ptr [rsp+44h]
  0000000140816BB1: 83 C0 01           add         eax,1
  0000000140816BB4: 89 44 24 44        mov         dword ptr [rsp+44h],eax
  0000000140816BB8: 8B 44 24 48        mov         eax,dword ptr [rsp+48h]
  0000000140816BBC: 39 44 24 44        cmp         dword ptr [rsp+44h],eax
  0000000140816BC0: 0F 8D 8D 00 00 00  jge         0000000140816C53
  0000000140816BC6: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140816BCE: 48 83 C1 30        add         rcx,30h
  0000000140816BD2: 8B 54 24 44        mov         edx,dword ptr [rsp+44h]
  0000000140816BD6: E8 35 A0 94 FF     call        0000000140160C10
  0000000140816BDB: 48 8B 08           mov         rcx,qword ptr [rax]
  0000000140816BDE: 8B 84 24 B8 00 00  mov         eax,dword ptr [rsp+00000000000000B8h]
                    00
  0000000140816BE5: 39 41 2C           cmp         dword ptr [rcx+2Ch],eax
  0000000140816BE8: 75 64              jne         0000000140816C4E
  0000000140816BEA: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140816BF2: 48 83 C1 30        add         rcx,30h
  0000000140816BF6: 8B 54 24 44        mov         edx,dword ptr [rsp+44h]
  0000000140816BFA: E8 11 A0 94 FF     call        0000000140160C10
  0000000140816BFF: 48 8B 94 24 C0 00  mov         rdx,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140816C07: 48 8B 08           mov         rcx,qword ptr [rax]
  0000000140816C0A: FF 15 F8 D2 5E 01  call        qword ptr [0000000141E03F08h]
  0000000140816C10: 0F B6 C0           movzx       eax,al
  0000000140816C13: 85 C0              test        eax,eax
  0000000140816C15: 74 37              je          0000000140816C4E
  0000000140816C17: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140816C1F: 48 83 C1 30        add         rcx,30h
  0000000140816C23: 8B 54 24 44        mov         edx,dword ptr [rsp+44h]
  0000000140816C27: E8 E4 9F 94 FF     call        0000000140160C10
  0000000140816C2C: 48 8B 08           mov         rcx,qword ptr [rax]
  0000000140816C2F: 48 83 C1 50        add         rcx,50h
  0000000140816C33: 48 8B 94 24 E8 00  mov         rdx,qword ptr [rsp+00000000000000E8h]
                    00 00
  0000000140816C3B: E8 B0 A3 8B FF     call        00000001400D0FF0
  0000000140816C40: 0F B6 C0           movzx       eax,al
  0000000140816C43: 85 C0              test        eax,eax
  0000000140816C45: 74 07              je          0000000140816C4E
  0000000140816C47: C6 44 24 40 01     mov         byte ptr [rsp+40h],1
  0000000140816C4C: EB 05              jmp         0000000140816C53
  0000000140816C4E: E9 5A FF FF FF     jmp         0000000140816BAD
  0000000140816C53: 0F B6 44 24 40     movzx       eax,byte ptr [rsp+40h]
  0000000140816C58: 85 C0              test        eax,eax
  0000000140816C5A: 0F 85 57 01 00 00  jne         0000000140816DB7
  0000000140816C60: B9 A8 00 00 00     mov         ecx,0A8h
  0000000140816C65: E8 52 7E C5 00     call        000000014146EABC
  0000000140816C6A: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000140816C6F: 48 83 7C 24 60 00  cmp         qword ptr [rsp+60h],0
  0000000140816C75: 74 79              je          0000000140816CF0
  0000000140816C77: 48 8D 44 24 68     lea         rax,[rsp+68h]
  0000000140816C7C: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  0000000140816C81: 48 8B 94 24 C0 00  mov         rdx,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140816C89: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140816C8E: FF 15 44 ED 5E 01  call        qword ptr [0000000141E059D8h]
  0000000140816C94: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  0000000140816C9C: 48 8B 84 24 E8 00  mov         rax,qword ptr [rsp+00000000000000E8h]
                    00 00
  0000000140816CA4: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140816CA9: 48 8B 84 24 E0 00  mov         rax,qword ptr [rsp+00000000000000E0h]
                    00 00
  0000000140816CB1: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140816CB6: F2 0F 10 84 24 D8  movsd       xmm0,mmword ptr [rsp+00000000000000D8h]
                    00 00 00
  0000000140816CBF: F2 0F 11 44 24 20  movsd       mmword ptr [rsp+20h],xmm0
  0000000140816CC5: 44 8B 8C 24 C8 00  mov         r9d,dword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140816CCD: 4C 8B 84 24 88 00  mov         r8,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140816CD5: 8B 94 24 B8 00 00  mov         edx,dword ptr [rsp+00000000000000B8h]
                    00
  0000000140816CDC: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140816CE1: E8 FA 99 10 00     call        00000001409206E0
  0000000140816CE6: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  0000000140816CEE: EB 0C              jmp         0000000140816CFC
  0000000140816CF0: 48 C7 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],0
                    00 00 00 00 00 00
  0000000140816CFC: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140816D04: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140816D09: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  0000000140816D0E: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140816D13: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140816D1B: 48 83 C1 30        add         rcx,30h
  0000000140816D1F: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  0000000140816D24: E8 C7 2D 4C 00     call        0000000140CD9AF0
  0000000140816D29: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  0000000140816D31: E8 AA DD 7E FF     call        0000000140004AE0
  0000000140816D36: 89 44 24 48        mov         dword ptr [rsp+48h],eax
  0000000140816D3A: C7 44 24 44 00 00  mov         dword ptr [rsp+44h],0
                    00 00
  0000000140816D42: EB 0B              jmp         0000000140816D4F
  0000000140816D44: 8B 44 24 44        mov         eax,dword ptr [rsp+44h]
  0000000140816D48: 83 C0 01           add         eax,1
  0000000140816D4B: 89 44 24 44        mov         dword ptr [rsp+44h],eax
  0000000140816D4F: 8B 44 24 48        mov         eax,dword ptr [rsp+48h]
  0000000140816D53: 39 44 24 44        cmp         dword ptr [rsp+44h],eax
  0000000140816D57: 7D 5E              jge         0000000140816DB7
  0000000140816D59: 8B 54 24 44        mov         edx,dword ptr [rsp+44h]
  0000000140816D5D: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  0000000140816D65: E8 56 E3 80 FF     call        00000001400250C0
  0000000140816D6A: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140816D72: 48 83 C1 38        add         rcx,38h
  0000000140816D76: 4C 8B C0           mov         r8,rax
  0000000140816D79: 48 8D 54 24 78     lea         rdx,[rsp+78h]
  0000000140816D7E: E8 4D E4 80 FF     call        00000001400251D0
  0000000140816D83: 48 8B C8           mov         rcx,rax
  0000000140816D86: E8 95 DC 7E FF     call        0000000140004A20
  0000000140816D8B: 48 85 C0           test        rax,rax
  0000000140816D8E: 75 25              jne         0000000140816DB5
  0000000140816D90: 8B 54 24 44        mov         edx,dword ptr [rsp+44h]
  0000000140816D94: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  0000000140816D9C: E8 1F E3 80 FF     call        00000001400250C0
  0000000140816DA1: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140816DA9: 48 83 C1 38        add         rcx,38h
  0000000140816DAD: 48 8B D0           mov         rdx,rax
  0000000140816DB0: E8 4B E3 80 FF     call        0000000140025100
  0000000140816DB5: EB 8D              jmp         0000000140816D44
  0000000140816DB7: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140816DBF: FF 15 1B EC 5E 01  call        qword ptr [0000000141E059E0h]
  0000000140816DC5: 48 81 C4 A8 00 00  add         rsp,0A8h
                    00
  0000000140816DCC: C3                 ret

  Summary

     1E02000 .text

RANGE 0x14091a5b0..0x14091a726 SHA256 2918f4c30e0b96385bff3d324fab51cbae9888c99565fc567d0cf702cdc85eed
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014091A5B0: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  000000014091A5B5: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  000000014091A5BA: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014091A5BF: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014091A5C4: 48 81 EC A8 00 00  sub         rsp,0A8h
                    00
  000000014091A5CB: 48 C7 44 24 78 FE  mov         qword ptr [rsp+78h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000014091A5D4: 48 8D 44 24 48     lea         rax,[rsp+48h]
  000000014091A5D9: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  000000014091A5DE: 48 8B 94 24 B8 00  mov         rdx,qword ptr [rsp+00000000000000B8h]
                    00 00
  000000014091A5E6: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014091A5EB: FF 15 E7 B3 4E 01  call        qword ptr [0000000141E059D8h]
  000000014091A5F1: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  000000014091A5F9: 48 8B 84 24 B0 00  mov         rax,qword ptr [rsp+00000000000000B0h]
                    00 00
  000000014091A601: 48 8B 00           mov         rax,qword ptr [rax]
  000000014091A604: 48 8B 94 24 80 00  mov         rdx,qword ptr [rsp+0000000000000080h]
                    00 00
  000000014091A60C: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  000000014091A614: FF 90 30 01 00 00  call        qword ptr [rax+0000000000000130h]
  000000014091A61A: 0F B6 C0           movzx       eax,al
  000000014091A61D: 85 C0              test        eax,eax
  000000014091A61F: 0F 84 DC 00 00 00  je          000000014091A701
  000000014091A625: 48 8D 44 24 58     lea         rax,[rsp+58h]
  000000014091A62A: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  000000014091A62F: 48 8B 94 24 B8 00  mov         rdx,qword ptr [rsp+00000000000000B8h]
                    00 00
  000000014091A637: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000014091A63C: FF 15 96 B3 4E 01  call        qword ptr [0000000141E059D8h]
  000000014091A642: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  000000014091A64A: 48 8B 84 24 B0 00  mov         rax,qword ptr [rsp+00000000000000B0h]
                    00 00
  000000014091A652: 48 8B 00           mov         rax,qword ptr [rax]
  000000014091A655: 48 8B 94 24 88 00  mov         rdx,qword ptr [rsp+0000000000000088h]
                    00 00
  000000014091A65D: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  000000014091A665: FF 90 80 01 00 00  call        qword ptr [rax+0000000000000180h]
  000000014091A66B: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  000000014091A673: 48 83 C1 38        add         rcx,38h
  000000014091A677: 8B D0              mov         edx,eax
  000000014091A679: E8 92 65 84 FF     call        0000000140160C10
  000000014091A67E: 48 8B 00           mov         rax,qword ptr [rax]
  000000014091A681: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  000000014091A686: 48 8D 44 24 68     lea         rax,[rsp+68h]
  000000014091A68B: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  000000014091A690: 48 8B 94 24 C8 00  mov         rdx,qword ptr [rsp+00000000000000C8h]
                    00 00
  000000014091A698: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  000000014091A69D: FF 15 35 B3 4E 01  call        qword ptr [0000000141E059D8h]
  000000014091A6A3: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  000000014091A6AB: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  000000014091A6B3: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  000000014091A6B8: 48 8B 84 24 E8 00  mov         rax,qword ptr [rsp+00000000000000E8h]
                    00 00
  000000014091A6C0: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014091A6C5: F2 0F 10 84 24 E0  movsd       xmm0,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  000000014091A6CE: F2 0F 11 44 24 28  movsd       mmword ptr [rsp+28h],xmm0
  000000014091A6D4: 8B 84 24 D8 00 00  mov         eax,dword ptr [rsp+00000000000000D8h]
                    00
  000000014091A6DB: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  000000014091A6DF: 44 8B 8C 24 D0 00  mov         r9d,dword ptr [rsp+00000000000000D0h]
                    00 00
  000000014091A6E7: 4C 8B 84 24 90 00  mov         r8,qword ptr [rsp+0000000000000090h]
                    00 00
  000000014091A6EF: 8B 94 24 C0 00 00  mov         edx,dword ptr [rsp+00000000000000C0h]
                    00
  000000014091A6F6: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014091A6FB: E8 40 C4 EF FF     call        0000000140816B40
  000000014091A700: 90                 nop
  000000014091A701: 48 8B 8C 24 B8 00  mov         rcx,qword ptr [rsp+00000000000000B8h]
                    00 00
  000000014091A709: FF 15 D1 B2 4E 01  call        qword ptr [0000000141E059E0h]
  000000014091A70F: 90                 nop
  000000014091A710: 48 8B 8C 24 C8 00  mov         rcx,qword ptr [rsp+00000000000000C8h]
                    00 00
  000000014091A718: FF 15 C2 B2 4E 01  call        qword ptr [0000000141E059E0h]
  000000014091A71E: 48 81 C4 A8 00 00  add         rsp,0A8h
                    00
  000000014091A725: C3                 ret

  Summary

     1E02000 .text

RANGE 0x140898f40..0x140899002 SHA256 dfa2b2eb9aac2d1f81e84941d4ccb986b64325928f0c94b2dad1d905146972da
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140898F40: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140898F45: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140898F4A: 48 83 EC 38        sub         rsp,38h
  0000000140898F4E: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140898F53: C7 40 58 00 00 00  mov         dword ptr [rax+58h],0
                    00
  0000000140898F5A: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140898F62: EB 0B              jmp         0000000140898F6F
  0000000140898F64: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  0000000140898F68: 83 C0 01           add         eax,1
  0000000140898F6B: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  0000000140898F6F: 83 7C 24 20 03     cmp         dword ptr [rsp+20h],3
  0000000140898F74: 7D 26              jge         0000000140898F9C
  0000000140898F76: 48 63 4C 24 20     movsxd      rcx,dword ptr [rsp+20h]
  0000000140898F7B: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140898F80: C7 44 88 10 00 00  mov         dword ptr [rax+rcx*4+10h],0
                    00 00
  0000000140898F88: 48 63 4C 24 20     movsxd      rcx,dword ptr [rsp+20h]
  0000000140898F8D: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140898F92: C7 44 88 1C 00 00  mov         dword ptr [rax+rcx*4+1Ch],0
                    00 00
  0000000140898F9A: EB C8              jmp         0000000140898F64
  0000000140898F9C: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140898FA1: 48 C7 40 48 00 00  mov         qword ptr [rax+48h],0
                    00 00
  0000000140898FA9: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140898FAE: 48 C7 40 38 00 00  mov         qword ptr [rax+38h],0
                    00 00
  0000000140898FB6: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140898FBB: 48 C7 40 30 00 00  mov         qword ptr [rax+30h],0
                    00 00
  0000000140898FC3: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140898FC8: 48 C7 40 28 00 00  mov         qword ptr [rax+28h],0
                    00 00
  0000000140898FD0: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140898FD5: 48 C7 40 68 00 00  mov         qword ptr [rax+68h],0
                    00 00
  0000000140898FDD: 33 D2              xor         edx,edx
  0000000140898FDF: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140898FE4: E8 27 00 00 00     call        0000000140899010
  0000000140898FE9: 48 8B 54 24 48     mov         rdx,qword ptr [rsp+48h]
  0000000140898FEE: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140898FF3: E8 68 1D 00 00     call        000000014089AD60
  0000000140898FF8: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140898FFD: 48 83 C4 38        add         rsp,38h
  0000000140899001: C3                 ret

  Summary

     1E02000 .text

RANGE 0x14089ad60..0x14089af17 SHA256 43651a213052f75cb536e53f21faafe653326ddd1a89765905303b91b1e4df78
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014089AD60: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014089AD65: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014089AD6A: 48 83 EC 38        sub         rsp,38h
  000000014089AD6E: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014089AD73: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  000000014089AD78: 8B 00              mov         eax,dword ptr [rax]
  000000014089AD7A: 89 01              mov         dword ptr [rcx],eax
  000000014089AD7C: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089AD81: 81 38 80 00 00 00  cmp         dword ptr [rax],80h
  000000014089AD87: 75 10              jne         000000014089AD99
  000000014089AD89: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014089AD8E: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  000000014089AD93: 8B 40 58           mov         eax,dword ptr [rax+58h]
  000000014089AD96: 89 41 58           mov         dword ptr [rcx+58h],eax
  000000014089AD99: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014089AD9E: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  000000014089ADA3: F2 0F 10 40 08     movsd       xmm0,mmword ptr [rax+8]
  000000014089ADA8: F2 0F 11 41 08     movsd       mmword ptr [rcx+8],xmm0
  000000014089ADAD: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014089ADB2: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  000000014089ADB7: 8B 40 54           mov         eax,dword ptr [rax+54h]
  000000014089ADBA: 89 41 54           mov         dword ptr [rcx+54h],eax
  000000014089ADBD: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014089ADC2: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  000000014089ADC7: 8B 40 04           mov         eax,dword ptr [rax+4]
  000000014089ADCA: 89 41 04           mov         dword ptr [rcx+4],eax
  000000014089ADCD: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014089ADD2: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  000000014089ADD7: 8B 40 40           mov         eax,dword ptr [rax+40h]
  000000014089ADDA: 89 41 40           mov         dword ptr [rcx+40h],eax
  000000014089ADDD: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014089ADE2: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  000000014089ADE7: 8B 40 50           mov         eax,dword ptr [rax+50h]
  000000014089ADEA: 89 41 50           mov         dword ptr [rcx+50h],eax
  000000014089ADED: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  000000014089ADF5: EB 0B              jmp         000000014089AE02
  000000014089ADF7: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  000000014089ADFB: 83 C0 01           add         eax,1
  000000014089ADFE: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  000000014089AE02: 83 7C 24 20 03     cmp         dword ptr [rsp+20h],3
  000000014089AE07: 7D 3C              jge         000000014089AE45
  000000014089AE09: 4C 63 44 24 20     movsxd      r8,dword ptr [rsp+20h]
  000000014089AE0E: 48 63 54 24 20     movsxd      rdx,dword ptr [rsp+20h]
  000000014089AE13: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014089AE18: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  000000014089AE1D: 42 8B 44 80 10     mov         eax,dword ptr [rax+r8*4+10h]
  000000014089AE22: 89 44 91 10        mov         dword ptr [rcx+rdx*4+10h],eax
  000000014089AE26: 4C 63 44 24 20     movsxd      r8,dword ptr [rsp+20h]
  000000014089AE2B: 48 63 54 24 20     movsxd      rdx,dword ptr [rsp+20h]
  000000014089AE30: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014089AE35: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  000000014089AE3A: 42 8B 44 80 1C     mov         eax,dword ptr [rax+r8*4+1Ch]
  000000014089AE3F: 89 44 91 1C        mov         dword ptr [rcx+rdx*4+1Ch],eax
  000000014089AE43: EB B2              jmp         000000014089ADF7
  000000014089AE45: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014089AE4A: E8 F1 09 00 00     call        000000014089B840
  000000014089AE4F: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  000000014089AE57: EB 0B              jmp         000000014089AE64
  000000014089AE59: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  000000014089AE5D: 83 C0 01           add         eax,1
  000000014089AE60: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  000000014089AE64: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089AE69: 8B 40 04           mov         eax,dword ptr [rax+4]
  000000014089AE6C: 39 44 24 20        cmp         dword ptr [rsp+20h],eax
  000000014089AE70: 7D 5E              jge         000000014089AED0
  000000014089AE72: 48 63 4C 24 20     movsxd      rcx,dword ptr [rsp+20h]
  000000014089AE77: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089AE7C: 48 83 7C C8 28 00  cmp         qword ptr [rax+rcx*8+28h],0
  000000014089AE82: 74 12              je          000000014089AE96
  000000014089AE84: 48 63 4C 24 20     movsxd      rcx,dword ptr [rsp+20h]
  000000014089AE89: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  000000014089AE8E: 48 83 7C C8 28 00  cmp         qword ptr [rax+rcx*8+28h],0
  000000014089AE94: 75 02              jne         000000014089AE98
  000000014089AE96: EB 38              jmp         000000014089AED0
  000000014089AE98: 48 63 4C 24 20     movsxd      rcx,dword ptr [rsp+20h]
  000000014089AE9D: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089AEA2: 4C 63 44 88 1C     movsxd      r8,dword ptr [rax+rcx*4+1Ch]
  000000014089AEA7: 49 C1 E0 03        shl         r8,3
  000000014089AEAB: 48 63 4C 24 20     movsxd      rcx,dword ptr [rsp+20h]
  000000014089AEB0: 48 63 44 24 20     movsxd      rax,dword ptr [rsp+20h]
  000000014089AEB5: 48 8B 54 24 48     mov         rdx,qword ptr [rsp+48h]
  000000014089AEBA: 48 8B 54 CA 28     mov         rdx,qword ptr [rdx+rcx*8+28h]
  000000014089AEBF: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014089AEC4: 48 8B 4C C1 28     mov         rcx,qword ptr [rcx+rax*8+28h]
  000000014089AEC9: E8 32 42 BD 00     call        000000014146F100
  000000014089AECE: EB 89              jmp         000000014089AE59
  000000014089AED0: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089AED5: 48 83 78 48 00     cmp         qword ptr [rax+48h],0
  000000014089AEDA: 74 0C              je          000000014089AEE8
  000000014089AEDC: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  000000014089AEE1: 48 83 78 48 00     cmp         qword ptr [rax+48h],0
  000000014089AEE6: 75 02              jne         000000014089AEEA
  000000014089AEE8: EB 28              jmp         000000014089AF12
  000000014089AEEA: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014089AEEF: E8 9C 0D 00 00     call        000000014089BC90
  000000014089AEF4: 4C 63 C0           movsxd      r8,eax
  000000014089AEF7: 49 C1 E0 03        shl         r8,3
  000000014089AEFB: 48 8B 54 24 48     mov         rdx,qword ptr [rsp+48h]
  000000014089AF00: 48 8B 52 48        mov         rdx,qword ptr [rdx+48h]
  000000014089AF04: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014089AF09: 48 8B 49 48        mov         rcx,qword ptr [rcx+48h]
  000000014089AF0D: E8 EE 41 BD 00     call        000000014146F100
  000000014089AF12: 48 83 C4 38        add         rsp,38h
  000000014089AF16: C3                 ret

  Summary

     1E02000 .text

RANGE 0x140b51140..0x140b5120d SHA256 097d1beb4fff7d86194a8a0e937f8a7a4c72625872cd533ae9940bda206a7869
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140B51140: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  0000000140B51145: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  0000000140B5114A: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140B5114F: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140B51154: 48 83 EC 48        sub         rsp,48h
  0000000140B51158: 33 D2              xor         edx,edx
  0000000140B5115A: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140B5115F: E8 BC AB 51 FF     call        000000014006BD20
  0000000140B51164: 8B 00              mov         eax,dword ptr [rax]
  0000000140B51166: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  0000000140B5116A: 83 7C 24 20 01     cmp         dword ptr [rsp+20h],1
  0000000140B5116F: 7D 4B              jge         0000000140B511BC
  0000000140B51171: BA 01 00 00 00     mov         edx,1
  0000000140B51176: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140B5117B: E8 A0 AB 51 FF     call        000000014006BD20
  0000000140B51180: 8B 00              mov         eax,dword ptr [rax]
  0000000140B51182: 89 44 24 34        mov         dword ptr [rsp+34h],eax
  0000000140B51186: BA 02 00 00 00     mov         edx,2
  0000000140B5118B: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140B51190: E8 8B AB 51 FF     call        000000014006BD20
  0000000140B51195: 8B 00              mov         eax,dword ptr [rax]
  0000000140B51197: 89 44 24 30        mov         dword ptr [rsp+30h],eax
  0000000140B5119B: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140B511A0: 48 81 C1 C8 13 00  add         rcx,13C8h
                    00
  0000000140B511A7: 44 8B 44 24 30     mov         r8d,dword ptr [rsp+30h]
  0000000140B511AC: 8B 54 24 34        mov         edx,dword ptr [rsp+34h]
  0000000140B511B0: E8 1B 5A D9 FF     call        00000001408E6BD0
  0000000140B511B5: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140B511BA: EB 1A              jmp         0000000140B511D6
  0000000140B511BC: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140B511C1: 48 81 C1 C8 13 00  add         rcx,13C8h
                    00
  0000000140B511C8: 8B 54 24 20        mov         edx,dword ptr [rsp+20h]
  0000000140B511CC: E8 9F 59 D9 FF     call        00000001408E6B70
  0000000140B511D1: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140B511D6: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  0000000140B511DC: 74 14              je          0000000140B511F2
  0000000140B511DE: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140B511E3: 48 81 C1 40 02 00  add         rcx,240h
                    00
  0000000140B511EA: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  0000000140B511EF: 48 89 08           mov         qword ptr [rax],rcx
  0000000140B511F2: C7 44 24 38 00 00  mov         dword ptr [rsp+38h],0
                    00 00
  0000000140B511FA: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140B511FF: E8 EC B4 D9 FF     call        00000001408EC6F0
  0000000140B51204: 8B 44 24 38        mov         eax,dword ptr [rsp+38h]
  0000000140B51208: 48 83 C4 48        add         rsp,48h
  0000000140B5120C: C3                 ret

  Summary

     1E02000 .text

RANGE 0x14089b180..0x14089b1e3 SHA256 6039b6294d2d54702b92b2f57878f0d0694da266317cbdc1e2e019ffd53db469
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014089B180: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014089B185: 48 83 EC 48        sub         rsp,48h
  000000014089B189: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014089B18E: E8 5D 00 00 00     call        000000014089B1F0
  000000014089B193: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  000000014089B198: 48 83 78 68 00     cmp         qword ptr [rax+68h],0
  000000014089B19D: 74 3F              je          000000014089B1DE
  000000014089B19F: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  000000014089B1A4: 48 8B 40 68        mov         rax,qword ptr [rax+68h]
  000000014089B1A8: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000014089B1AD: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  000000014089B1B2: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014089B1B7: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  000000014089B1BD: 74 16              je          000000014089B1D5
  000000014089B1BF: BA 01 00 00 00     mov         edx,1
  000000014089B1C4: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000014089B1C9: E8 02 58 86 FF     call        00000001401009D0
  000000014089B1CE: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014089B1D3: EB 09              jmp         000000014089B1DE
  000000014089B1D5: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  000000014089B1DE: 48 83 C4 48        add         rsp,48h
  000000014089B1E2: C3                 ret

  Summary

     1E02000 .text

RANGE 0x1400d0ff0..0x1400d10bb SHA256 3bc6e9736b47707cebe3d1acc454ef912d85acc10ac1cf4f7aa18b1c4e1a1bb3
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400D0FF0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001400D0FF5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400D0FFA: 53                 push        rbx
  00000001400D0FFB: 48 83 EC 40        sub         rsp,40h
  00000001400D0FFF: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001400D1004: FF 15 06 32 D3 01  call        qword ptr [0000000141E04210h]
  00000001400D100A: 8B D8              mov         ebx,eax
  00000001400D100C: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  00000001400D1011: FF 15 F9 31 D3 01  call        qword ptr [0000000141E04210h]
  00000001400D1017: 3B D8              cmp         ebx,eax
  00000001400D1019: 74 07              je          00000001400D1022
  00000001400D101B: 32 C0              xor         al,al
  00000001400D101D: E9 93 00 00 00     jmp         00000001400D10B5
  00000001400D1022: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001400D1027: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  00000001400D102C: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400D102F: 48 39 01           cmp         qword ptr [rcx],rax
  00000001400D1032: 75 04              jne         00000001400D1038
  00000001400D1034: B0 01              mov         al,1
  00000001400D1036: EB 7D              jmp         00000001400D10B5
  00000001400D1038: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001400D103D: FF 15 AD 31 D3 01  call        qword ptr [0000000141E041F0h]
  00000001400D1043: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001400D1048: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001400D104D: FF 15 8D 31 D3 01  call        qword ptr [0000000141E041E0h]
  00000001400D1053: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001400D1058: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  00000001400D105D: FF 15 8D 31 D3 01  call        qword ptr [0000000141E041F0h]
  00000001400D1063: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001400D1068: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  00000001400D106D: 48 39 44 24 20     cmp         qword ptr [rsp+20h],rax
  00000001400D1072: 74 3F              je          00000001400D10B3
  00000001400D1074: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000001400D1079: 48 83 E8 08        sub         rax,8
  00000001400D107D: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001400D1082: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001400D1087: 48 83 E8 08        sub         rax,8
  00000001400D108B: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001400D1090: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001400D1095: E8 36 76 2B 01     call        00000001413886D0
  00000001400D109A: 48 8B D8           mov         rbx,rax
  00000001400D109D: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001400D10A2: E8 29 76 2B 01     call        00000001413886D0
  00000001400D10A7: 8B 00              mov         eax,dword ptr [rax]
  00000001400D10A9: 39 03              cmp         dword ptr [rbx],eax
  00000001400D10AB: 74 04              je          00000001400D10B1
  00000001400D10AD: 32 C0              xor         al,al
  00000001400D10AF: EB 04              jmp         00000001400D10B5
  00000001400D10B1: EB B5              jmp         00000001400D1068
  00000001400D10B3: B0 01              mov         al,1
  00000001400D10B5: 48 83 C4 40        add         rsp,40h
  00000001400D10B9: 5B                 pop         rbx
  00000001400D10BA: C3                 ret

  Summary

     1E02000 .text

RANGE 0x140922160..0x1409221c4 SHA256 5aa57428dbfdd268ebceb64ad653c0d319cbb7719714c1a31f1e3225e66194b1
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140922160: 89 4C 24 08        mov         dword ptr [rsp+8],ecx
  0000000140922164: 48 83 EC 18        sub         rsp,18h
  0000000140922168: C7 04 24 30 01 00  mov         dword ptr [rsp],130h
                    00
  000000014092216F: C7 44 24 04 00 00  mov         dword ptr [rsp+4],0
                    00 00
  0000000140922177: EB 0B              jmp         0000000140922184
  0000000140922179: 8B 44 24 04        mov         eax,dword ptr [rsp+4]
  000000014092217D: 83 C0 01           add         eax,1
  0000000140922180: 89 44 24 04        mov         dword ptr [rsp+4],eax
  0000000140922184: 8B 04 24           mov         eax,dword ptr [rsp]
  0000000140922187: 39 44 24 04        cmp         dword ptr [rsp+4],eax
  000000014092218B: 7D 30              jge         00000001409221BD
  000000014092218D: 48 63 4C 24 04     movsxd      rcx,dword ptr [rsp+4]
  0000000140922192: 48 6B C9 30        imul        rcx,rcx,30h
  0000000140922196: 48 8D 05 43 E5 E9  lea         rax,[00000001427C06E0h]
                    01
  000000014092219D: 8B 04 08           mov         eax,dword ptr [rax+rcx]
  00000001409221A0: 39 44 24 20        cmp         dword ptr [rsp+20h],eax
  00000001409221A4: 75 15              jne         00000001409221BB
  00000001409221A6: 48 63 4C 24 04     movsxd      rcx,dword ptr [rsp+4]
  00000001409221AB: 48 6B C9 30        imul        rcx,rcx,30h
  00000001409221AF: 48 8D 05 2A E5 E9  lea         rax,[00000001427C06E0h]
                    01
  00000001409221B6: 48 03 C1           add         rax,rcx
  00000001409221B9: EB 04              jmp         00000001409221BF
  00000001409221BB: EB BC              jmp         0000000140922179
  00000001409221BD: 33 C0              xor         eax,eax
  00000001409221BF: 48 83 C4 18        add         rsp,18h
  00000001409221C3: C3                 ret

  Summary

     1E02000 .text

RANGE 0x140916530..0x140917055 SHA256 43aef4d26f957c36cc8858d3b46e67eb54d62b82eaf324fd1047ee153fcc7110
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140916530: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  0000000140916535: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  000000014091653A: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014091653F: 89 4C 24 08        mov         dword ptr [rsp+8],ecx
  0000000140916543: 48 81 EC B8 02 00  sub         rsp,2B8h
                    00
  000000014091654A: 48 C7 84 24 28 02  mov         qword ptr [rsp+0000000000000228h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  0000000140916556: 48 8B 05 D3 7A 24  mov         rax,qword ptr [0000000143B5E030h]
                    03
  000000014091655D: 48 33 C4           xor         rax,rsp
  0000000140916560: 48 89 84 24 A8 02  mov         qword ptr [rsp+00000000000002A8h],rax
                    00 00
  0000000140916568: C7 44 24 40 FF FF  mov         dword ptr [rsp+40h],0FFFFFFFFh
                    FF FF
  0000000140916570: 48 8D 15 B1 54 EA  lea         rdx,[00000001427BBA28h]
                    01
  0000000140916577: 48 8D 8C 24 28 01  lea         rcx,[rsp+0000000000000128h]
                    00 00
  000000014091657F: FF 15 FB DE 4E 01  call        qword ptr [0000000141E04480h]
  0000000140916585: 90                 nop
  0000000140916586: 48 8D 94 24 28 01  lea         rdx,[rsp+0000000000000128h]
                    00 00
  000000014091658E: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140916593: FF 15 3F DA 4E 01  call        qword ptr [0000000141E03FD8h]
  0000000140916599: 90                 nop
  000000014091659A: 48 8D 8C 24 28 01  lea         rcx,[rsp+0000000000000128h]
                    00 00
  00000001409165A2: FF 15 38 F4 4E 01  call        qword ptr [0000000141E059E0h]
  00000001409165A8: 48 8D 84 24 30 01  lea         rax,[rsp+0000000000000130h]
                    00 00
  00000001409165B0: 48 89 84 24 38 01  mov         qword ptr [rsp+0000000000000138h],rax
                    00 00
  00000001409165B8: 41 B8 10 00 00 00  mov         r8d,10h
  00000001409165BE: BA 02 00 00 00     mov         edx,2
  00000001409165C3: 48 8B 8C 24 38 01  mov         rcx,qword ptr [rsp+0000000000000138h]
                    00 00
  00000001409165CB: E8 B0 B0 71 FF     call        0000000140031680
  00000001409165D0: 48 8B D0           mov         rdx,rax
  00000001409165D3: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001409165D8: FF 15 02 DA 4E 01  call        qword ptr [0000000141E03FE0h]
  00000001409165DE: 0F B6 C0           movzx       eax,al
  00000001409165E1: 85 C0              test        eax,eax
  00000001409165E3: 0F 84 29 0A 00 00  je          0000000140917012
  00000001409165E9: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  00000001409165EE: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  00000001409165F3: FF 15 DF D7 4E 01  call        qword ptr [0000000141E03DD8h]
  00000001409165F9: 90                 nop
  00000001409165FA: 48 8B 94 24 E0 02  mov         rdx,qword ptr [rsp+00000000000002E0h]
                    00 00
  0000000140916602: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  0000000140916607: FF 15 E3 D7 4E 01  call        qword ptr [0000000141E03DF0h]
  000000014091660D: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140916612: FF 15 00 DA 4E 01  call        qword ptr [0000000141E04018h]
  0000000140916618: B9 08 34 00 00     mov         ecx,3408h
  000000014091661D: E8 9A 84 B5 00     call        000000014146EABC
  0000000140916622: 48 89 84 24 48 01  mov         qword ptr [rsp+0000000000000148h],rax
                    00 00
  000000014091662A: 48 83 BC 24 48 01  cmp         qword ptr [rsp+0000000000000148h],0
                    00 00 00
  0000000140916633: 74 1E              je          0000000140916653
  0000000140916635: 8B 94 24 D0 02 00  mov         edx,dword ptr [rsp+00000000000002D0h]
                    00
  000000014091663C: 48 8B 8C 24 48 01  mov         rcx,qword ptr [rsp+0000000000000148h]
                    00 00
  0000000140916644: E8 A7 B5 F0 FF     call        0000000140821BF0
  0000000140916649: 48 89 84 24 30 02  mov         qword ptr [rsp+0000000000000230h],rax
                    00 00
  0000000140916651: EB 0C              jmp         000000014091665F
  0000000140916653: 48 C7 84 24 30 02  mov         qword ptr [rsp+0000000000000230h],0
                    00 00 00 00 00 00
  000000014091665F: 48 8B 84 24 30 02  mov         rax,qword ptr [rsp+0000000000000230h]
                    00 00
  0000000140916667: 48 89 84 24 40 01  mov         qword ptr [rsp+0000000000000140h],rax
                    00 00
  000000014091666F: 48 8B 84 24 40 01  mov         rax,qword ptr [rsp+0000000000000140h]
                    00 00
  0000000140916677: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  000000014091667C: 48 8D 15 A1 52 EA  lea         rdx,[00000001427BB924h]
                    01
  0000000140916683: 48 8D 0D AE 53 EA  lea         rcx,[00000001427BBA38h]
                    01
  000000014091668A: E8 49 96 B5 00     call        000000014146FCD8
  000000014091668F: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140916694: 48 83 7C 24 58 00  cmp         qword ptr [rsp+58h],0
  000000014091669A: 0F 84 17 09 00 00  je          0000000140916FB7
  00000001409166A0: 4C 8D 44 24 70     lea         r8,[rsp+70h]
  00000001409166A5: 48 8D 15 98 53 EA  lea         rdx,[00000001427BBA44h]
                    01
  00000001409166AC: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  00000001409166B1: E8 12 C4 B5 00     call        0000000141472AC8
  00000001409166B6: 8B 8C 24 C0 02 00  mov         ecx,dword ptr [rsp+00000000000002C0h]
                    00
  00000001409166BD: E8 CE B9 00 00     call        0000000140922090
  00000001409166C2: 89 84 24 38 02 00  mov         dword ptr [rsp+0000000000000238h],eax
                    00
  00000001409166C9: 83 BC 24 38 02 00  cmp         dword ptr [rsp+0000000000000238h],1
                    00 01
  00000001409166D1: 74 31              je          0000000140916704
  00000001409166D3: 83 BC 24 38 02 00  cmp         dword ptr [rsp+0000000000000238h],2
                    00 02
  00000001409166DB: 74 47              je          0000000140916724
  00000001409166DD: 83 BC 24 38 02 00  cmp         dword ptr [rsp+0000000000000238h],3
                    00 03
  00000001409166E5: 74 5D              je          0000000140916744
  00000001409166E7: 83 BC 24 38 02 00  cmp         dword ptr [rsp+0000000000000238h],4
                    00 04
  00000001409166EF: 74 73              je          0000000140916764
  00000001409166F1: 83 BC 24 38 02 00  cmp         dword ptr [rsp+0000000000000238h],5
                    00 05
  00000001409166F9: 0F 84 85 00 00 00  je          0000000140916784
  00000001409166FF: E9 9E 00 00 00     jmp         00000001409167A2
  0000000140916704: 44 8B 84 24 C0 02  mov         r8d,dword ptr [rsp+00000000000002C0h]
                    00 00
  000000014091670C: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  0000000140916711: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140916716: E8 75 29 25 00     call        0000000140B69090
  000000014091671B: 89 84 24 98 00 00  mov         dword ptr [rsp+0000000000000098h],eax
                    00
  0000000140916722: EB 7E              jmp         00000001409167A2
  0000000140916724: 44 8B 84 24 C0 02  mov         r8d,dword ptr [rsp+00000000000002C0h]
                    00 00
  000000014091672C: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  0000000140916731: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140916736: E8 65 AF 18 00     call        0000000140AA16A0
  000000014091673B: 89 84 24 98 00 00  mov         dword ptr [rsp+0000000000000098h],eax
                    00
  0000000140916742: EB 5E              jmp         00000001409167A2
  0000000140916744: 44 8B 84 24 C0 02  mov         r8d,dword ptr [rsp+00000000000002C0h]
                    00 00
  000000014091674C: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  0000000140916751: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140916756: E8 45 00 25 00     call        0000000140B667A0
  000000014091675B: 89 84 24 98 00 00  mov         dword ptr [rsp+0000000000000098h],eax
                    00
  0000000140916762: EB 3E              jmp         00000001409167A2
  0000000140916764: 44 8B 84 24 C0 02  mov         r8d,dword ptr [rsp+00000000000002C0h]
                    00 00
  000000014091676C: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  0000000140916771: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140916776: E8 25 BB 24 00     call        0000000140B622A0
  000000014091677B: 89 84 24 98 00 00  mov         dword ptr [rsp+0000000000000098h],eax
                    00
  0000000140916782: EB 1E              jmp         00000001409167A2
  0000000140916784: 44 8B 84 24 C0 02  mov         r8d,dword ptr [rsp+00000000000002C0h]
                    00 00
  000000014091678C: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  0000000140916791: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140916796: E8 D5 A1 24 00     call        0000000140B60970
  000000014091679B: 89 84 24 98 00 00  mov         dword ptr [rsp+0000000000000098h],eax
                    00
  00000001409167A2: 83 BC 24 98 00 00  cmp         dword ptr [rsp+0000000000000098h],0
                    00 00
  00000001409167AA: 0F 85 FD 07 00 00  jne         0000000140916FAD
  00000001409167B0: 48 8D 84 24 50 01  lea         rax,[rsp+0000000000000150h]
                    00 00
  00000001409167B8: 48 89 84 24 58 01  mov         qword ptr [rsp+0000000000000158h],rax
                    00 00
  00000001409167C0: 41 B8 10 00 00 00  mov         r8d,10h
  00000001409167C6: BA 02 00 00 00     mov         edx,2
  00000001409167CB: 48 8B 8C 24 58 01  mov         rcx,qword ptr [rsp+0000000000000158h]
                    00 00
  00000001409167D3: E8 A8 AE 71 FF     call        0000000140031680
  00000001409167D8: 48 8B D0           mov         rdx,rax
  00000001409167DB: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001409167E0: FF 15 FA D7 4E 01  call        qword ptr [0000000141E03FE0h]
  00000001409167E6: 0F B6 C0           movzx       eax,al
  00000001409167E9: 85 C0              test        eax,eax
  00000001409167EB: 0F 84 BC 07 00 00  je          0000000140916FAD
  00000001409167F1: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  00000001409167F6: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  00000001409167FE: FF 15 D4 D5 4E 01  call        qword ptr [0000000141E03DD8h]
  0000000140916804: 90                 nop
  0000000140916805: 48 8B 94 24 E8 02  mov         rdx,qword ptr [rsp+00000000000002E8h]
                    00 00
  000000014091680D: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  0000000140916815: FF 15 D5 D5 4E 01  call        qword ptr [0000000141E03DF0h]
  000000014091681B: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140916820: FF 15 F2 D7 4E 01  call        qword ptr [0000000141E04018h]
  0000000140916826: B9 08 34 00 00     mov         ecx,3408h
  000000014091682B: E8 8C 82 B5 00     call        000000014146EABC
  0000000140916830: 48 89 84 24 68 01  mov         qword ptr [rsp+0000000000000168h],rax
                    00 00
  0000000140916838: 48 83 BC 24 68 01  cmp         qword ptr [rsp+0000000000000168h],0
                    00 00 00
  0000000140916841: 74 1E              je          0000000140916861
  0000000140916843: 8B 94 24 D0 02 00  mov         edx,dword ptr [rsp+00000000000002D0h]
                    00
  000000014091684A: 48 8B 8C 24 68 01  mov         rcx,qword ptr [rsp+0000000000000168h]
                    00 00
  0000000140916852: E8 99 B3 F0 FF     call        0000000140821BF0
  0000000140916857: 48 89 84 24 40 02  mov         qword ptr [rsp+0000000000000240h],rax
                    00 00
  000000014091685F: EB 0C              jmp         000000014091686D
  0000000140916861: 48 C7 84 24 40 02  mov         qword ptr [rsp+0000000000000240h],0
                    00 00 00 00 00 00
  000000014091686D: 48 8B 84 24 40 02  mov         rax,qword ptr [rsp+0000000000000240h]
                    00 00
  0000000140916875: 48 89 84 24 60 01  mov         qword ptr [rsp+0000000000000160h],rax
                    00 00
  000000014091687D: 48 8B 84 24 60 01  mov         rax,qword ptr [rsp+0000000000000160h]
                    00 00
  0000000140916885: 48 89 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],rax
                    00 00
  000000014091688D: 48 8D 15 F8 50 EA  lea         rdx,[00000001427BB98Ch]
                    01
  0000000140916894: 48 8D 0D B5 51 EA  lea         rcx,[00000001427BBA50h]
                    01
  000000014091689B: E8 38 94 B5 00     call        000000014146FCD8
  00000001409168A0: 48 89 84 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],rax
                    00 00
  00000001409168A8: 48 83 BC 24 B0 00  cmp         qword ptr [rsp+00000000000000B0h],0
                    00 00 00
  00000001409168B1: 0F 84 95 06 00 00  je          0000000140916F4C
  00000001409168B7: 4C 8D 84 24 C0 00  lea         r8,[rsp+00000000000000C0h]
                    00 00
  00000001409168BF: 48 8D 15 96 51 EA  lea         rdx,[00000001427BBA5Ch]
                    01
  00000001409168C6: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  00000001409168CE: E8 F5 C1 B5 00     call        0000000141472AC8
  00000001409168D3: 8B 8C 24 C0 02 00  mov         ecx,dword ptr [rsp+00000000000002C0h]
                    00
  00000001409168DA: E8 B1 B7 00 00     call        0000000140922090
  00000001409168DF: 89 84 24 48 02 00  mov         dword ptr [rsp+0000000000000248h],eax
                    00
  00000001409168E6: 83 BC 24 48 02 00  cmp         dword ptr [rsp+0000000000000248h],1
                    00 01
  00000001409168EE: 74 35              je          0000000140916925
  00000001409168F0: 83 BC 24 48 02 00  cmp         dword ptr [rsp+0000000000000248h],2
                    00 02
  00000001409168F8: 74 54              je          000000014091694E
  00000001409168FA: 83 BC 24 48 02 00  cmp         dword ptr [rsp+0000000000000248h],3
                    00 03
  0000000140916902: 74 70              je          0000000140916974
  0000000140916904: 83 BC 24 48 02 00  cmp         dword ptr [rsp+0000000000000248h],4
                    00 04
  000000014091690C: 0F 84 88 00 00 00  je          000000014091699A
  0000000140916912: 83 BC 24 48 02 00  cmp         dword ptr [rsp+0000000000000248h],5
                    00 05
  000000014091691A: 0F 84 A0 00 00 00  je          00000001409169C0
  0000000140916920: E9 BF 00 00 00     jmp         00000001409169E4
  0000000140916925: 44 8B 84 24 C0 02  mov         r8d,dword ptr [rsp+00000000000002C0h]
                    00 00
  000000014091692D: 48 8B 94 24 B0 00  mov         rdx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140916935: 48 8B 8C 24 B8 00  mov         rcx,qword ptr [rsp+00000000000000B8h]
                    00 00
  000000014091693D: E8 4E 27 25 00     call        0000000140B69090
  0000000140916942: 89 84 24 E8 00 00  mov         dword ptr [rsp+00000000000000E8h],eax
                    00
  0000000140916949: E9 96 00 00 00     jmp         00000001409169E4
  000000014091694E: 44 8B 84 24 C0 02  mov         r8d,dword ptr [rsp+00000000000002C0h]
                    00 00
  0000000140916956: 48 8B 94 24 B0 00  mov         rdx,qword ptr [rsp+00000000000000B0h]
                    00 00
  000000014091695E: 48 8B 8C 24 B8 00  mov         rcx,qword ptr [rsp+00000000000000B8h]
                    00 00
  0000000140916966: E8 35 AD 18 00     call        0000000140AA16A0
  000000014091696B: 89 84 24 E8 00 00  mov         dword ptr [rsp+00000000000000E8h],eax
                    00
  0000000140916972: EB 70              jmp         00000001409169E4
  0000000140916974: 44 8B 84 24 C0 02  mov         r8d,dword ptr [rsp+00000000000002C0h]
                    00 00
  000000014091697C: 48 8B 94 24 B0 00  mov         rdx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140916984: 48 8B 8C 24 B8 00  mov         rcx,qword ptr [rsp+00000000000000B8h]
                    00 00
  000000014091698C: E8 0F FE 24 00     call        0000000140B667A0
  0000000140916991: 89 84 24 E8 00 00  mov         dword ptr [rsp+00000000000000E8h],eax
                    00
  0000000140916998: EB 4A              jmp         00000001409169E4
  000000014091699A: 44 8B 84 24 C0 02  mov         r8d,dword ptr [rsp+00000000000002C0h]
                    00 00
  00000001409169A2: 48 8B 94 24 B0 00  mov         rdx,qword ptr [rsp+00000000000000B0h]
                    00 00
  00000001409169AA: 48 8B 8C 24 B8 00  mov         rcx,qword ptr [rsp+00000000000000B8h]
                    00 00
  00000001409169B2: E8 E9 B8 24 00     call        0000000140B622A0
  00000001409169B7: 89 84 24 E8 00 00  mov         dword ptr [rsp+00000000000000E8h],eax
                    00
  00000001409169BE: EB 24              jmp         00000001409169E4
  00000001409169C0: 44 8B 84 24 C0 02  mov         r8d,dword ptr [rsp+00000000000002C0h]
                    00 00
  00000001409169C8: 48 8B 94 24 B0 00  mov         rdx,qword ptr [rsp+00000000000000B0h]
                    00 00
  00000001409169D0: 48 8B 8C 24 B8 00  mov         rcx,qword ptr [rsp+00000000000000B8h]
                    00 00
  00000001409169D8: E8 93 9F 24 00     call        0000000140B60970
  00000001409169DD: 89 84 24 E8 00 00  mov         dword ptr [rsp+00000000000000E8h],eax
                    00
  00000001409169E4: 83 BC 24 E8 00 00  cmp         dword ptr [rsp+00000000000000E8h],0
                    00 00
  00000001409169EC: 0F 85 4D 05 00 00  jne         0000000140916F3F
  00000001409169F2: 48 8D 84 24 70 01  lea         rax,[rsp+0000000000000170h]
                    00 00
  00000001409169FA: 48 89 84 24 78 01  mov         qword ptr [rsp+0000000000000178h],rax
                    00 00
  0000000140916A02: 41 B8 10 00 00 00  mov         r8d,10h
  0000000140916A08: BA 02 00 00 00     mov         edx,2
  0000000140916A0D: 48 8B 8C 24 78 01  mov         rcx,qword ptr [rsp+0000000000000178h]
                    00 00
  0000000140916A15: E8 66 AC 71 FF     call        0000000140031680
  0000000140916A1A: 48 8B D0           mov         rdx,rax
  0000000140916A1D: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140916A22: FF 15 B8 D5 4E 01  call        qword ptr [0000000141E03FE0h]
  0000000140916A28: 0F B6 C0           movzx       eax,al
  0000000140916A2B: 85 C0              test        eax,eax
  0000000140916A2D: 0F 84 0C 05 00 00  je          0000000140916F3F
  0000000140916A33: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  0000000140916A38: 48 8D 8C 24 F8 00  lea         rcx,[rsp+00000000000000F8h]
                    00 00
  0000000140916A40: FF 15 92 D3 4E 01  call        qword ptr [0000000141E03DD8h]
  0000000140916A46: 90                 nop
  0000000140916A47: 48 8B 94 24 F0 02  mov         rdx,qword ptr [rsp+00000000000002F0h]
                    00 00
  0000000140916A4F: 48 8D 8C 24 F8 00  lea         rcx,[rsp+00000000000000F8h]
                    00 00
  0000000140916A57: FF 15 93 D3 4E 01  call        qword ptr [0000000141E03DF0h]
  0000000140916A5D: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140916A62: FF 15 B0 D5 4E 01  call        qword ptr [0000000141E04018h]
  0000000140916A68: B9 08 34 00 00     mov         ecx,3408h
  0000000140916A6D: E8 4A 80 B5 00     call        000000014146EABC
  0000000140916A72: 48 89 84 24 88 01  mov         qword ptr [rsp+0000000000000188h],rax
                    00 00
  0000000140916A7A: 48 83 BC 24 88 01  cmp         qword ptr [rsp+0000000000000188h],0
                    00 00 00
  0000000140916A83: 74 1E              je          0000000140916AA3
  0000000140916A85: 8B 94 24 D0 02 00  mov         edx,dword ptr [rsp+00000000000002D0h]
                    00
  0000000140916A8C: 48 8B 8C 24 88 01  mov         rcx,qword ptr [rsp+0000000000000188h]
                    00 00
  0000000140916A94: E8 57 B1 F0 FF     call        0000000140821BF0
  0000000140916A99: 48 89 84 24 50 02  mov         qword ptr [rsp+0000000000000250h],rax
                    00 00
  0000000140916AA1: EB 0C              jmp         0000000140916AAF
  0000000140916AA3: 48 C7 84 24 50 02  mov         qword ptr [rsp+0000000000000250h],0
                    00 00 00 00 00 00
  0000000140916AAF: 48 8B 84 24 50 02  mov         rax,qword ptr [rsp+0000000000000250h]
                    00 00
  0000000140916AB7: 48 89 84 24 80 01  mov         qword ptr [rsp+0000000000000180h],rax
                    00 00
  0000000140916ABF: 48 8B 84 24 80 01  mov         rax,qword ptr [rsp+0000000000000180h]
                    00 00
  0000000140916AC7: 48 89 84 24 F0 00  mov         qword ptr [rsp+00000000000000F0h],rax
                    00 00
  0000000140916ACF: 48 8D 15 E6 4E EA  lea         rdx,[00000001427BB9BCh]
                    01
  0000000140916AD6: 48 8D 0D 8B 4F EA  lea         rcx,[00000001427BBA68h]
                    01
  0000000140916ADD: E8 F6 91 B5 00     call        000000014146FCD8
  0000000140916AE2: 48 89 84 24 08 01  mov         qword ptr [rsp+0000000000000108h],rax
                    00 00
  0000000140916AEA: 48 83 BC 24 08 01  cmp         qword ptr [rsp+0000000000000108h],0
                    00 00 00
  0000000140916AF3: 0F 84 E5 03 00 00  je          0000000140916EDE
  0000000140916AF9: 4C 8D 84 24 C0 00  lea         r8,[rsp+00000000000000C0h]
                    00 00
  0000000140916B01: 48 8D 15 6C 4F EA  lea         rdx,[00000001427BBA74h]
                    01
  0000000140916B08: 48 8B 8C 24 08 01  mov         rcx,qword ptr [rsp+0000000000000108h]
                    00 00
  0000000140916B10: E8 B3 BF B5 00     call        0000000141472AC8
  0000000140916B15: 8B 8C 24 C0 02 00  mov         ecx,dword ptr [rsp+00000000000002C0h]
                    00
  0000000140916B1C: E8 6F B5 00 00     call        0000000140922090
  0000000140916B21: 89 84 24 58 02 00  mov         dword ptr [rsp+0000000000000258h],eax
                    00
  0000000140916B28: 83 BC 24 58 02 00  cmp         dword ptr [rsp+0000000000000258h],1
                    00 01
  0000000140916B30: 74 35              je          0000000140916B67
  0000000140916B32: 83 BC 24 58 02 00  cmp         dword ptr [rsp+0000000000000258h],2
                    00 02
  0000000140916B3A: 74 54              je          0000000140916B90
  0000000140916B3C: 83 BC 24 58 02 00  cmp         dword ptr [rsp+0000000000000258h],3
                    00 03
  0000000140916B44: 74 70              je          0000000140916BB6
  0000000140916B46: 83 BC 24 58 02 00  cmp         dword ptr [rsp+0000000000000258h],4
                    00 04
  0000000140916B4E: 0F 84 88 00 00 00  je          0000000140916BDC
  0000000140916B54: 83 BC 24 58 02 00  cmp         dword ptr [rsp+0000000000000258h],5
                    00 05
  0000000140916B5C: 0F 84 A0 00 00 00  je          0000000140916C02
  0000000140916B62: E9 BF 00 00 00     jmp         0000000140916C26
  0000000140916B67: 44 8B 84 24 C0 02  mov         r8d,dword ptr [rsp+00000000000002C0h]
                    00 00
  0000000140916B6F: 48 8B 94 24 08 01  mov         rdx,qword ptr [rsp+0000000000000108h]
                    00 00
  0000000140916B77: 48 8B 8C 24 F0 00  mov         rcx,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140916B7F: E8 0C 25 25 00     call        0000000140B69090
  0000000140916B84: 89 84 24 E8 00 00  mov         dword ptr [rsp+00000000000000E8h],eax
                    00
  0000000140916B8B: E9 96 00 00 00     jmp         0000000140916C26
  0000000140916B90: 44 8B 84 24 C0 02  mov         r8d,dword ptr [rsp+00000000000002C0h]
                    00 00
  0000000140916B98: 48 8B 94 24 08 01  mov         rdx,qword ptr [rsp+0000000000000108h]
                    00 00
  0000000140916BA0: 48 8B 8C 24 F0 00  mov         rcx,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140916BA8: E8 F3 AA 18 00     call        0000000140AA16A0
  0000000140916BAD: 89 84 24 E8 00 00  mov         dword ptr [rsp+00000000000000E8h],eax
                    00
  0000000140916BB4: EB 70              jmp         0000000140916C26
  0000000140916BB6: 44 8B 84 24 C0 02  mov         r8d,dword ptr [rsp+00000000000002C0h]
                    00 00
  0000000140916BBE: 48 8B 94 24 08 01  mov         rdx,qword ptr [rsp+0000000000000108h]
                    00 00
  0000000140916BC6: 48 8B 8C 24 F0 00  mov         rcx,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140916BCE: E8 CD FB 24 00     call        0000000140B667A0
  0000000140916BD3: 89 84 24 E8 00 00  mov         dword ptr [rsp+00000000000000E8h],eax
                    00
  0000000140916BDA: EB 4A              jmp         0000000140916C26
  0000000140916BDC: 44 8B 84 24 C0 02  mov         r8d,dword ptr [rsp+00000000000002C0h]
                    00 00
  0000000140916BE4: 48 8B 94 24 08 01  mov         rdx,qword ptr [rsp+0000000000000108h]
                    00 00
  0000000140916BEC: 48 8B 8C 24 F0 00  mov         rcx,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140916BF4: E8 A7 B6 24 00     call        0000000140B622A0
  0000000140916BF9: 89 84 24 E8 00 00  mov         dword ptr [rsp+00000000000000E8h],eax
                    00
  0000000140916C00: EB 24              jmp         0000000140916C26
  0000000140916C02: 44 8B 84 24 C0 02  mov         r8d,dword ptr [rsp+00000000000002C0h]
                    00 00
  0000000140916C0A: 48 8B 94 24 08 01  mov         rdx,qword ptr [rsp+0000000000000108h]
                    00 00
  0000000140916C12: 48 8B 8C 24 F0 00  mov         rcx,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140916C1A: E8 51 9D 24 00     call        0000000140B60970
  0000000140916C1F: 89 84 24 E8 00 00  mov         dword ptr [rsp+00000000000000E8h],eax
                    00
  0000000140916C26: 83 BC 24 E8 00 00  cmp         dword ptr [rsp+00000000000000E8h],0
                    00 00
  0000000140916C2E: 0F 85 9D 02 00 00  jne         0000000140916ED1
  0000000140916C34: 48 C7 84 24 10 01  mov         qword ptr [rsp+0000000000000110h],0
                    00 00 00 00 00 00
  0000000140916C40: 48 8D 84 24 90 01  lea         rax,[rsp+0000000000000190h]
                    00 00
  0000000140916C48: 48 89 84 24 98 01  mov         qword ptr [rsp+0000000000000198h],rax
                    00 00
  0000000140916C50: 48 8B 94 24 D8 02  mov         rdx,qword ptr [rsp+00000000000002D8h]
                    00 00
  0000000140916C58: 48 8B 8C 24 98 01  mov         rcx,qword ptr [rsp+0000000000000198h]
                    00 00
  0000000140916C60: E8 9B 0B 00 00     call        0000000140917800
  0000000140916C65: 48 89 84 24 60 02  mov         qword ptr [rsp+0000000000000260h],rax
                    00 00
  0000000140916C6D: 48 8D 84 24 10 01  lea         rax,[rsp+0000000000000110h]
                    00 00
  0000000140916C75: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140916C7A: 4C 8B 4C 24 60     mov         r9,qword ptr [rsp+60h]
  0000000140916C7F: 4C 8B 84 24 60 02  mov         r8,qword ptr [rsp+0000000000000260h]
                    00 00
  0000000140916C87: 48 8B 94 24 C8 02  mov         rdx,qword ptr [rsp+00000000000002C8h]
                    00 00
  0000000140916C8F: 8B 8C 24 C0 02 00  mov         ecx,dword ptr [rsp+00000000000002C0h]
                    00
  0000000140916C96: E8 E5 B5 00 00     call        0000000140922280
  0000000140916C9B: 48 83 BC 24 10 01  cmp         qword ptr [rsp+0000000000000110h],0
                    00 00 00
  0000000140916CA4: 74 6B              je          0000000140916D11
  0000000140916CA6: B9 78 00 00 00     mov         ecx,78h
  0000000140916CAB: E8 0C 7E B5 00     call        000000014146EABC
  0000000140916CB0: 48 89 84 24 A8 01  mov         qword ptr [rsp+00000000000001A8h],rax
                    00 00
  0000000140916CB8: 48 83 BC 24 A8 01  cmp         qword ptr [rsp+00000000000001A8h],0
                    00 00 00
  0000000140916CC1: 74 1F              je          0000000140916CE2
  0000000140916CC3: 48 8B 94 24 10 01  mov         rdx,qword ptr [rsp+0000000000000110h]
                    00 00
  0000000140916CCB: 48 8B 8C 24 A8 01  mov         rcx,qword ptr [rsp+00000000000001A8h]
                    00 00
  0000000140916CD3: E8 68 22 F8 FF     call        0000000140898F40
  0000000140916CD8: 48 89 84 24 68 02  mov         qword ptr [rsp+0000000000000268h],rax
                    00 00
  0000000140916CE0: EB 0C              jmp         0000000140916CEE
  0000000140916CE2: 48 C7 84 24 68 02  mov         qword ptr [rsp+0000000000000268h],0
                    00 00 00 00 00 00
  0000000140916CEE: 48 8B 84 24 68 02  mov         rax,qword ptr [rsp+0000000000000268h]
                    00 00
  0000000140916CF6: 48 89 84 24 A0 01  mov         qword ptr [rsp+00000000000001A0h],rax
                    00 00
  0000000140916CFE: 48 8B 8C 24 F8 02  mov         rcx,qword ptr [rsp+00000000000002F8h]
                    00 00
  0000000140916D06: 48 8B 84 24 A0 01  mov         rax,qword ptr [rsp+00000000000001A0h]
                    00 00
  0000000140916D0E: 48 89 01           mov         qword ptr [rcx],rax
  0000000140916D11: 48 C7 84 24 20 01  mov         qword ptr [rsp+0000000000000120h],0
                    00 00 00 00 00 00
  0000000140916D1D: 48 8D 84 24 B0 01  lea         rax,[rsp+00000000000001B0h]
                    00 00
  0000000140916D25: 48 89 84 24 B8 01  mov         qword ptr [rsp+00000000000001B8h],rax
                    00 00
  0000000140916D2D: 48 8B 94 24 D8 02  mov         rdx,qword ptr [rsp+00000000000002D8h]
                    00 00
  0000000140916D35: 48 8B 8C 24 B8 01  mov         rcx,qword ptr [rsp+00000000000001B8h]
                    00 00
  0000000140916D3D: E8 BE 0A 00 00     call        0000000140917800
  0000000140916D42: 48 89 84 24 70 02  mov         qword ptr [rsp+0000000000000270h],rax
                    00 00
  0000000140916D4A: 48 8D 84 24 20 01  lea         rax,[rsp+0000000000000120h]
                    00 00
  0000000140916D52: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140916D57: 4C 8B 8C 24 B8 00  mov         r9,qword ptr [rsp+00000000000000B8h]
                    00 00
  0000000140916D5F: 4C 8B 84 24 70 02  mov         r8,qword ptr [rsp+0000000000000270h]
                    00 00
  0000000140916D67: 48 8B 94 24 C8 02  mov         rdx,qword ptr [rsp+00000000000002C8h]
                    00 00
  0000000140916D6F: 8B 8C 24 C0 02 00  mov         ecx,dword ptr [rsp+00000000000002C0h]
                    00
  0000000140916D76: E8 05 B5 00 00     call        0000000140922280
  0000000140916D7B: 48 83 BC 24 20 01  cmp         qword ptr [rsp+0000000000000120h],0
                    00 00 00
  0000000140916D84: 74 6B              je          0000000140916DF1
  0000000140916D86: B9 78 00 00 00     mov         ecx,78h
  0000000140916D8B: E8 2C 7D B5 00     call        000000014146EABC
  0000000140916D90: 48 89 84 24 C8 01  mov         qword ptr [rsp+00000000000001C8h],rax
                    00 00
  0000000140916D98: 48 83 BC 24 C8 01  cmp         qword ptr [rsp+00000000000001C8h],0
                    00 00 00
  0000000140916DA1: 74 1F              je          0000000140916DC2
  0000000140916DA3: 48 8B 94 24 20 01  mov         rdx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140916DAB: 48 8B 8C 24 C8 01  mov         rcx,qword ptr [rsp+00000000000001C8h]
                    00 00
  0000000140916DB3: E8 88 21 F8 FF     call        0000000140898F40
  0000000140916DB8: 48 89 84 24 78 02  mov         qword ptr [rsp+0000000000000278h],rax
                    00 00
  0000000140916DC0: EB 0C              jmp         0000000140916DCE
  0000000140916DC2: 48 C7 84 24 78 02  mov         qword ptr [rsp+0000000000000278h],0
                    00 00 00 00 00 00
  0000000140916DCE: 48 8B 84 24 78 02  mov         rax,qword ptr [rsp+0000000000000278h]
                    00 00
  0000000140916DD6: 48 89 84 24 C0 01  mov         qword ptr [rsp+00000000000001C0h],rax
                    00 00
  0000000140916DDE: 48 8B 8C 24 00 03  mov         rcx,qword ptr [rsp+0000000000000300h]
                    00 00
  0000000140916DE6: 48 8B 84 24 C0 01  mov         rax,qword ptr [rsp+00000000000001C0h]
                    00 00
  0000000140916DEE: 48 89 01           mov         qword ptr [rcx],rax
  0000000140916DF1: 48 C7 84 24 18 01  mov         qword ptr [rsp+0000000000000118h],0
                    00 00 00 00 00 00
  0000000140916DFD: 48 8D 84 24 D0 01  lea         rax,[rsp+00000000000001D0h]
                    00 00
  0000000140916E05: 48 89 84 24 D8 01  mov         qword ptr [rsp+00000000000001D8h],rax
                    00 00
  0000000140916E0D: 48 8B 94 24 D8 02  mov         rdx,qword ptr [rsp+00000000000002D8h]
                    00 00
  0000000140916E15: 48 8B 8C 24 D8 01  mov         rcx,qword ptr [rsp+00000000000001D8h]
                    00 00
  0000000140916E1D: E8 DE 09 00 00     call        0000000140917800
  0000000140916E22: 48 89 84 24 80 02  mov         qword ptr [rsp+0000000000000280h],rax
                    00 00
  0000000140916E2A: 48 8D 84 24 18 01  lea         rax,[rsp+0000000000000118h]
                    00 00
  0000000140916E32: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140916E37: 4C 8B 8C 24 F0 00  mov         r9,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140916E3F: 4C 8B 84 24 80 02  mov         r8,qword ptr [rsp+0000000000000280h]
                    00 00
  0000000140916E47: 48 8B 94 24 C8 02  mov         rdx,qword ptr [rsp+00000000000002C8h]
                    00 00
  0000000140916E4F: 8B 8C 24 C0 02 00  mov         ecx,dword ptr [rsp+00000000000002C0h]
                    00
  0000000140916E56: E8 25 B4 00 00     call        0000000140922280
  0000000140916E5B: 48 83 BC 24 18 01  cmp         qword ptr [rsp+0000000000000118h],0
                    00 00 00
  0000000140916E64: 74 6B              je          0000000140916ED1
  0000000140916E66: B9 78 00 00 00     mov         ecx,78h
  0000000140916E6B: E8 4C 7C B5 00     call        000000014146EABC
  0000000140916E70: 48 89 84 24 E8 01  mov         qword ptr [rsp+00000000000001E8h],rax
                    00 00
  0000000140916E78: 48 83 BC 24 E8 01  cmp         qword ptr [rsp+00000000000001E8h],0
                    00 00 00
  0000000140916E81: 74 1F              je          0000000140916EA2
  0000000140916E83: 48 8B 94 24 18 01  mov         rdx,qword ptr [rsp+0000000000000118h]
                    00 00
  0000000140916E8B: 48 8B 8C 24 E8 01  mov         rcx,qword ptr [rsp+00000000000001E8h]
                    00 00
  0000000140916E93: E8 A8 20 F8 FF     call        0000000140898F40
  0000000140916E98: 48 89 84 24 88 02  mov         qword ptr [rsp+0000000000000288h],rax
                    00 00
  0000000140916EA0: EB 0C              jmp         0000000140916EAE
  0000000140916EA2: 48 C7 84 24 88 02  mov         qword ptr [rsp+0000000000000288h],0
                    00 00 00 00 00 00
  0000000140916EAE: 48 8B 84 24 88 02  mov         rax,qword ptr [rsp+0000000000000288h]
                    00 00
  0000000140916EB6: 48 89 84 24 E0 01  mov         qword ptr [rsp+00000000000001E0h],rax
                    00 00
  0000000140916EBE: 48 8B 8C 24 08 03  mov         rcx,qword ptr [rsp+0000000000000308h]
                    00 00
  0000000140916EC6: 48 8B 84 24 E0 01  mov         rax,qword ptr [rsp+00000000000001E0h]
                    00 00
  0000000140916ECE: 48 89 01           mov         qword ptr [rcx],rax
  0000000140916ED1: 48 8B 8C 24 08 01  mov         rcx,qword ptr [rsp+0000000000000108h]
                    00 00
  0000000140916ED9: E8 4A 8C B5 00     call        000000014146FB28
  0000000140916EDE: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140916EE6: 48 89 84 24 F8 01  mov         qword ptr [rsp+00000000000001F8h],rax
                    00 00
  0000000140916EEE: 48 8B 84 24 F8 01  mov         rax,qword ptr [rsp+00000000000001F8h]
                    00 00
  0000000140916EF6: 48 89 84 24 F0 01  mov         qword ptr [rsp+00000000000001F0h],rax
                    00 00
  0000000140916EFE: 48 83 BC 24 F0 01  cmp         qword ptr [rsp+00000000000001F0h],0
                    00 00 00
  0000000140916F07: 74 1C              je          0000000140916F25
  0000000140916F09: BA 01 00 00 00     mov         edx,1
  0000000140916F0E: 48 8B 8C 24 F0 01  mov         rcx,qword ptr [rsp+00000000000001F0h]
                    00 00
  0000000140916F16: E8 45 E9 74 FF     call        0000000140065860
  0000000140916F1B: 48 89 84 24 90 02  mov         qword ptr [rsp+0000000000000290h],rax
                    00 00
  0000000140916F23: EB 0C              jmp         0000000140916F31
  0000000140916F25: 48 C7 84 24 90 02  mov         qword ptr [rsp+0000000000000290h],0
                    00 00 00 00 00 00
  0000000140916F31: 48 8D 8C 24 F8 00  lea         rcx,[rsp+00000000000000F8h]
                    00 00
  0000000140916F39: FF 15 B9 CE 4E 01  call        qword ptr [0000000141E03DF8h]
  0000000140916F3F: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140916F47: E8 DC 8B B5 00     call        000000014146FB28
  0000000140916F4C: 48 8B 84 24 B8 00  mov         rax,qword ptr [rsp+00000000000000B8h]
                    00 00
  0000000140916F54: 48 89 84 24 08 02  mov         qword ptr [rsp+0000000000000208h],rax
                    00 00
  0000000140916F5C: 48 8B 84 24 08 02  mov         rax,qword ptr [rsp+0000000000000208h]
                    00 00
  0000000140916F64: 48 89 84 24 00 02  mov         qword ptr [rsp+0000000000000200h],rax
                    00 00
  0000000140916F6C: 48 83 BC 24 00 02  cmp         qword ptr [rsp+0000000000000200h],0
                    00 00 00
  0000000140916F75: 74 1C              je          0000000140916F93
  0000000140916F77: BA 01 00 00 00     mov         edx,1
  0000000140916F7C: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140916F84: E8 D7 E8 74 FF     call        0000000140065860
  0000000140916F89: 48 89 84 24 98 02  mov         qword ptr [rsp+0000000000000298h],rax
                    00 00
  0000000140916F91: EB 0C              jmp         0000000140916F9F
  0000000140916F93: 48 C7 84 24 98 02  mov         qword ptr [rsp+0000000000000298h],0
                    00 00 00 00 00 00
  0000000140916F9F: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  0000000140916FA7: FF 15 4B CE 4E 01  call        qword ptr [0000000141E03DF8h]
  0000000140916FAD: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000140916FB2: E8 71 8B B5 00     call        000000014146FB28
  0000000140916FB7: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  0000000140916FBC: 48 89 84 24 18 02  mov         qword ptr [rsp+0000000000000218h],rax
                    00 00
  0000000140916FC4: 48 8B 84 24 18 02  mov         rax,qword ptr [rsp+0000000000000218h]
                    00 00
  0000000140916FCC: 48 89 84 24 10 02  mov         qword ptr [rsp+0000000000000210h],rax
                    00 00
  0000000140916FD4: 48 83 BC 24 10 02  cmp         qword ptr [rsp+0000000000000210h],0
                    00 00 00
  0000000140916FDD: 74 1C              je          0000000140916FFB
  0000000140916FDF: BA 01 00 00 00     mov         edx,1
  0000000140916FE4: 48 8B 8C 24 10 02  mov         rcx,qword ptr [rsp+0000000000000210h]
                    00 00
  0000000140916FEC: E8 6F E8 74 FF     call        0000000140065860
  0000000140916FF1: 48 89 84 24 A0 02  mov         qword ptr [rsp+00000000000002A0h],rax
                    00 00
  0000000140916FF9: EB 0C              jmp         0000000140917007
  0000000140916FFB: 48 C7 84 24 A0 02  mov         qword ptr [rsp+00000000000002A0h],0
                    00 00 00 00 00 00
  0000000140917007: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000014091700C: FF 15 E6 CD 4E 01  call        qword ptr [0000000141E03DF8h]
  0000000140917012: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000140917016: 89 84 24 20 02 00  mov         dword ptr [rsp+0000000000000220h],eax
                    00
  000000014091701D: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140917022: FF 15 F8 CF 4E 01  call        qword ptr [0000000141E04020h]
  0000000140917028: 90                 nop
  0000000140917029: 48 8B 8C 24 D8 02  mov         rcx,qword ptr [rsp+00000000000002D8h]
                    00 00
  0000000140917031: E8 BA 56 FD FF     call        00000001408EC6F0
  0000000140917036: 8B 84 24 20 02 00  mov         eax,dword ptr [rsp+0000000000000220h]
                    00
  000000014091703D: 48 8B 8C 24 A8 02  mov         rcx,qword ptr [rsp+00000000000002A8h]
                    00 00
  0000000140917045: 48 33 CC           xor         rcx,rsp
  0000000140917048: E8 C3 6E B5 00     call        000000014146DF10
  000000014091704D: 48 81 C4 B8 02 00  add         rsp,2B8h
                    00
  0000000140917054: C3                 ret

  Summary

     1E02000 .text

RANGE 0x14089b1f0..0x14089b371 SHA256 a9f38cd2e77c249d53db072cca67bbeff194a4f72a0936bf4cb3a6748c044ef9
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014089B1F0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014089B1F5: 48 83 EC 38        sub         rsp,38h
  000000014089B1F9: C6 44 24 20 00     mov         byte ptr [rsp+20h],0
  000000014089B1FE: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B203: 83 78 1C 00        cmp         dword ptr [rax+1Ch],0
  000000014089B207: 75 11              jne         000000014089B21A
  000000014089B209: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B20E: 48 83 78 28 00     cmp         qword ptr [rax+28h],0
  000000014089B213: 74 05              je          000000014089B21A
  000000014089B215: C6 44 24 20 01     mov         byte ptr [rsp+20h],1
  000000014089B21A: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B21F: 83 78 20 00        cmp         dword ptr [rax+20h],0
  000000014089B223: 75 11              jne         000000014089B236
  000000014089B225: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B22A: 48 83 78 30 00     cmp         qword ptr [rax+30h],0
  000000014089B22F: 74 05              je          000000014089B236
  000000014089B231: C6 44 24 20 01     mov         byte ptr [rsp+20h],1
  000000014089B236: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B23B: 83 78 24 00        cmp         dword ptr [rax+24h],0
  000000014089B23F: 75 11              jne         000000014089B252
  000000014089B241: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B246: 48 83 78 38 00     cmp         qword ptr [rax+38h],0
  000000014089B24B: 74 05              je          000000014089B252
  000000014089B24D: C6 44 24 20 01     mov         byte ptr [rsp+20h],1
  000000014089B252: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  000000014089B257: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B25C: 83 78 1C 00        cmp         dword ptr [rax+1Ch],0
  000000014089B260: 7E 30              jle         000000014089B292
  000000014089B262: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  000000014089B267: 85 C0              test        eax,eax
  000000014089B269: 75 27              jne         000000014089B292
  000000014089B26B: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B270: 48 83 78 28 00     cmp         qword ptr [rax+28h],0
  000000014089B275: 74 0E              je          000000014089B285
  000000014089B277: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014089B27C: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  000000014089B280: E8 F7 68 BD 00     call        0000000141471B7C
  000000014089B285: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B28A: 48 C7 40 28 00 00  mov         qword ptr [rax+28h],0
                    00 00
  000000014089B292: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B297: 83 78 20 00        cmp         dword ptr [rax+20h],0
  000000014089B29B: 7E 30              jle         000000014089B2CD
  000000014089B29D: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  000000014089B2A2: 85 C0              test        eax,eax
  000000014089B2A4: 75 27              jne         000000014089B2CD
  000000014089B2A6: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B2AB: 48 83 78 30 00     cmp         qword ptr [rax+30h],0
  000000014089B2B0: 74 0E              je          000000014089B2C0
  000000014089B2B2: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014089B2B7: 48 8B 49 30        mov         rcx,qword ptr [rcx+30h]
  000000014089B2BB: E8 BC 68 BD 00     call        0000000141471B7C
  000000014089B2C0: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B2C5: 48 C7 40 30 00 00  mov         qword ptr [rax+30h],0
                    00 00
  000000014089B2CD: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B2D2: 83 78 24 00        cmp         dword ptr [rax+24h],0
  000000014089B2D6: 7E 30              jle         000000014089B308
  000000014089B2D8: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  000000014089B2DD: 85 C0              test        eax,eax
  000000014089B2DF: 75 27              jne         000000014089B308
  000000014089B2E1: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B2E6: 48 83 78 38 00     cmp         qword ptr [rax+38h],0
  000000014089B2EB: 74 0E              je          000000014089B2FB
  000000014089B2ED: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014089B2F2: 48 8B 49 38        mov         rcx,qword ptr [rcx+38h]
  000000014089B2F6: E8 81 68 BD 00     call        0000000141471B7C
  000000014089B2FB: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B300: 48 C7 40 38 00 00  mov         qword ptr [rax+38h],0
                    00 00
  000000014089B308: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  000000014089B30D: 85 C0              test        eax,eax
  000000014089B30F: 75 27              jne         000000014089B338
  000000014089B311: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B316: 48 83 78 48 00     cmp         qword ptr [rax+48h],0
  000000014089B31B: 74 0E              je          000000014089B32B
  000000014089B31D: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014089B322: 48 8B 49 48        mov         rcx,qword ptr [rcx+48h]
  000000014089B326: E8 51 68 BD 00     call        0000000141471B7C
  000000014089B32B: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B330: 48 C7 40 48 00 00  mov         qword ptr [rax+48h],0
                    00 00
  000000014089B338: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B33D: 48 C7 40 48 00 00  mov         qword ptr [rax+48h],0
                    00 00
  000000014089B345: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B34A: 48 C7 40 38 00 00  mov         qword ptr [rax+38h],0
                    00 00
  000000014089B352: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B357: 48 C7 40 30 00 00  mov         qword ptr [rax+30h],0
                    00 00
  000000014089B35F: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B364: 48 C7 40 28 00 00  mov         qword ptr [rax+28h],0
                    00 00
  000000014089B36C: 48 83 C4 38        add         rsp,38h
  000000014089B370: C3                 ret

  Summary

     1E02000 .text
