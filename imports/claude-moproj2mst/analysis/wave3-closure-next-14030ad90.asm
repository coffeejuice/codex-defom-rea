Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014030AD90: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014030AD95: 53                 push        rbx
  000000014030AD96: 57                 push        rdi
  000000014030AD97: 48 81 EC 08 05 00  sub         rsp,508h
                    00
  000000014030AD9E: 48 C7 84 24 C0 04  mov         qword ptr [rsp+00000000000004C0h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  000000014030ADAA: 48 8B 05 7F 32 85  mov         rax,qword ptr [0000000143B5E030h]
                    03
  000000014030ADB1: 48 33 C4           xor         rax,rsp
  000000014030ADB4: 48 89 84 24 F8 04  mov         qword ptr [rsp+00000000000004F8h],rax
                    00 00
  000000014030ADBC: C7 84 24 60 04 00  mov         dword ptr [rsp+0000000000000460h],0
                    00 00 00 00 00
  000000014030ADC7: C6 84 24 90 04 00  mov         byte ptr [rsp+0000000000000490h],0
                    00 00
  000000014030ADCF: 48 8B 8C 24 20 05  mov         rcx,qword ptr [rsp+0000000000000520h]
                    00 00
  000000014030ADD7: E8 C4 48 00 00     call        000000014030F6A0
  000000014030ADDC: 48 8B C8           mov         rcx,rax
  000000014030ADDF: FF 15 E3 91 AF 01  call        qword ptr [0000000141E03FC8h]
  000000014030ADE5: 0F B6 C0           movzx       eax,al
  000000014030ADE8: 85 C0              test        eax,eax
  000000014030ADEA: 74 07              je          000000014030ADF3
  000000014030ADEC: 32 C0              xor         al,al
  000000014030ADEE: E9 80 03 00 00     jmp         000000014030B173
  000000014030ADF3: 48 8B 8C 24 20 05  mov         rcx,qword ptr [rsp+0000000000000520h]
                    00 00
  000000014030ADFB: E8 90 F4 FF FF     call        000000014030A290
  000000014030AE00: 48 85 C0           test        rax,rax
  000000014030AE03: 75 07              jne         000000014030AE0C
  000000014030AE05: 32 C0              xor         al,al
  000000014030AE07: E9 67 03 00 00     jmp         000000014030B173
  000000014030AE0C: 48 8B 8C 24 20 05  mov         rcx,qword ptr [rsp+0000000000000520h]
                    00 00
  000000014030AE14: E8 97 F4 FF FF     call        000000014030A2B0
  000000014030AE19: 48 89 84 24 80 04  mov         qword ptr [rsp+0000000000000480h],rax
                    00 00
  000000014030AE21: 48 83 BC 24 80 04  cmp         qword ptr [rsp+0000000000000480h],0
                    00 00 00
  000000014030AE2A: 0F 85 EF 00 00 00  jne         000000014030AF1F
  000000014030AE30: C7 84 24 60 04 00  mov         dword ptr [rsp+0000000000000460h],0
                    00 00 00 00 00
  000000014030AE3B: EB 11              jmp         000000014030AE4E
  000000014030AE3D: 8B 84 24 60 04 00  mov         eax,dword ptr [rsp+0000000000000460h]
                    00
  000000014030AE44: 83 C0 01           add         eax,1
  000000014030AE47: 89 84 24 60 04 00  mov         dword ptr [rsp+0000000000000460h],eax
                    00
  000000014030AE4E: 83 BC 24 60 04 00  cmp         dword ptr [rsp+0000000000000460h],64h
                    00 64
  000000014030AE56: 0F 8D B7 00 00 00  jge         000000014030AF13
  000000014030AE5C: 8B BC 24 60 04 00  mov         edi,dword ptr [rsp+0000000000000460h]
                    00
  000000014030AE63: 83 C7 01           add         edi,1
  000000014030AE66: 48 8B 9C 24 20 05  mov         rbx,qword ptr [rsp+0000000000000520h]
                    00 00
  000000014030AE6E: 48 83 C3 48        add         rbx,48h
  000000014030AE72: 48 8B 8C 24 20 05  mov         rcx,qword ptr [rsp+0000000000000520h]
                    00 00
  000000014030AE7A: E8 21 48 00 00     call        000000014030F6A0
  000000014030AE7F: 48 8D 94 24 98 04  lea         rdx,[rsp+0000000000000498h]
                    00 00
  000000014030AE87: 48 8B C8           mov         rcx,rax
  000000014030AE8A: FF 15 C0 90 AF 01  call        qword ptr [0000000141E03F50h]
  000000014030AE90: 48 89 84 24 C8 04  mov         qword ptr [rsp+00000000000004C8h],rax
                    00 00
  000000014030AE98: 48 8B 84 24 C8 04  mov         rax,qword ptr [rsp+00000000000004C8h]
                    00 00
  000000014030AEA0: 48 89 84 24 D0 04  mov         qword ptr [rsp+00000000000004D0h],rax
                    00 00
  000000014030AEA8: 48 8B 8C 24 D0 04  mov         rcx,qword ptr [rsp+00000000000004D0h]
                    00 00
  000000014030AEB0: FF 15 72 93 AF 01  call        qword ptr [0000000141E04228h]
  000000014030AEB6: 4C 8B C0           mov         r8,rax
  000000014030AEB9: 48 8D 05 60 16 00  lea         rax,[000000014230C520h]
                    02
  000000014030AEC0: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014030AEC5: 89 7C 24 28        mov         dword ptr [rsp+28h],edi
  000000014030AEC9: 48 8D 05 58 16 00  lea         rax,[000000014230C528h]
                    02
  000000014030AED0: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014030AED5: 4C 8B CB           mov         r9,rbx
  000000014030AED8: 48 8D 15 59 16 00  lea         rdx,[000000014230C538h]
                    02
  000000014030AEDF: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  000000014030AEE4: E8 4F 2F 16 01     call        000000014146DE38
  000000014030AEE9: 90                 nop
  000000014030AEEA: 48 8D 8C 24 98 04  lea         rcx,[rsp+0000000000000498h]
                    00 00
  000000014030AEF2: FF 15 98 93 AF 01  call        qword ptr [0000000141E04290h]
  000000014030AEF8: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  000000014030AEFD: E8 9E 0C 4F 00     call        00000001407FBBA0
  000000014030AF02: 85 C0              test        eax,eax
  000000014030AF04: 74 08              je          000000014030AF0E
  000000014030AF06: C6 84 24 90 04 00  mov         byte ptr [rsp+0000000000000490h],1
                    00 01
  000000014030AF0E: E9 2A FF FF FF     jmp         000000014030AE3D
  000000014030AF13: 8A 84 24 90 04 00  mov         al,byte ptr [rsp+0000000000000490h]
                    00
  000000014030AF1A: E9 54 02 00 00     jmp         000000014030B173
  000000014030AF1F: 48 8B 8C 24 80 04  mov         rcx,qword ptr [rsp+0000000000000480h]
                    00 00
  000000014030AF27: E8 24 FD 96 00     call        0000000140C7AC50
  000000014030AF2C: 48 89 84 24 78 04  mov         qword ptr [rsp+0000000000000478h],rax
                    00 00
  000000014030AF34: 48 83 BC 24 78 04  cmp         qword ptr [rsp+0000000000000478h],0
                    00 00 00
  000000014030AF3D: 75 07              jne         000000014030AF46
  000000014030AF3F: 32 C0              xor         al,al
  000000014030AF41: E9 2D 02 00 00     jmp         000000014030B173
  000000014030AF46: 48 8D 84 24 A0 04  lea         rax,[rsp+00000000000004A0h]
                    00 00
  000000014030AF4E: 48 89 84 24 A8 04  mov         qword ptr [rsp+00000000000004A8h],rax
                    00 00
  000000014030AF56: 48 8B 8C 24 20 05  mov         rcx,qword ptr [rsp+0000000000000520h]
                    00 00
  000000014030AF5E: E8 2D F3 FF FF     call        000000014030A290
  000000014030AF63: 48 89 84 24 D8 04  mov         qword ptr [rsp+00000000000004D8h],rax
                    00 00
  000000014030AF6B: 48 8B 84 24 D8 04  mov         rax,qword ptr [rsp+00000000000004D8h]
                    00 00
  000000014030AF73: 48 8B 00           mov         rax,qword ptr [rax]
  000000014030AF76: 48 8B 94 24 A8 04  mov         rdx,qword ptr [rsp+00000000000004A8h]
                    00 00
  000000014030AF7E: 48 8B 8C 24 D8 04  mov         rcx,qword ptr [rsp+00000000000004D8h]
                    00 00
  000000014030AF86: FF 90 50 02 00 00  call        qword ptr [rax+0000000000000250h]
  000000014030AF8C: 48 89 84 24 E0 04  mov         qword ptr [rsp+00000000000004E0h],rax
                    00 00
  000000014030AF94: 48 8B 94 24 E0 04  mov         rdx,qword ptr [rsp+00000000000004E0h]
                    00 00
  000000014030AF9C: 48 8B 8C 24 20 05  mov         rcx,qword ptr [rsp+0000000000000520h]
                    00 00
  000000014030AFA4: E8 E7 1D 00 00     call        000000014030CD90
  000000014030AFA9: 48 8D 94 24 68 04  lea         rdx,[rsp+0000000000000468h]
                    00 00
  000000014030AFB1: 48 8B 8C 24 80 04  mov         rcx,qword ptr [rsp+0000000000000480h]
                    00 00
  000000014030AFB9: E8 F2 7C 97 00     call        0000000140C82CB0
  000000014030AFBE: 90                 nop
  000000014030AFBF: 48 C7 84 24 58 04  mov         qword ptr [rsp+0000000000000458h],0
                    00 00 00 00 00 00
  000000014030AFCB: 48 C7 84 24 88 04  mov         qword ptr [rsp+0000000000000488h],0
                    00 00 00 00 00 00
  000000014030AFD7: 48 8B 8C 24 20 05  mov         rcx,qword ptr [rsp+0000000000000520h]
                    00 00
  000000014030AFDF: E8 CC 4A 01 00     call        000000014031FAB0
  000000014030AFE4: 48 8B C8           mov         rcx,rax
  000000014030AFE7: E8 34 D7 0C 00     call        00000001403D8720
  000000014030AFEC: 48 8B C8           mov         rcx,rax
  000000014030AFEF: E8 5C 28 0E 00     call        00000001403ED850
  000000014030AFF4: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  000000014030AFF8: 8B 54 24 40        mov         edx,dword ptr [rsp+40h]
  000000014030AFFC: 48 8B 8C 24 78 04  mov         rcx,qword ptr [rsp+0000000000000478h]
                    00 00
  000000014030B004: E8 57 35 50 00     call        000000014080E560
  000000014030B009: 89 84 24 94 04 00  mov         dword ptr [rsp+0000000000000494h],eax
                    00
  000000014030B010: 48 8B 8C 24 78 04  mov         rcx,qword ptr [rsp+0000000000000478h]
                    00 00
  000000014030B018: E8 B3 3E 50 00     call        000000014080EED0
  000000014030B01D: 89 84 24 50 04 00  mov         dword ptr [rsp+0000000000000450h],eax
                    00
  000000014030B024: C7 84 24 60 04 00  mov         dword ptr [rsp+0000000000000460h],0
                    00 00 00 00 00
  000000014030B02F: EB 11              jmp         000000014030B042
  000000014030B031: 8B 84 24 60 04 00  mov         eax,dword ptr [rsp+0000000000000460h]
                    00
  000000014030B038: 83 C0 01           add         eax,1
  000000014030B03B: 89 84 24 60 04 00  mov         dword ptr [rsp+0000000000000460h],eax
                    00
  000000014030B042: 8B 84 24 50 04 00  mov         eax,dword ptr [rsp+0000000000000450h]
                    00
  000000014030B049: 39 84 24 60 04 00  cmp         dword ptr [rsp+0000000000000460h],eax
                    00
  000000014030B050: 0F 8D FA 00 00 00  jge         000000014030B150
  000000014030B056: 8B 84 24 60 04 00  mov         eax,dword ptr [rsp+0000000000000460h]
                    00
  000000014030B05D: 83 C0 01           add         eax,1
  000000014030B060: 89 84 24 70 04 00  mov         dword ptr [rsp+0000000000000470h],eax
                    00
  000000014030B067: 8B 94 24 60 04 00  mov         edx,dword ptr [rsp+0000000000000460h]
                    00
  000000014030B06E: 83 C2 01           add         edx,1
  000000014030B071: 48 8B 8C 24 80 04  mov         rcx,qword ptr [rsp+0000000000000480h]
                    00 00
  000000014030B079: E8 52 FE 96 00     call        0000000140C7AED0
  000000014030B07E: 48 89 84 24 88 04  mov         qword ptr [rsp+0000000000000488h],rax
                    00 00
  000000014030B086: 83 BC 24 70 04 00  cmp         dword ptr [rsp+0000000000000470h],0
                    00 00
  000000014030B08E: 7E 0A              jle         000000014030B09A
  000000014030B090: 83 BC 24 70 04 00  cmp         dword ptr [rsp+0000000000000470h],64h
                    00 64
  000000014030B098: 7E 02              jle         000000014030B09C
  000000014030B09A: EB 95              jmp         000000014030B031
  000000014030B09C: 48 8B 9C 24 20 05  mov         rbx,qword ptr [rsp+0000000000000520h]
                    00 00
  000000014030B0A4: 48 83 C3 48        add         rbx,48h
  000000014030B0A8: 48 8B 8C 24 20 05  mov         rcx,qword ptr [rsp+0000000000000520h]
                    00 00
  000000014030B0B0: E8 EB 45 00 00     call        000000014030F6A0
  000000014030B0B5: 48 8D 94 24 B0 04  lea         rdx,[rsp+00000000000004B0h]
                    00 00
  000000014030B0BD: 48 8B C8           mov         rcx,rax
  000000014030B0C0: FF 15 8A 8E AF 01  call        qword ptr [0000000141E03F50h]
  000000014030B0C6: 48 89 84 24 E8 04  mov         qword ptr [rsp+00000000000004E8h],rax
                    00 00
  000000014030B0CE: 48 8B 84 24 E8 04  mov         rax,qword ptr [rsp+00000000000004E8h]
                    00 00
  000000014030B0D6: 48 89 84 24 F0 04  mov         qword ptr [rsp+00000000000004F0h],rax
                    00 00
  000000014030B0DE: 48 8B 8C 24 F0 04  mov         rcx,qword ptr [rsp+00000000000004F0h]
                    00 00
  000000014030B0E6: FF 15 3C 91 AF 01  call        qword ptr [0000000141E04228h]
  000000014030B0EC: 4C 8B C0           mov         r8,rax
  000000014030B0EF: 48 8D 05 52 14 00  lea         rax,[000000014230C548h]
                    02
  000000014030B0F6: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014030B0FB: 8B 84 24 70 04 00  mov         eax,dword ptr [rsp+0000000000000470h]
                    00
  000000014030B102: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  000000014030B106: 48 8D 05 43 14 00  lea         rax,[000000014230C550h]
                    02
  000000014030B10D: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014030B112: 4C 8B CB           mov         r9,rbx
  000000014030B115: 48 8D 15 44 14 00  lea         rdx,[000000014230C560h]
                    02
  000000014030B11C: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  000000014030B121: E8 12 2D 16 01     call        000000014146DE38
  000000014030B126: 90                 nop
  000000014030B127: 48 8D 8C 24 B0 04  lea         rcx,[rsp+00000000000004B0h]
                    00 00
  000000014030B12F: FF 15 5B 91 AF 01  call        qword ptr [0000000141E04290h]
  000000014030B135: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  000000014030B13A: E8 61 0A 4F 00     call        00000001407FBBA0
  000000014030B13F: 85 C0              test        eax,eax
  000000014030B141: 74 08              je          000000014030B14B
  000000014030B143: C6 84 24 90 04 00  mov         byte ptr [rsp+0000000000000490h],1
                    00 01
  000000014030B14B: E9 E1 FE FF FF     jmp         000000014030B031
  000000014030B150: 0F B6 84 24 90 04  movzx       eax,byte ptr [rsp+0000000000000490h]
                    00 00
  000000014030B158: 88 84 24 B8 04 00  mov         byte ptr [rsp+00000000000004B8h],al
                    00
  000000014030B15F: 48 8D 8C 24 68 04  lea         rcx,[rsp+0000000000000468h]
                    00 00
  000000014030B167: E8 34 D0 EF FF     call        00000001402081A0
  000000014030B16C: 8A 84 24 B8 04 00  mov         al,byte ptr [rsp+00000000000004B8h]
                    00
  000000014030B173: 48 8B 8C 24 F8 04  mov         rcx,qword ptr [rsp+00000000000004F8h]
                    00 00
  000000014030B17B: 48 33 CC           xor         rcx,rsp
  000000014030B17E: E8 8D 2D 16 01     call        000000014146DF10
  000000014030B183: 48 81 C4 08 05 00  add         rsp,508h
                    00
  000000014030B18A: 5F                 pop         rdi
  000000014030B18B: 5B                 pop         rbx
  000000014030B18C: C3                 ret

  Summary

     1E02000 .text
