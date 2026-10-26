Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014002DE50: F2 0F 11 5C 24 20  movsd       mmword ptr [rsp+20h],xmm3
  000000014002DE56: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  000000014002DE5B: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014002DE60: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014002DE65: 53                 push        rbx
  000000014002DE66: 48 81 EC 50 01 00  sub         rsp,150h
                    00
  000000014002DE6D: 48 C7 84 24 F0 00  mov         qword ptr [rsp+00000000000000F0h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  000000014002DE79: 0F 29 B4 24 40 01  movaps      xmmword ptr [rsp+0000000000000140h],xmm6
                    00 00
  000000014002DE81: 0F 29 BC 24 30 01  movaps      xmmword ptr [rsp+0000000000000130h],xmm7
                    00 00
  000000014002DE89: 48 8D 44 24 70     lea         rax,[rsp+70h]
  000000014002DE8E: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  000000014002DE93: 33 D2              xor         edx,edx
  000000014002DE95: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  000000014002DE9A: E8 D1 8D 51 00     call        0000000140546C70
  000000014002DE9F: 4C 8B C0           mov         r8,rax
  000000014002DEA2: 48 8B 94 24 60 01  mov         rdx,qword ptr [rsp+0000000000000160h]
                    00 00
  000000014002DEAA: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000014002DEAF: FF 15 6B DA DD 01  call        qword ptr [0000000141E0B920h]
  000000014002DEB5: 90                 nop
  000000014002DEB6: 48 8B 94 24 68 01  mov         rdx,qword ptr [rsp+0000000000000168h]
                    00 00
  000000014002DEBE: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000014002DEC3: FF 15 17 DE DD 01  call        qword ptr [0000000141E0BCE0h]
  000000014002DEC9: 48 8D 84 24 80 00  lea         rax,[rsp+0000000000000080h]
                    00 00
  000000014002DED1: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  000000014002DED9: 48 8D 94 24 90 00  lea         rdx,[rsp+0000000000000090h]
                    00 00
  000000014002DEE1: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000014002DEE6: FF 15 3C DA DD 01  call        qword ptr [0000000141E0B928h]
  000000014002DEEC: 41 B8 FF FF FE FF  mov         r8d,0FFFEFFFFh
  000000014002DEF2: 48 8B 94 24 88 00  mov         rdx,qword ptr [rsp+0000000000000088h]
                    00 00
  000000014002DEFA: 48 8B C8           mov         rcx,rax
  000000014002DEFD: E8 FE AA D5 00     call        0000000140D88A00
  000000014002DF02: 48 8B D0           mov         rdx,rax
  000000014002DF05: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000014002DF0A: FF 15 20 DA DD 01  call        qword ptr [0000000141E0B930h]
  000000014002DF10: B9 20 00 00 00     mov         ecx,20h
  000000014002DF15: E8 A2 0B 44 01     call        000000014146EABC
  000000014002DF1A: 48 89 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],rax
                    00 00
  000000014002DF22: 48 83 BC 24 A0 00  cmp         qword ptr [rsp+00000000000000A0h],0
                    00 00 00
  000000014002DF2B: 74 45              je          000000014002DF72
  000000014002DF2D: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014002DF35: FF 15 FD D9 DD 01  call        qword ptr [0000000141E0B938h]
  000000014002DF3B: 4C 8B 9C 24 A0 00  mov         r11,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014002DF43: 48 8D 05 6E 36 0D  lea         rax,[00000001421015B8h]
                    02
  000000014002DF4A: 49 89 03           mov         qword ptr [r11],rax
  000000014002DF4D: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014002DF55: 48 8D 05 DC 35 0D  lea         rax,[0000000142101538h]
                    02
  000000014002DF5C: 48 89 41 10        mov         qword ptr [rcx+10h],rax
  000000014002DF60: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014002DF68: 48 89 84 24 F8 00  mov         qword ptr [rsp+00000000000000F8h],rax
                    00 00
  000000014002DF70: EB 0C              jmp         000000014002DF7E
  000000014002DF72: 48 C7 84 24 F8 00  mov         qword ptr [rsp+00000000000000F8h],0
                    00 00 00 00 00 00
  000000014002DF7E: 48 8B 84 24 F8 00  mov         rax,qword ptr [rsp+00000000000000F8h]
                    00 00
  000000014002DF86: 48 89 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],rax
                    00 00
  000000014002DF8E: 48 8B 84 24 98 00  mov         rax,qword ptr [rsp+0000000000000098h]
                    00 00
  000000014002DF96: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  000000014002DF9B: 33 D2              xor         edx,edx
  000000014002DF9D: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014002DFA2: FF 15 98 D9 DD 01  call        qword ptr [0000000141E0B940h]
  000000014002DFA8: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  000000014002DFAD: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000014002DFB2: FF 15 90 D9 DD 01  call        qword ptr [0000000141E0B948h]
  000000014002DFB8: B9 80 00 00 00     mov         ecx,80h
  000000014002DFBD: E8 FA 0A 44 01     call        000000014146EABC
  000000014002DFC2: 48 89 84 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],rax
                    00 00
  000000014002DFCA: 48 83 BC 24 B0 00  cmp         qword ptr [rsp+00000000000000B0h],0
                    00 00 00
  000000014002DFD3: 74 1C              je          000000014002DFF1
  000000014002DFD5: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  000000014002DFDA: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  000000014002DFE2: E8 29 6D 2D 00     call        0000000140304D10
  000000014002DFE7: 48 89 84 24 00 01  mov         qword ptr [rsp+0000000000000100h],rax
                    00 00
  000000014002DFEF: EB 0C              jmp         000000014002DFFD
  000000014002DFF1: 48 C7 84 24 00 01  mov         qword ptr [rsp+0000000000000100h],0
                    00 00 00 00 00 00
  000000014002DFFD: 48 8B 84 24 00 01  mov         rax,qword ptr [rsp+0000000000000100h]
                    00 00
  000000014002E005: 48 89 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],rax
                    00 00
  000000014002E00D: 48 8B 84 24 A8 00  mov         rax,qword ptr [rsp+00000000000000A8h]
                    00 00
  000000014002E015: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  000000014002E01A: 4C 8B 8C 24 90 01  mov         r9,qword ptr [rsp+0000000000000190h]
                    00 00
  000000014002E022: 4C 8B 84 24 88 01  mov         r8,qword ptr [rsp+0000000000000188h]
                    00 00
  000000014002E02A: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  000000014002E02F: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  000000014002E034: E8 B7 6E 2D 00     call        0000000140304EF0
  000000014002E039: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000014002E03E: FF 15 0C D9 DD 01  call        qword ptr [0000000141E0B950h]
  000000014002E044: 48 8B 54 24 38     mov         rdx,qword ptr [rsp+38h]
  000000014002E049: 48 8B C8           mov         rcx,rax
  000000014002E04C: FF 15 06 D9 DD 01  call        qword ptr [0000000141E0B958h]
  000000014002E052: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  000000014002E057: E8 B4 70 2D 00     call        0000000140305110
  000000014002E05C: FF 15 EE 63 DD 01  call        qword ptr [0000000141E04450h]
  000000014002E062: 48 8B 8C 24 70 01  mov         rcx,qword ptr [rsp+0000000000000170h]
                    00 00
  000000014002E06A: FF 15 B0 5E DD 01  call        qword ptr [0000000141E03F20h]
  000000014002E070: 66 0F 6E F8        movd        xmm7,eax
  000000014002E074: F3 0F E6 FF        cvtdq2pd    xmm7,xmm7
  000000014002E078: F2 0F 10 35 A0 34  movsd       xmm6,mmword ptr [0000000142101520h]
                    0D 02
  000000014002E080: F2 0F 5C B4 24 78  subsd       xmm6,mmword ptr [rsp+0000000000000178h]
                    01 00 00
  000000014002E089: 48 8D 8C 24 B8 00  lea         rcx,[rsp+00000000000000B8h]
                    00 00
  000000014002E091: FF 15 C9 D8 DD 01  call        qword ptr [0000000141E0B960h]
  000000014002E097: 48 89 84 24 08 01  mov         qword ptr [rsp+0000000000000108h],rax
                    00 00
  000000014002E09F: 48 8B 84 24 08 01  mov         rax,qword ptr [rsp+0000000000000108h]
                    00 00
  000000014002E0A7: 48 89 84 24 10 01  mov         qword ptr [rsp+0000000000000110h],rax
                    00 00
  000000014002E0AF: 48 8B 8C 24 10 01  mov         rcx,qword ptr [rsp+0000000000000110h]
                    00 00
  000000014002E0B7: FF 15 AB D8 DD 01  call        qword ptr [0000000141E0B968h]
  000000014002E0BD: 66 0F 6E C8        movd        xmm1,eax
  000000014002E0C1: F3 0F E6 C9        cvtdq2pd    xmm1,xmm1
  000000014002E0C5: F2 0F 5C 0D 4B 34  subsd       xmm1,mmword ptr [0000000142101518h]
                    0D 02
  000000014002E0CD: F2 0F 10 84 24 78  movsd       xmm0,mmword ptr [rsp+0000000000000178h]
                    01 00 00
  000000014002E0D6: F2 0F 59 C1        mulsd       xmm0,xmm1
  000000014002E0DA: F2 0F 5E 05 36 34  divsd       xmm0,mmword ptr [0000000142101518h]
                    0D 02
  000000014002E0E2: 66 0F 28 CE        movapd      xmm1,xmm6
  000000014002E0E6: F2 0F 58 C8        addsd       xmm1,xmm0
  000000014002E0EA: 66 0F 28 C7        movapd      xmm0,xmm7
  000000014002E0EE: F2 0F 59 C1        mulsd       xmm0,xmm1
  000000014002E0F2: F2 0F 2C C0        cvttsd2si   eax,xmm0
  000000014002E0F6: 89 44 24 30        mov         dword ptr [rsp+30h],eax
  000000014002E0FA: 48 8D 8C 24 B8 00  lea         rcx,[rsp+00000000000000B8h]
                    00 00
  000000014002E102: FF 15 20 DE DD 01  call        qword ptr [0000000141E0BF28h]
  000000014002E108: FF 15 42 63 DD 01  call        qword ptr [0000000141E04450h]
  000000014002E10E: 48 8B 8C 24 70 01  mov         rcx,qword ptr [rsp+0000000000000170h]
                    00 00
  000000014002E116: FF 15 0C 5E DD 01  call        qword ptr [0000000141E03F28h]
  000000014002E11C: 66 0F 6E F8        movd        xmm7,eax
  000000014002E120: F3 0F E6 FF        cvtdq2pd    xmm7,xmm7
  000000014002E124: F2 0F 10 35 F4 33  movsd       xmm6,mmword ptr [0000000142101520h]
                    0D 02
  000000014002E12C: F2 0F 5C B4 24 80  subsd       xmm6,mmword ptr [rsp+0000000000000180h]
                    01 00 00
  000000014002E135: 48 8D 8C 24 C8 00  lea         rcx,[rsp+00000000000000C8h]
                    00 00
  000000014002E13D: FF 15 1D D8 DD 01  call        qword ptr [0000000141E0B960h]
  000000014002E143: 48 89 84 24 18 01  mov         qword ptr [rsp+0000000000000118h],rax
                    00 00
  000000014002E14B: 48 8B 84 24 18 01  mov         rax,qword ptr [rsp+0000000000000118h]
                    00 00
  000000014002E153: 48 89 84 24 20 01  mov         qword ptr [rsp+0000000000000120h],rax
                    00 00
  000000014002E15B: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  000000014002E163: FF 15 FF D7 DD 01  call        qword ptr [0000000141E0B968h]
  000000014002E169: 66 0F 6E C8        movd        xmm1,eax
  000000014002E16D: F3 0F E6 C9        cvtdq2pd    xmm1,xmm1
  000000014002E171: F2 0F 5C 0D 9F 33  subsd       xmm1,mmword ptr [0000000142101518h]
                    0D 02
  000000014002E179: F2 0F 10 84 24 80  movsd       xmm0,mmword ptr [rsp+0000000000000180h]
                    01 00 00
  000000014002E182: F2 0F 59 C1        mulsd       xmm0,xmm1
  000000014002E186: F2 0F 5E 05 8A 33  divsd       xmm0,mmword ptr [0000000142101518h]
                    0D 02
  000000014002E18E: 66 0F 28 CE        movapd      xmm1,xmm6
  000000014002E192: F2 0F 58 C8        addsd       xmm1,xmm0
  000000014002E196: 66 0F 28 C7        movapd      xmm0,xmm7
  000000014002E19A: F2 0F 59 C1        mulsd       xmm0,xmm1
  000000014002E19E: F2 0F 2C C0        cvttsd2si   eax,xmm0
  000000014002E1A2: 89 44 24 34        mov         dword ptr [rsp+34h],eax
  000000014002E1A6: 48 8D 8C 24 C8 00  lea         rcx,[rsp+00000000000000C8h]
                    00 00
  000000014002E1AE: FF 15 74 DD DD 01  call        qword ptr [0000000141E0BF28h]
  000000014002E1B4: 48 8B 8C 24 60 01  mov         rcx,qword ptr [rsp+0000000000000160h]
                    00 00
  000000014002E1BC: FF 15 3E D8 DD 01  call        qword ptr [0000000141E0BA00h]
  000000014002E1C2: 48 8D 94 24 D8 00  lea         rdx,[rsp+00000000000000D8h]
                    00 00
  000000014002E1CA: 48 8B C8           mov         rcx,rax
  000000014002E1CD: FF 15 5D 5D DD 01  call        qword ptr [0000000141E03F30h]
  000000014002E1D3: 48 8B C8           mov         rcx,rax
  000000014002E1D6: FF 15 5C 5D DD 01  call        qword ptr [0000000141E03F38h]
  000000014002E1DC: 8B D8              mov         ebx,eax
  000000014002E1DE: 8B 44 24 34        mov         eax,dword ptr [rsp+34h]
  000000014002E1E2: 99                 cdq
  000000014002E1E3: 2B C2              sub         eax,edx
  000000014002E1E5: D1 F8              sar         eax,1
  000000014002E1E7: 2B D8              sub         ebx,eax
  000000014002E1E9: 48 8B 8C 24 60 01  mov         rcx,qword ptr [rsp+0000000000000160h]
                    00 00
  000000014002E1F1: FF 15 09 D8 DD 01  call        qword ptr [0000000141E0BA00h]
  000000014002E1F7: 48 8D 94 24 E0 00  lea         rdx,[rsp+00000000000000E0h]
                    00 00
  000000014002E1FF: 48 8B C8           mov         rcx,rax
  000000014002E202: FF 15 28 5D DD 01  call        qword ptr [0000000141E03F30h]
  000000014002E208: 48 8B C8           mov         rcx,rax
  000000014002E20B: FF 15 2F 5D DD 01  call        qword ptr [0000000141E03F40h]
  000000014002E211: 44 8B D8           mov         r11d,eax
  000000014002E214: 8B 44 24 30        mov         eax,dword ptr [rsp+30h]
  000000014002E218: 99                 cdq
  000000014002E219: 2B C2              sub         eax,edx
  000000014002E21B: D1 F8              sar         eax,1
  000000014002E21D: 41 8B D3           mov         edx,r11d
  000000014002E220: 2B D0              sub         edx,eax
  000000014002E222: 8B 44 24 34        mov         eax,dword ptr [rsp+34h]
  000000014002E226: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  000000014002E22A: 44 8B 4C 24 30     mov         r9d,dword ptr [rsp+30h]
  000000014002E22F: 44 8B C3           mov         r8d,ebx
  000000014002E232: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000014002E237: FF 15 33 D7 DD 01  call        qword ptr [0000000141E0B970h]
  000000014002E23D: 44 8B 44 24 34     mov         r8d,dword ptr [rsp+34h]
  000000014002E242: 8B 54 24 30        mov         edx,dword ptr [rsp+30h]
  000000014002E246: 48 8D 8C 24 E8 00  lea         rcx,[rsp+00000000000000E8h]
                    00 00
  000000014002E24E: FF 15 54 5E DD 01  call        qword ptr [0000000141E040A8h]
  000000014002E254: 48 8B D0           mov         rdx,rax
  000000014002E257: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000014002E25C: FF 15 B6 D7 DD 01  call        qword ptr [0000000141E0BA18h]
  000000014002E262: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  000000014002E26A: 4C 8D 0D CF 26 0D  lea         r9,[0000000142100940h]
                    02
  000000014002E271: 4C 8D 44 24 48     lea         r8,[rsp+48h]
  000000014002E276: 48 8D 15 D3 26 0D  lea         rdx,[0000000142100950h]
                    02
  000000014002E27D: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  000000014002E282: FF 15 48 5F DD 01  call        qword ptr [0000000141E041D0h]
  000000014002E288: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000014002E28D: FF 15 65 D7 DD 01  call        qword ptr [0000000141E0B9F8h]
  000000014002E293: 90                 nop
  000000014002E294: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000014002E299: FF 15 D9 D6 DD 01  call        qword ptr [0000000141E0B978h]
  000000014002E29F: 0F 28 B4 24 40 01  movaps      xmm6,xmmword ptr [rsp+0000000000000140h]
                    00 00
  000000014002E2A7: 0F 28 BC 24 30 01  movaps      xmm7,xmmword ptr [rsp+0000000000000130h]
                    00 00
  000000014002E2AF: 48 81 C4 50 01 00  add         rsp,150h
                    00
  000000014002E2B6: 5B                 pop         rbx
  000000014002E2B7: C3                 ret

  Summary

     1E02000 .text
