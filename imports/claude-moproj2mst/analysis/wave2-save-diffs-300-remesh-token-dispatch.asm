Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\gui_mo\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001409068F0: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  00000001409068F5: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  00000001409068FA: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001409068FF: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140906904: 48 83 EC 68        sub         rsp,68h
  0000000140906908: 83 BC 24 80 00 00  cmp         dword ptr [rsp+0000000000000080h],0Ch
                    00 0C
  0000000140906910: 0F 85 A1 00 00 00  jne         00000001409069B7
  0000000140906916: 4C 8B 44 24 78     mov         r8,qword ptr [rsp+78h]
  000000014090691B: BA D0 07 00 00     mov         edx,7D0h
  0000000140906920: 48 8D 0D 39 1D 4D  lea         rcx,[0000000143DD8660h]
                    03
  0000000140906927: E8 80 CA B6 00     call        00000001414733AC
  000000014090692C: 48 85 C0           test        rax,rax
  000000014090692F: 75 27              jne         0000000140906958
  0000000140906931: 48 8D 15 40 35 EB  lea         rdx,[00000001427B9E78h]
                    01
  0000000140906938: 48 8B 8C 24 88 00  mov         rcx,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140906940: E8 7B 95 B6 00     call        000000014146FEC0
  0000000140906945: C7 44 24 40 FE FF  mov         dword ptr [rsp+40h],0FFFFFFFEh
                    FF FF
  000000014090694D: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000140906951: E9 A5 04 00 00     jmp         0000000140906DFB
  0000000140906956: EB 5A              jmp         00000001409069B2
  0000000140906958: 48 8D 15 21 35 EB  lea         rdx,[00000001427B9E80h]
                    01
  000000014090695F: 48 8D 0D FA 1C 4D  lea         rcx,[0000000143DD8660h]
                    03
  0000000140906966: E8 05 BF B6 00     call        0000000141472870
  000000014090696B: 48 89 05 E6 18 4D  mov         qword ptr [0000000143DD8258h],rax
                    03
  0000000140906972: 48 83 3D DE 18 4D  cmp         qword ptr [0000000143DD8258h],0
                    03 00
  000000014090697A: 75 0E              jne         000000014090698A
  000000014090697C: 48 8D 05 DD 1C 4D  lea         rax,[0000000143DD8660h]
                    03
  0000000140906983: 48 89 05 CE 18 4D  mov         qword ptr [0000000143DD8258h],rax
                    03
  000000014090698A: 48 8B 05 C7 18 4D  mov         rax,qword ptr [0000000143DD8258h]
                    03
  0000000140906991: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140906996: C7 05 C8 8E F6 02  mov         dword ptr [000000014386F868h],2
                    02 00 00 00
  00000001409069A0: C7 05 A2 18 4D 03  mov         dword ptr [0000000143DD824Ch],1
                    01 00 00 00
  00000001409069AA: C7 44 24 40 00 00  mov         dword ptr [rsp+40h],0
                    00 00
  00000001409069B2: E9 45 02 00 00     jmp         0000000140906BFC
  00000001409069B7: 83 BC 24 90 00 00  cmp         dword ptr [rsp+0000000000000090h],0
                    00 00
  00000001409069BF: 74 5E              je          0000000140906A1F
  00000001409069C1: 81 BC 24 90 00 00  cmp         dword ptr [rsp+0000000000000090h],0FFFFEC78h
                    00 78 EC FF FF
  00000001409069CC: 74 51              je          0000000140906A1F
  00000001409069CE: 83 BC 24 90 00 00  cmp         dword ptr [rsp+0000000000000090h],1
                    00 01
  00000001409069D6: 7C 31              jl          0000000140906A09
  00000001409069D8: 8B 05 6E 18 4D 03  mov         eax,dword ptr [0000000143DD824Ch]
  00000001409069DE: 39 84 24 90 00 00  cmp         dword ptr [rsp+0000000000000090h],eax
                    00
  00000001409069E5: 7F 22              jg          0000000140906A09
  00000001409069E7: 48 63 8C 24 90 00  movsxd      rcx,dword ptr [rsp+0000000000000090h]
                    00 00
  00000001409069EF: 48 8D 05 5A 18 4D  lea         rax,[0000000143DD8250h]
                    03
  00000001409069F6: 48 8B 04 C8        mov         rax,qword ptr [rax+rcx*8]
  00000001409069FA: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001409069FF: C7 44 24 40 00 00  mov         dword ptr [rsp+40h],0
                    00 00
  0000000140906A07: EB 11              jmp         0000000140906A1A
  0000000140906A09: C7 44 24 40 FF FF  mov         dword ptr [rsp+40h],0FFFFFFFFh
                    FF FF
  0000000140906A11: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000140906A15: E9 E1 03 00 00     jmp         0000000140906DFB
  0000000140906A1A: E9 DD 01 00 00     jmp         0000000140906BFC
  0000000140906A1F: 8B 05 27 18 4D 03  mov         eax,dword ptr [0000000143DD824Ch]
  0000000140906A25: 39 05 3D 8E F6 02  cmp         dword ptr [000000014386F868h],eax
  0000000140906A2B: 7F 40              jg          0000000140906A6D
  0000000140906A2D: 81 BC 24 90 00 00  cmp         dword ptr [rsp+0000000000000090h],0FFFFEC78h
                    00 78 EC FF FF
  0000000140906A38: 74 33              je          0000000140906A6D
  0000000140906A3A: 48 63 0D 27 8E F6  movsxd      rcx,dword ptr [000000014386F868h]
                    02
  0000000140906A41: 48 8D 05 08 18 4D  lea         rax,[0000000143DD8250h]
                    03
  0000000140906A48: 48 8B 04 C8        mov         rax,qword ptr [rax+rcx*8]
  0000000140906A4C: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140906A51: 8B 05 11 8E F6 02  mov         eax,dword ptr [000000014386F868h]
  0000000140906A57: 83 C0 01           add         eax,1
  0000000140906A5A: 89 05 08 8E F6 02  mov         dword ptr [000000014386F868h],eax
  0000000140906A60: C7 44 24 40 00 00  mov         dword ptr [rsp+40h],0
                    00 00
  0000000140906A68: E9 8F 01 00 00     jmp         0000000140906BFC
  0000000140906A6D: 81 BC 24 90 00 00  cmp         dword ptr [rsp+0000000000000090h],0FFFFEC78h
                    00 78 EC FF FF
  0000000140906A78: 75 0A              jne         0000000140906A84
  0000000140906A7A: C7 44 24 40 00 00  mov         dword ptr [rsp+40h],0
                    00 00
  0000000140906A82: EB 21              jmp         0000000140906AA5
  0000000140906A84: 8B 05 C2 17 4D 03  mov         eax,dword ptr [0000000143DD824Ch]
  0000000140906A8A: 39 84 24 90 00 00  cmp         dword ptr [rsp+0000000000000090h],eax
                    00
  0000000140906A91: 7F 0A              jg          0000000140906A9D
  0000000140906A93: C7 44 24 40 00 00  mov         dword ptr [rsp+40h],0
                    00 00
  0000000140906A9B: EB 08              jmp         0000000140906AA5
  0000000140906A9D: C7 44 24 40 FF FF  mov         dword ptr [rsp+40h],0FFFFFFFFh
                    FF FF
  0000000140906AA5: 4C 8B 44 24 78     mov         r8,qword ptr [rsp+78h]
  0000000140906AAA: BA D0 07 00 00     mov         edx,7D0h
  0000000140906AAF: 48 8D 0D AA 1B 4D  lea         rcx,[0000000143DD8660h]
                    03
  0000000140906AB6: E8 F1 C8 B6 00     call        00000001414733AC
  0000000140906ABB: 48 85 C0           test        rax,rax
  0000000140906ABE: 75 2F              jne         0000000140906AEF
  0000000140906AC0: 83 BC 24 80 00 00  cmp         dword ptr [rsp+0000000000000080h],2
                    00 02
  0000000140906AC8: 75 14              jne         0000000140906ADE
  0000000140906ACA: 48 8D 15 B3 33 EB  lea         rdx,[00000001427B9E84h]
                    01
  0000000140906AD1: 48 8B 8C 24 88 00  mov         rcx,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140906AD9: E8 E2 93 B6 00     call        000000014146FEC0
  0000000140906ADE: C7 44 24 40 FE FF  mov         dword ptr [rsp+40h],0FFFFFFFEh
                    FF FF
  0000000140906AE6: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000140906AEA: E9 0C 03 00 00     jmp         0000000140906DFB
  0000000140906AEF: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  0000000140906AF4: 83 38 01           cmp         dword ptr [rax],1
  0000000140906AF7: 75 07              jne         0000000140906B00
  0000000140906AF9: C6 05 9F 1B 4D 03  mov         byte ptr [0000000143DD869Fh],0
                    00
  0000000140906B00: 83 BC 24 80 00 00  cmp         dword ptr [rsp+0000000000000080h],4
                    00 04
  0000000140906B08: 74 44              je          0000000140906B4E
  0000000140906B0A: 48 8D 15 7B 33 EB  lea         rdx,[00000001427B9E8Ch]
                    01
  0000000140906B11: 48 8D 0D 48 1B 4D  lea         rcx,[0000000143DD8660h]
                    03
  0000000140906B18: E8 C3 B7 7A FF     call        00000001400B22E0
  0000000140906B1D: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140906B22: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  0000000140906B28: 74 08              je          0000000140906B32
  0000000140906B2A: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140906B2F: C6 00 00           mov         byte ptr [rax],0
  0000000140906B32: 48 8D 15 57 33 EB  lea         rdx,[00000001427B9E90h]
                    01
  0000000140906B39: 48 8D 0D 20 1B 4D  lea         rcx,[0000000143DD8660h]
                    03
  0000000140906B40: E8 2B BD B6 00     call        0000000141472870
  0000000140906B45: 48 89 05 0C 17 4D  mov         qword ptr [0000000143DD8258h],rax
                    03
  0000000140906B4C: EB 1A              jmp         0000000140906B68
  0000000140906B4E: 48 8D 15 43 33 EB  lea         rdx,[00000001427B9E98h]
                    01
  0000000140906B55: 48 8D 0D 04 1B 4D  lea         rcx,[0000000143DD8660h]
                    03
  0000000140906B5C: E8 0F BD B6 00     call        0000000141472870
  0000000140906B61: 48 89 05 F0 16 4D  mov         qword ptr [0000000143DD8258h],rax
                    03
  0000000140906B68: C7 05 DA 16 4D 03  mov         dword ptr [0000000143DD824Ch],1
                    01 00 00 00
  0000000140906B72: 48 8D 15 27 33 EB  lea         rdx,[00000001427B9EA0h]
                    01
  0000000140906B79: 33 C9              xor         ecx,ecx
  0000000140906B7B: E8 F0 BC B6 00     call        0000000141472870
  0000000140906B80: 4C 8B D8           mov         r11,rax
  0000000140906B83: 8B 05 C3 16 4D 03  mov         eax,dword ptr [0000000143DD824Ch]
  0000000140906B89: 83 C0 01           add         eax,1
  0000000140906B8C: 48 63 C8           movsxd      rcx,eax
  0000000140906B8F: 48 8D 05 BA 16 4D  lea         rax,[0000000143DD8250h]
                    03
  0000000140906B96: 4C 89 1C C8        mov         qword ptr [rax+rcx*8],r11
  0000000140906B9A: 8B 05 AC 16 4D 03  mov         eax,dword ptr [0000000143DD824Ch]
  0000000140906BA0: 83 C0 01           add         eax,1
  0000000140906BA3: 48 63 C8           movsxd      rcx,eax
  0000000140906BA6: 48 8D 05 A3 16 4D  lea         rax,[0000000143DD8250h]
                    03
  0000000140906BAD: 48 83 3C C8 00     cmp         qword ptr [rax+rcx*8],0
  0000000140906BB2: 74 11              je          0000000140906BC5
  0000000140906BB4: 8B 05 92 16 4D 03  mov         eax,dword ptr [0000000143DD824Ch]
  0000000140906BBA: 83 C0 01           add         eax,1
  0000000140906BBD: 89 05 89 16 4D 03  mov         dword ptr [0000000143DD824Ch],eax
  0000000140906BC3: EB AD              jmp         0000000140906B72
  0000000140906BC5: 48 83 3D 8B 16 4D  cmp         qword ptr [0000000143DD8258h],0
                    03 00
  0000000140906BCD: 0F 84 D2 FE FF FF  je          0000000140906AA5
  0000000140906BD3: 48 8B 05 7E 16 4D  mov         rax,qword ptr [0000000143DD8258h]
                    03
  0000000140906BDA: 0F BE 00           movsx       eax,byte ptr [rax]
  0000000140906BDD: 83 F8 2A           cmp         eax,2Ah
  0000000140906BE0: 0F 84 BF FE FF FF  je          0000000140906AA5
  0000000140906BE6: 48 8B 05 6B 16 4D  mov         rax,qword ptr [0000000143DD8258h]
                    03
  0000000140906BED: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140906BF2: C7 05 6C 8C F6 02  mov         dword ptr [000000014386F868h],2
                    02 00 00 00
  0000000140906BFC: 8B 84 24 80 00 00  mov         eax,dword ptr [rsp+0000000000000080h]
                    00
  0000000140906C03: 89 44 24 50        mov         dword ptr [rsp+50h],eax
  0000000140906C07: 8B 44 24 50        mov         eax,dword ptr [rsp+50h]
  0000000140906C0B: 83 E8 01           sub         eax,1
  0000000140906C0E: 89 44 24 50        mov         dword ptr [rsp+50h],eax
  0000000140906C12: 83 7C 24 50 0B     cmp         dword ptr [rsp+50h],0Bh
  0000000140906C17: 0F 87 C3 01 00 00  ja          0000000140906DE0
  0000000140906C1D: 48 63 44 24 50     movsxd      rax,dword ptr [rsp+50h]
  0000000140906C22: 48 8D 0D D7 93 6F  lea         rcx,[0000000140000000h]
                    FF
  0000000140906C29: 8B 84 81 00 6E 90  mov         eax,dword ptr [rcx+rax*4+0000000000906E00h]
                    00
  0000000140906C30: 48 03 C1           add         rax,rcx
  0000000140906C33: FF E0              jmp         rax
  0000000140906C35: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140906C3A: E8 6D CB B6 00     call        00000001414737AC
  0000000140906C3F: 85 C0              test        eax,eax
  0000000140906C41: 74 0A              je          0000000140906C4D
  0000000140906C43: C7 44 24 54 01 00  mov         dword ptr [rsp+54h],1
                    00 00
  0000000140906C4B: EB 08              jmp         0000000140906C55
  0000000140906C4D: C7 44 24 54 00 00  mov         dword ptr [rsp+54h],0
                    00 00
  0000000140906C55: 48 8B 8C 24 88 00  mov         rcx,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140906C5D: 0F B6 44 24 54     movzx       eax,byte ptr [rsp+54h]
  0000000140906C62: 88 01              mov         byte ptr [rcx],al
  0000000140906C64: E9 77 01 00 00     jmp         0000000140906DE0
  0000000140906C69: 48 8B 54 24 30     mov         rdx,qword ptr [rsp+30h]
  0000000140906C6E: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140906C73: E8 08 FC FF FF     call        0000000140906880
  0000000140906C78: 48 8B 54 24 30     mov         rdx,qword ptr [rsp+30h]
  0000000140906C7D: 48 8B 8C 24 88 00  mov         rcx,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140906C85: E8 36 92 B6 00     call        000000014146FEC0
  0000000140906C8A: E9 51 01 00 00     jmp         0000000140906DE0
  0000000140906C8F: 48 8B 54 24 30     mov         rdx,qword ptr [rsp+30h]
  0000000140906C94: 48 8B 8C 24 88 00  mov         rcx,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140906C9C: E8 1F 92 B6 00     call        000000014146FEC0
  0000000140906CA1: E9 3A 01 00 00     jmp         0000000140906DE0
  0000000140906CA6: 48 8B 54 24 30     mov         rdx,qword ptr [rsp+30h]
  0000000140906CAB: 48 8B 8C 24 88 00  mov         rcx,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140906CB3: E8 08 92 B6 00     call        000000014146FEC0
  0000000140906CB8: E9 23 01 00 00     jmp         0000000140906DE0
  0000000140906CBD: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140906CC2: E8 E5 CA B6 00     call        00000001414737AC
  0000000140906CC7: 44 8B D8           mov         r11d,eax
  0000000140906CCA: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140906CD2: 44 89 18           mov         dword ptr [rax],r11d
  0000000140906CD5: E9 06 01 00 00     jmp         0000000140906DE0
  0000000140906CDA: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140906CDF: E8 C8 CA B6 00     call        00000001414737AC
  0000000140906CE4: 44 8B D8           mov         r11d,eax
  0000000140906CE7: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140906CEF: 44 89 18           mov         dword ptr [rax],r11d
  0000000140906CF2: E9 E9 00 00 00     jmp         0000000140906DE0
  0000000140906CF7: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140906CFC: E8 AB CA B6 00     call        00000001414737AC
  0000000140906D01: 44 8B D8           mov         r11d,eax
  0000000140906D04: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140906D0C: 44 89 18           mov         dword ptr [rax],r11d
  0000000140906D0F: E9 CC 00 00 00     jmp         0000000140906DE0
  0000000140906D14: 41 B8 0A 00 00 00  mov         r8d,0Ah
  0000000140906D1A: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  0000000140906D1F: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140906D24: E8 4B EF B6 00     call        0000000141475C74
  0000000140906D29: 44 8B D8           mov         r11d,eax
  0000000140906D2C: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140906D34: 44 89 18           mov         dword ptr [rax],r11d
  0000000140906D37: E9 A4 00 00 00     jmp         0000000140906DE0
  0000000140906D3C: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140906D44: 41 B1 45           mov         r9b,45h
  0000000140906D47: 41 B0 44           mov         r8b,44h
  0000000140906D4A: 48 8B 54 24 30     mov         rdx,qword ptr [rsp+30h]
  0000000140906D4F: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140906D54: E8 37 FA FF FF     call        0000000140906790
  0000000140906D59: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140906D5E: E8 41 CA B6 00     call        00000001414737A4
  0000000140906D63: F2 0F 5A C0        cvtsd2ss    xmm0,xmm0
  0000000140906D67: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140906D6F: F3 0F 11 00        movss       dword ptr [rax],xmm0
  0000000140906D73: EB 6B              jmp         0000000140906DE0
  0000000140906D75: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140906D7D: 41 B1 45           mov         r9b,45h
  0000000140906D80: 41 B0 44           mov         r8b,44h
  0000000140906D83: 48 8B 54 24 30     mov         rdx,qword ptr [rsp+30h]
  0000000140906D88: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140906D8D: E8 FE F9 FF FF     call        0000000140906790
  0000000140906D92: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  0000000140906D97: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140906D9C: E8 17 EC B6 00     call        00000001414759B8
  0000000140906DA1: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140906DA9: F2 0F 11 00        movsd       mmword ptr [rax],xmm0
  0000000140906DAD: EB 31              jmp         0000000140906DE0
  0000000140906DAF: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140906DB7: 41 B1 45           mov         r9b,45h
  0000000140906DBA: 41 B0 44           mov         r8b,44h
  0000000140906DBD: 48 8B 54 24 30     mov         rdx,qword ptr [rsp+30h]
  0000000140906DC2: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140906DC7: E8 C4 F9 FF FF     call        0000000140906790
  0000000140906DCC: EB 12              jmp         0000000140906DE0
  0000000140906DCE: 48 8B 54 24 30     mov         rdx,qword ptr [rsp+30h]
  0000000140906DD3: 48 8B 8C 24 88 00  mov         rcx,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140906DDB: E8 E0 90 B6 00     call        000000014146FEC0
  0000000140906DE0: 81 BC 24 90 00 00  cmp         dword ptr [rsp+0000000000000090h],0FFFFEC78h
                    00 78 EC FF FF
  0000000140906DEB: 75 0A              jne         0000000140906DF7
  0000000140906DED: 8B
  0000000140906DEE: 05
  0000000140906DEF: 59                 pop         rcx
  0000000140906DF0: 14

  Summary

     1E02000 .text
