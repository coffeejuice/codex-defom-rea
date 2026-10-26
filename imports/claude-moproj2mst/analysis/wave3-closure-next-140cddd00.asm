Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140CDDD00: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140CDDD05: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140CDDD0A: 48 81 EC F8 00 00  sub         rsp,0F8h
                    00
  0000000140CDDD11: 48 C7 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  0000000140CDDD1D: 48 8B 8C 24 08 01  mov         rcx,qword ptr [rsp+0000000000000108h]
                    00 00
  0000000140CDDD25: FF 15 0D E8 12 01  call        qword ptr [0000000141E0C538h]
  0000000140CDDD2B: 0F B6 C0           movzx       eax,al
  0000000140CDDD2E: 85 C0              test        eax,eax
  0000000140CDDD30: 0F 85 B1 02 00 00  jne         0000000140CDDFE7
  0000000140CDDD36: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  0000000140CDDD3B: 48 8B 8C 24 08 01  mov         rcx,qword ptr [rsp+0000000000000108h]
                    00 00
  0000000140CDDD43: FF 15 5F E8 12 01  call        qword ptr [0000000141E0C5A8h]
  0000000140CDDD49: 48 89 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],rax
                    00 00
  0000000140CDDD51: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140CDDD59: 48 89 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],rax
                    00 00
  0000000140CDDD61: 48 8D 15 74 BE D0  lea         rdx,[00000001429E9BDCh]
                    01
  0000000140CDDD68: 48 8B 8C 24 A8 00  mov         rcx,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000140CDDD70: FF 15 F2 66 12 01  call        qword ptr [0000000141E04468h]
  0000000140CDDD76: 88 44 24 48        mov         byte ptr [rsp+48h],al
  0000000140CDDD7A: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  0000000140CDDD7F: FF 15 5B 7C 12 01  call        qword ptr [0000000141E059E0h]
  0000000140CDDD85: 44 0F B6 5C 24 48  movzx       r11d,byte ptr [rsp+48h]
  0000000140CDDD8B: 45 85 DB           test        r11d,r11d
  0000000140CDDD8E: 74 2C              je          0000000140CDDDBC
  0000000140CDDD90: 48 8B 8C 24 00 01  mov         rcx,qword ptr [rsp+0000000000000100h]
                    00 00
  0000000140CDDD98: E8 E3 FB FF FF     call        0000000140CDD980
  0000000140CDDD9D: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140CDDDA2: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140CDDDA8: 74 12              je          0000000140CDDDBC
  0000000140CDDDAA: 48 8B 94 24 08 01  mov         rdx,qword ptr [rsp+0000000000000108h]
                    00 00
  0000000140CDDDB2: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140CDDDB7: E8 B4 07 63 FF     call        000000014030E570
  0000000140CDDDBC: 48 8D 54 24 60     lea         rdx,[rsp+60h]
  0000000140CDDDC1: 48 8B 8C 24 08 01  mov         rcx,qword ptr [rsp+0000000000000108h]
                    00 00
  0000000140CDDDC9: FF 15 D9 E7 12 01  call        qword ptr [0000000141E0C5A8h]
  0000000140CDDDCF: 48 89 84 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],rax
                    00 00
  0000000140CDDDD7: 48 8B 84 24 B0 00  mov         rax,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140CDDDDF: 48 89 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],rax
                    00 00
  0000000140CDDDE7: 48 8D 15 F6 BD D0  lea         rdx,[00000001429E9BE4h]
                    01
  0000000140CDDDEE: 48 8B 8C 24 B8 00  mov         rcx,qword ptr [rsp+00000000000000B8h]
                    00 00
  0000000140CDDDF6: FF 15 6C 66 12 01  call        qword ptr [0000000141E04468h]
  0000000140CDDDFC: 88 44 24 58        mov         byte ptr [rsp+58h],al
  0000000140CDDE00: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  0000000140CDDE05: FF 15 D5 7B 12 01  call        qword ptr [0000000141E059E0h]
  0000000140CDDE0B: 44 0F B6 5C 24 58  movzx       r11d,byte ptr [rsp+58h]
  0000000140CDDE11: 45 85 DB           test        r11d,r11d
  0000000140CDDE14: 74 2C              je          0000000140CDDE42
  0000000140CDDE16: 48 8B 8C 24 00 01  mov         rcx,qword ptr [rsp+0000000000000100h]
                    00 00
  0000000140CDDE1E: E8 ED FB FF FF     call        0000000140CDDA10
  0000000140CDDE23: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140CDDE28: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  0000000140CDDE2E: 74 12              je          0000000140CDDE42
  0000000140CDDE30: 48 8B 94 24 08 01  mov         rdx,qword ptr [rsp+0000000000000108h]
                    00 00
  0000000140CDDE38: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140CDDE3D: E8 BE 6D 94 00     call        0000000141624C00
  0000000140CDDE42: 48 8D 54 24 70     lea         rdx,[rsp+70h]
  0000000140CDDE47: 48 8B 8C 24 08 01  mov         rcx,qword ptr [rsp+0000000000000108h]
                    00 00
  0000000140CDDE4F: FF 15 53 E7 12 01  call        qword ptr [0000000141E0C5A8h]
  0000000140CDDE55: 48 89 84 24 C0 00  mov         qword ptr [rsp+00000000000000C0h],rax
                    00 00
  0000000140CDDE5D: 48 8B 84 24 C0 00  mov         rax,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140CDDE65: 48 89 84 24 C8 00  mov         qword ptr [rsp+00000000000000C8h],rax
                    00 00
  0000000140CDDE6D: 48 8D 15 7C BD D0  lea         rdx,[00000001429E9BF0h]
                    01
  0000000140CDDE74: 48 8B 8C 24 C8 00  mov         rcx,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140CDDE7C: FF 15 E6 65 12 01  call        qword ptr [0000000141E04468h]
  0000000140CDDE82: 88 44 24 68        mov         byte ptr [rsp+68h],al
  0000000140CDDE86: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000140CDDE8B: FF 15 4F 7B 12 01  call        qword ptr [0000000141E059E0h]
  0000000140CDDE91: 44 0F B6 5C 24 68  movzx       r11d,byte ptr [rsp+68h]
  0000000140CDDE97: 45 85 DB           test        r11d,r11d
  0000000140CDDE9A: 74 2C              je          0000000140CDDEC8
  0000000140CDDE9C: 48 8B 8C 24 00 01  mov         rcx,qword ptr [rsp+0000000000000100h]
                    00 00
  0000000140CDDEA4: E8 17 FD FF FF     call        0000000140CDDBC0
  0000000140CDDEA9: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140CDDEAE: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  0000000140CDDEB4: 74 12              je          0000000140CDDEC8
  0000000140CDDEB6: 48 8B 94 24 08 01  mov         rdx,qword ptr [rsp+0000000000000108h]
                    00 00
  0000000140CDDEBE: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140CDDEC3: E8 48 28 92 00     call        0000000141600710
  0000000140CDDEC8: 48 8D 94 24 80 00  lea         rdx,[rsp+0000000000000080h]
                    00 00
  0000000140CDDED0: 48 8B 8C 24 08 01  mov         rcx,qword ptr [rsp+0000000000000108h]
                    00 00
  0000000140CDDED8: FF 15 CA E6 12 01  call        qword ptr [0000000141E0C5A8h]
  0000000140CDDEDE: 48 89 84 24 D0 00  mov         qword ptr [rsp+00000000000000D0h],rax
                    00 00
  0000000140CDDEE6: 48 8B 84 24 D0 00  mov         rax,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140CDDEEE: 48 89 84 24 D8 00  mov         qword ptr [rsp+00000000000000D8h],rax
                    00 00
  0000000140CDDEF6: 48 8D 15 03 BD D0  lea         rdx,[00000001429E9C00h]
                    01
  0000000140CDDEFD: 48 8B 8C 24 D8 00  mov         rcx,qword ptr [rsp+00000000000000D8h]
                    00 00
  0000000140CDDF05: FF 15 5D 65 12 01  call        qword ptr [0000000141E04468h]
  0000000140CDDF0B: 88 44 24 78        mov         byte ptr [rsp+78h],al
  0000000140CDDF0F: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  0000000140CDDF17: FF 15 C3 7A 12 01  call        qword ptr [0000000141E059E0h]
  0000000140CDDF1D: 44 0F B6 5C 24 78  movzx       r11d,byte ptr [rsp+78h]
  0000000140CDDF23: 45 85 DB           test        r11d,r11d
  0000000140CDDF26: 74 2C              je          0000000140CDDF54
  0000000140CDDF28: 48 8B 8C 24 00 01  mov         rcx,qword ptr [rsp+0000000000000100h]
                    00 00
  0000000140CDDF30: E8 6B FB FF FF     call        0000000140CDDAA0
  0000000140CDDF35: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140CDDF3A: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  0000000140CDDF40: 74 12              je          0000000140CDDF54
  0000000140CDDF42: 48 8B 94 24 08 01  mov         rdx,qword ptr [rsp+0000000000000108h]
                    00 00
  0000000140CDDF4A: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  0000000140CDDF4F: E8 0C 86 9D 00     call        00000001416B6560
  0000000140CDDF54: 48 8D 94 24 90 00  lea         rdx,[rsp+0000000000000090h]
                    00 00
  0000000140CDDF5C: 48 8B 8C 24 08 01  mov         rcx,qword ptr [rsp+0000000000000108h]
                    00 00
  0000000140CDDF64: FF 15 3E E6 12 01  call        qword ptr [0000000141E0C5A8h]
  0000000140CDDF6A: 48 89 84 24 E0 00  mov         qword ptr [rsp+00000000000000E0h],rax
                    00 00
  0000000140CDDF72: 48 8B 84 24 E0 00  mov         rax,qword ptr [rsp+00000000000000E0h]
                    00 00
  0000000140CDDF7A: 48 89 84 24 E8 00  mov         qword ptr [rsp+00000000000000E8h],rax
                    00 00
  0000000140CDDF82: 48 8D 15 8F BC D0  lea         rdx,[00000001429E9C18h]
                    01
  0000000140CDDF89: 48 8B 8C 24 E8 00  mov         rcx,qword ptr [rsp+00000000000000E8h]
                    00 00
  0000000140CDDF91: FF 15 D1 64 12 01  call        qword ptr [0000000141E04468h]
  0000000140CDDF97: 88 84 24 88 00 00  mov         byte ptr [rsp+0000000000000088h],al
                    00
  0000000140CDDF9E: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  0000000140CDDFA6: FF 15 34 7A 12 01  call        qword ptr [0000000141E059E0h]
  0000000140CDDFAC: 44 0F B6 9C 24 88  movzx       r11d,byte ptr [rsp+0000000000000088h]
                    00 00 00
  0000000140CDDFB5: 45 85 DB           test        r11d,r11d
  0000000140CDDFB8: 74 2D              je          0000000140CDDFE7
  0000000140CDDFBA: 48 8B 8C 24 00 01  mov         rcx,qword ptr [rsp+0000000000000100h]
                    00 00
  0000000140CDDFC2: E8 69 FB FF FF     call        0000000140CDDB30
  0000000140CDDFC7: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140CDDFCC: 48 83 7C 24 40 00  cmp         qword ptr [rsp+40h],0
  0000000140CDDFD2: 74 13              je          0000000140CDDFE7
  0000000140CDDFD4: 48 8B 94 24 08 01  mov         rdx,qword ptr [rsp+0000000000000108h]
                    00 00
  0000000140CDDFDC: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140CDDFE1: E8 5A C5 8A 00     call        000000014158A540
  0000000140CDDFE6: 90                 nop
  0000000140CDDFE7: 48 8B 8C 24 08 01  mov         rcx,qword ptr [rsp+0000000000000108h]
                    00 00
  0000000140CDDFEF: FF 15 BB E6 12 01  call        qword ptr [0000000141E0C6B0h]
  0000000140CDDFF5: 48 81 C4 F8 00 00  add         rsp,0F8h
                    00
  0000000140CDDFFC: C3                 ret

  Summary

     1E02000 .text
