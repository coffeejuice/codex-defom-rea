Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140045490: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140045495: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014004549A: 48 83 EC 48        sub         rsp,48h
  000000014004549E: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001400454A7: B9 F0 19 00 00     mov         ecx,19F0h
  00000001400454AC: E8 0B 96 42 01     call        000000014146EABC
  00000001400454B1: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001400454B6: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  00000001400454BC: 74 13              je          00000001400454D1
  00000001400454BE: 33 D2              xor         edx,edx
  00000001400454C0: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  00000001400454C5: E8 16 EB FF FF     call        0000000140043FE0
  00000001400454CA: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001400454CF: EB 09              jmp         00000001400454DA
  00000001400454D1: 48 C7 44 24 38 00  mov         qword ptr [rsp+38h],0
                    00 00 00
  00000001400454DA: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  00000001400454DF: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001400454E4: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000001400454E9: 48 83 C4 48        add         rsp,48h
  00000001400454ED: C3                 ret
  00000001400454EE: CC                 int         3
  00000001400454EF: CC                 int         3

  Summary

     1E02000 .text

Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140CDCDC0: 44 88 44 24 18     mov         byte ptr [rsp+18h],r8b
  0000000140CDCDC5: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140CDCDCA: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140CDCDCF: 53                 push        rbx
  0000000140CDCDD0: 48 81 EC 00 01 00  sub         rsp,100h
                    00
  0000000140CDCDD7: 48 C7 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  0000000140CDCDE3: 48 8B 84 24 10 01  mov         rax,qword ptr [rsp+0000000000000110h]
                    00 00
  0000000140CDCDEB: C6 40 20 01        mov         byte ptr [rax+20h],1
  0000000140CDCDEF: 48 8B 8C 24 10 01  mov         rcx,qword ptr [rsp+0000000000000110h]
                    00 00
  0000000140CDCDF7: 48 83 C1 28        add         rcx,28h
  0000000140CDCDFB: 48 8B 94 24 18 01  mov         rdx,qword ptr [rsp+0000000000000118h]
                    00 00
  0000000140CDCE03: FF 15 C7 72 12 01  call        qword ptr [0000000141E040D0h]
  0000000140CDCE09: 0F B6 C0           movzx       eax,al
  0000000140CDCE0C: 85 C0              test        eax,eax
  0000000140CDCE0E: 74 50              je          0000000140CDCE60
  0000000140CDCE10: 48 8B 8C 24 10 01  mov         rcx,qword ptr [rsp+0000000000000110h]
                    00 00
  0000000140CDCE18: 48 83 C1 28        add         rcx,28h
  0000000140CDCE1C: 48 8B 94 24 18 01  mov         rdx,qword ptr [rsp+0000000000000118h]
                    00 00
  0000000140CDCE24: FF 15 46 76 12 01  call        qword ptr [0000000141E04470h]
  0000000140CDCE2A: 0F B6 84 24 20 01  movzx       eax,byte ptr [rsp+0000000000000120h]
                    00 00
  0000000140CDCE32: 85 C0              test        eax,eax
  0000000140CDCE34: 75 0D              jne         0000000140CDCE43
  0000000140CDCE36: 48 8B 8C 24 10 01  mov         rcx,qword ptr [rsp+0000000000000110h]
                    00 00
  0000000140CDCE3E: E8 7D DD FF FF     call        0000000140CDABC0
  0000000140CDCE43: 48 8B 84 24 10 01  mov         rax,qword ptr [rsp+0000000000000110h]
                    00 00
  0000000140CDCE4B: C6 40 38 00        mov         byte ptr [rax+38h],0
  0000000140CDCE4F: 48 8B 8C 24 10 01  mov         rcx,qword ptr [rsp+0000000000000110h]
                    00 00
  0000000140CDCE57: 48 83 C1 40        add         rcx,40h
  0000000140CDCE5B: E8 F0 CA 05 00     call        0000000140D39950
  0000000140CDCE60: 48 8B 8C 24 10 01  mov         rcx,qword ptr [rsp+0000000000000110h]
                    00 00
  0000000140CDCE68: 48 83 C1 28        add         rcx,28h
  0000000140CDCE6C: FF 15 56 71 12 01  call        qword ptr [0000000141E03FC8h]
  0000000140CDCE72: 0F B6 C0           movzx       eax,al
  0000000140CDCE75: 85 C0              test        eax,eax
  0000000140CDCE77: 0F 85 57 01 00 00  jne         0000000140CDCFD4
  0000000140CDCE7D: 48 8D 15 00 98 D0  lea         rdx,[00000001429E6684h]
                    01
  0000000140CDCE84: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  0000000140CDCE89: FF 15 F1 75 12 01  call        qword ptr [0000000141E04480h]
  0000000140CDCE8F: 90                 nop
  0000000140CDCE90: 48 8B 8C 24 10 01  mov         rcx,qword ptr [rsp+0000000000000110h]
                    00 00
  0000000140CDCE98: 48 83 C1 28        add         rcx,28h
  0000000140CDCE9C: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  0000000140CDCEA1: E8 AA BC B3 FF     call        0000000140818B50
  0000000140CDCEA6: 0F B6 C0           movzx       eax,al
  0000000140CDCEA9: 85 C0              test        eax,eax
  0000000140CDCEAB: 75 0D              jne         0000000140CDCEBA
  0000000140CDCEAD: C7 84 24 98 00 00  mov         dword ptr [rsp+0000000000000098h],1
                    00 01 00 00 00
  0000000140CDCEB8: EB 0B              jmp         0000000140CDCEC5
  0000000140CDCEBA: C7 84 24 98 00 00  mov         dword ptr [rsp+0000000000000098h],0
                    00 00 00 00 00
  0000000140CDCEC5: 0F B6 84 24 98 00  movzx       eax,byte ptr [rsp+0000000000000098h]
                    00 00
  0000000140CDCECD: 88 44 24 30        mov         byte ptr [rsp+30h],al
  0000000140CDCED1: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  0000000140CDCED6: FF 15 04 8B 12 01  call        qword ptr [0000000141E059E0h]
  0000000140CDCEDC: 44 0F B6 5C 24 30  movzx       r11d,byte ptr [rsp+30h]
  0000000140CDCEE2: 45 85 DB           test        r11d,r11d
  0000000140CDCEE5: 0F 84 E9 00 00 00  je          0000000140CDCFD4
  0000000140CDCEEB: 48 8D 15 C6 CC D0  lea         rdx,[00000001429E9BB8h]
                    01
  0000000140CDCEF2: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  0000000140CDCEF7: FF 15 83 75 12 01  call        qword ptr [0000000141E04480h]
  0000000140CDCEFD: 90                 nop
  0000000140CDCEFE: 48 8B 8C 24 10 01  mov         rcx,qword ptr [rsp+0000000000000110h]
                    00 00
  0000000140CDCF06: 48 83 C1 28        add         rcx,28h
  0000000140CDCF0A: 41 B8 01 00 00 00  mov         r8d,1
  0000000140CDCF10: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  0000000140CDCF15: FF 15 A5 70 12 01  call        qword ptr [0000000141E03FC0h]
  0000000140CDCF1B: 0F B6 C0           movzx       eax,al
  0000000140CDCF1E: 85 C0              test        eax,eax
  0000000140CDCF20: 75 0D              jne         0000000140CDCF2F
  0000000140CDCF22: C7 84 24 9C 00 00  mov         dword ptr [rsp+000000000000009Ch],1
                    00 01 00 00 00
  0000000140CDCF2D: EB 0B              jmp         0000000140CDCF3A
  0000000140CDCF2F: C7 84 24 9C 00 00  mov         dword ptr [rsp+000000000000009Ch],0
                    00 00 00 00 00
  0000000140CDCF3A: 0F B6 84 24 9C 00  movzx       eax,byte ptr [rsp+000000000000009Ch]
                    00 00
  0000000140CDCF42: 88 44 24 40        mov         byte ptr [rsp+40h],al
  0000000140CDCF46: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  0000000140CDCF4B: FF 15 8F 8A 12 01  call        qword ptr [0000000141E059E0h]
  0000000140CDCF51: 44 0F B6 5C 24 40  movzx       r11d,byte ptr [rsp+40h]
  0000000140CDCF57: 45 85 DB           test        r11d,r11d
  0000000140CDCF5A: 74 78              je          0000000140CDCFD4
  0000000140CDCF5C: 48 8D 15 51 97 D0  lea         rdx,[00000001429E66B4h]
                    01
  0000000140CDCF63: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  0000000140CDCF68: FF 15 12 75 12 01  call        qword ptr [0000000141E04480h]
  0000000140CDCF6E: 90                 nop
  0000000140CDCF6F: 48 8B 94 24 10 01  mov         rdx,qword ptr [rsp+0000000000000110h]
                    00 00
  0000000140CDCF77: 48 83 C2 28        add         rdx,28h
  0000000140CDCF7B: 4C 8D 44 24 50     lea         r8,[rsp+50h]
  0000000140CDCF80: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  0000000140CDCF85: E8 C6 BD B3 FF     call        0000000140818D50
  0000000140CDCF8A: 48 89 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],rax
                    00 00
  0000000140CDCF92: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140CDCF9A: 48 89 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],rax
                    00 00
  0000000140CDCFA2: 48 8B 8C 24 10 01  mov         rcx,qword ptr [rsp+0000000000000110h]
                    00 00
  0000000140CDCFAA: 48 83 C1 28        add         rcx,28h
  0000000140CDCFAE: 48 8B 94 24 A8 00  mov         rdx,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000140CDCFB6: FF 15 B4 74 12 01  call        qword ptr [0000000141E04470h]
  0000000140CDCFBC: 90                 nop
  0000000140CDCFBD: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  0000000140CDCFC2: FF 15 18 8A 12 01  call        qword ptr [0000000141E059E0h]
  0000000140CDCFC8: 90                 nop
  0000000140CDCFC9: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  0000000140CDCFCE: FF 15 0C 8A 12 01  call        qword ptr [0000000141E059E0h]
  0000000140CDCFD4: 48 8B 94 24 10 01  mov         rdx,qword ptr [rsp+0000000000000110h]
                    00 00
  0000000140CDCFDC: 48 83 C2 28        add         rdx,28h
  0000000140CDCFE0: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  0000000140CDCFE5: FF 15 CD 70 12 01  call        qword ptr [0000000141E040B8h]
  0000000140CDCFEB: 48 89 84 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],rax
                    00 00
  0000000140CDCFF3: 48 8B 84 24 B0 00  mov         rax,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140CDCFFB: 48 89 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],rax
                    00 00
  0000000140CDD003: 48 8B 8C 24 10 01  mov         rcx,qword ptr [rsp+0000000000000110h]
                    00 00
  0000000140CDD00B: 48 83 C1 28        add         rcx,28h
  0000000140CDD00F: 48 8B 94 24 B8 00  mov         rdx,qword ptr [rsp+00000000000000B8h]
                    00 00
  0000000140CDD017: FF 15 53 74 12 01  call        qword ptr [0000000141E04470h]
  0000000140CDD01D: 90                 nop
  0000000140CDD01E: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  0000000140CDD023: FF 15 B7 89 12 01  call        qword ptr [0000000141E059E0h]
  0000000140CDD029: C7 44 24 28 00 00  mov         dword ptr [rsp+28h],0
                    00 00
  0000000140CDD031: C7 44 24 2C 00 00  mov         dword ptr [rsp+2Ch],0
                    00 00
  0000000140CDD039: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  0000000140CDD03E: FF 15 0C 72 12 01  call        qword ptr [0000000141E04250h]
  0000000140CDD044: 48 8B D8           mov         rbx,rax
  0000000140CDD047: 48 8B 94 24 10 01  mov         rdx,qword ptr [rsp+0000000000000110h]
                    00 00
  0000000140CDD04F: 48 83 C2 28        add         rdx,28h
  0000000140CDD053: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000140CDD058: FF 15 02 6F 12 01  call        qword ptr [0000000141E03F60h]
  0000000140CDD05E: 48 89 84 24 C0 00  mov         qword ptr [rsp+00000000000000C0h],rax
                    00 00
  0000000140CDD066: 48 8B 84 24 C0 00  mov         rax,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140CDD06E: 48 89 84 24 C8 00  mov         qword ptr [rsp+00000000000000C8h],rax
                    00 00
  0000000140CDD076: 48 8D 54 24 78     lea         rdx,[rsp+78h]
  0000000140CDD07B: 48 8B 8C 24 C8 00  mov         rcx,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140CDD083: FF 15 F7 74 12 01  call        qword ptr [0000000141E04580h]
  0000000140CDD089: 48 89 84 24 D0 00  mov         qword ptr [rsp+00000000000000D0h],rax
                    00 00
  0000000140CDD091: 48 8B 84 24 D0 00  mov         rax,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140CDD099: 48 89 84 24 D8 00  mov         qword ptr [rsp+00000000000000D8h],rax
                    00 00
  0000000140CDD0A1: 44 0F B7 03        movzx       r8d,word ptr [rbx]
  0000000140CDD0A5: 48 8B 94 24 D8 00  mov         rdx,qword ptr [rsp+00000000000000D8h]
                    00 00
  0000000140CDD0AD: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  0000000140CDD0B5: E8 B6 78 32 FF     call        0000000140004970
  0000000140CDD0BA: 48 89 84 24 E0 00  mov         qword ptr [rsp+00000000000000E0h],rax
                    00 00
  0000000140CDD0C2: 48 8B 84 24 E0 00  mov         rax,qword ptr [rsp+00000000000000E0h]
                    00 00
  0000000140CDD0CA: 48 89 84 24 E8 00  mov         qword ptr [rsp+00000000000000E8h],rax
                    00 00
  0000000140CDD0D2: 4C 8D 05 E7 CA D0  lea         r8,[00000001429E9BC0h]
                    01
  0000000140CDD0D9: 48 8B 94 24 E8 00  mov         rdx,qword ptr [rsp+00000000000000E8h]
                    00 00
  0000000140CDD0E1: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140CDD0E6: E8 25 50 32 FF     call        0000000140002110
  0000000140CDD0EB: 90                 nop
  0000000140CDD0EC: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  0000000140CDD0F4: FF 15 E6 88 12 01  call        qword ptr [0000000141E059E0h]
  0000000140CDD0FA: 90                 nop
  0000000140CDD0FB: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  0000000140CDD100: FF 15 DA 88 12 01  call        qword ptr [0000000141E059E0h]
  0000000140CDD106: 90                 nop
  0000000140CDD107: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000140CDD10C: FF 15 76 6E 12 01  call        qword ptr [0000000141E03F88h]
  0000000140CDD112: 48 8D 94 24 88 00  lea         rdx,[rsp+0000000000000088h]
                    00 00
  0000000140CDD11A: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140CDD11F: FF 15 2B 6E 12 01  call        qword ptr [0000000141E03F50h]
  0000000140CDD125: 48 89 84 24 F0 00  mov         qword ptr [rsp+00000000000000F0h],rax
                    00 00
  0000000140CDD12D: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140CDD135: 48 89 84 24 F8 00  mov         qword ptr [rsp+00000000000000F8h],rax
                    00 00
  0000000140CDD13D: 48 8B 8C 24 F8 00  mov         rcx,qword ptr [rsp+00000000000000F8h]
                    00 00
  0000000140CDD145: FF 15 DD 70 12 01  call        qword ptr [0000000141E04228h]
  0000000140CDD14B: 4C 8B C8           mov         r9,rax
  0000000140CDD14E: 4C 8D 44 24 2C     lea         r8,[rsp+2Ch]
  0000000140CDD153: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  0000000140CDD158: 48 8D 0D 01 77 AE  lea         rcx,[00000001447C4860h]
                    03
  0000000140CDD15F: E8 4C 19 00 00     call        0000000140CDEAB0
  0000000140CDD164: 90                 nop
  0000000140CDD165: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  0000000140CDD16D: FF 15 1D 71 12 01  call        qword ptr [0000000141E04290h]
  0000000140CDD173: 90                 nop
  0000000140CDD174: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140CDD179: FF 15 61 88 12 01  call        qword ptr [0000000141E059E0h]
  0000000140CDD17F: 48 81 C4 00 01 00  add         rsp,100h
                    00
  0000000140CDD186: 5B                 pop         rbx
  0000000140CDD187: C3                 ret

  Summary

     1E02000 .text
