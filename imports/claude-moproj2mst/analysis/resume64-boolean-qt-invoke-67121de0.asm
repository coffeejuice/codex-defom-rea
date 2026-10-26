Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067121DE0: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  0000000067121DE5: 55                 push        rbp
  0000000067121DE6: 56                 push        rsi
  0000000067121DE7: 57                 push        rdi
  0000000067121DE8: 41 54              push        r12
  0000000067121DEA: 41 55              push        r13
  0000000067121DEC: 41 56              push        r14
  0000000067121DEE: 41 57              push        r15
  0000000067121DF0: 48 81 EC E0 03 00  sub         rsp,3E0h
                    00
  0000000067121DF7: 48 C7 84 24 B0 01  mov         qword ptr [rsp+00000000000001B0h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  0000000067121E03: 48 89 9C 24 28 04  mov         qword ptr [rsp+0000000000000428h],rbx
                    00 00
  0000000067121E0B: 4D 8B F9           mov         r15,r9
  0000000067121E0E: 48 8B DA           mov         rbx,rdx
  0000000067121E11: 4C 8B E1           mov         r12,rcx
  0000000067121E14: 48 85 C9           test        rcx,rcx
  0000000067121E17: 75 07              jne         0000000067121E20
  0000000067121E19: 32 C0              xor         al,al
  0000000067121E1B: E9 E7 06 00 00     jmp         0000000067122507
  0000000067121E20: 33 ED              xor         ebp,ebp
  0000000067121E22: 8B D5              mov         edx,ebp
  0000000067121E24: 89 94 24 C4 01 00  mov         dword ptr [rsp+00000000000001C4h],edx
                    00
  0000000067121E2B: 4C 8D 8C 24 D0 01  lea         r9,[rsp+00000000000001D0h]
                    00 00
  0000000067121E33: 4C 89 8C 24 C8 01  mov         qword ptr [rsp+00000000000001C8h],r9
                    00 00
  0000000067121E3B: C7 84 24 C0 01 00  mov         dword ptr [rsp+00000000000001C0h],200h
                    00 00 02 00 00
  0000000067121E46: 48 85 DB           test        rbx,rbx
  0000000067121E49: 0F 84 B6 06 00 00  je          0000000067122505
  0000000067121E4F: 33 C0              xor         eax,eax
  0000000067121E51: 49 83 CE FF        or          r14,0FFFFFFFFFFFFFFFFh
  0000000067121E55: 49 8B CE           mov         rcx,r14
  0000000067121E58: 48 8B FB           mov         rdi,rbx
  0000000067121E5B: F2 AE              repne scas  byte ptr [rdi]
  0000000067121E5D: 48 F7 D1           not         rcx
  0000000067121E60: 48 FF C9           dec         rcx
  0000000067121E63: 48 8B F9           mov         rdi,rcx
  0000000067121E66: 85 C9              test        ecx,ecx
  0000000067121E68: 0F 8E 97 06 00 00  jle         0000000067122505
  0000000067121E6E: 89 8C 24 20 04 00  mov         dword ptr [rsp+0000000000000420h],ecx
                    00
  0000000067121E75: 81 F9 00 02 00 00  cmp         ecx,200h
  0000000067121E7B: 7C 3E              jl          0000000067121EBB
  0000000067121E7D: 89 AC 24 88 00 00  mov         dword ptr [rsp+0000000000000088h],ebp
                    00
  0000000067121E84: 48 8D 84 24 20 04  lea         rax,[rsp+0000000000000420h]
                    00 00
  0000000067121E8C: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  0000000067121E94: 85 FF              test        edi,edi
  0000000067121E96: 48 0F 4E C1        cmovle      rax,rcx
  0000000067121E9A: 44 8B 00           mov         r8d,dword ptr [rax]
  0000000067121E9D: 33 D2              xor         edx,edx
  0000000067121E9F: 48 8D 8C 24 C0 01  lea         rcx,[rsp+00000000000001C0h]
                    00 00
  0000000067121EA7: E8 84 2C EE FF     call        0000000067004B30
  0000000067121EAC: 4C 8B 8C 24 C8 01  mov         r9,qword ptr [rsp+00000000000001C8h]
                    00 00
  0000000067121EB4: 8B 94 24 C4 01 00  mov         edx,dword ptr [rsp+00000000000001C4h]
                    00
  0000000067121EBB: 4C 63 C7           movsxd      r8,edi
  0000000067121EBE: 48 63 CA           movsxd      rcx,edx
  0000000067121EC1: 49 03 C9           add         rcx,r9
  0000000067121EC4: 48 8B D3           mov         rdx,rbx
  0000000067121EC7: E8 84 A1 EF FF     call        000000006701C050
  0000000067121ECC: 89 BC 24 C4 01 00  mov         dword ptr [rsp+00000000000001C4h],edi
                    00
  0000000067121ED3: 3B BC 24 C0 01 00  cmp         edi,dword ptr [rsp+00000000000001C0h]
                    00
  0000000067121EDA: 75 1A              jne         0000000067121EF6
  0000000067121EDC: 44 8D 04 3F        lea         r8d,[rdi+rdi]
  0000000067121EE0: 8B D7              mov         edx,edi
  0000000067121EE2: 48 8D 8C 24 C0 01  lea         rcx,[rsp+00000000000001C0h]
                    00 00
  0000000067121EEA: E8 41 2C EE FF     call        0000000067004B30
  0000000067121EEF: 8B BC 24 C4 01 00  mov         edi,dword ptr [rsp+00000000000001C4h]
                    00
  0000000067121EF6: 48 63 CF           movsxd      rcx,edi
  0000000067121EF9: FF C7              inc         edi
  0000000067121EFB: 89 BC 24 C4 01 00  mov         dword ptr [rsp+00000000000001C4h],edi
                    00
  0000000067121F02: 48 8B 84 24 C8 01  mov         rax,qword ptr [rsp+00000000000001C8h]
                    00 00
  0000000067121F0A: C6 04 01 28        mov         byte ptr [rcx+rax],28h
  0000000067121F0E: 49 8B 47 08        mov         rax,qword ptr [r15+8]
  0000000067121F12: 48 89 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],rax
                    00 00
  0000000067121F1A: 48 8B 84 24 40 04  mov         rax,qword ptr [rsp+0000000000000440h]
                    00 00
  0000000067121F22: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  0000000067121F26: 48 89 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],rax
                    00 00
  0000000067121F2E: 48 8B 84 24 48 04  mov         rax,qword ptr [rsp+0000000000000448h]
                    00 00
  0000000067121F36: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  0000000067121F3A: 48 89 84 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],rax
                    00 00
  0000000067121F42: 48 8B 84 24 50 04  mov         rax,qword ptr [rsp+0000000000000450h]
                    00 00
  0000000067121F4A: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  0000000067121F4E: 48 89 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],rax
                    00 00
  0000000067121F56: 48 8B 84 24 58 04  mov         rax,qword ptr [rsp+0000000000000458h]
                    00 00
  0000000067121F5E: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  0000000067121F62: 48 89 84 24 C0 00  mov         qword ptr [rsp+00000000000000C0h],rax
                    00 00
  0000000067121F6A: 48 8B 84 24 60 04  mov         rax,qword ptr [rsp+0000000000000460h]
                    00 00
  0000000067121F72: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  0000000067121F76: 48 89 84 24 C8 00  mov         qword ptr [rsp+00000000000000C8h],rax
                    00 00
  0000000067121F7E: 48 8B 84 24 68 04  mov         rax,qword ptr [rsp+0000000000000468h]
                    00 00
  0000000067121F86: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  0000000067121F8A: 48 89 84 24 D0 00  mov         qword ptr [rsp+00000000000000D0h],rax
                    00 00
  0000000067121F92: 48 8B 84 24 70 04  mov         rax,qword ptr [rsp+0000000000000470h]
                    00 00
  0000000067121F9A: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  0000000067121F9E: 48 89 84 24 D8 00  mov         qword ptr [rsp+00000000000000D8h],rax
                    00 00
  0000000067121FA6: 48 8B 84 24 78 04  mov         rax,qword ptr [rsp+0000000000000478h]
                    00 00
  0000000067121FAE: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  0000000067121FB2: 48 89 84 24 E0 00  mov         qword ptr [rsp+00000000000000E0h],rax
                    00 00
  0000000067121FBA: 48 8B 84 24 80 04  mov         rax,qword ptr [rsp+0000000000000480h]
                    00 00
  0000000067121FC2: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  0000000067121FC6: 48 89 84 24 E8 00  mov         qword ptr [rsp+00000000000000E8h],rax
                    00 00
  0000000067121FCE: 48 8B 84 24 88 04  mov         rax,qword ptr [rsp+0000000000000488h]
                    00 00
  0000000067121FD6: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  0000000067121FDA: 48 89 84 24 F0 00  mov         qword ptr [rsp+00000000000000F0h],rax
                    00 00
  0000000067121FE2: 41 BD 01 00 00 00  mov         r13d,1
  0000000067121FE8: 49 8B DD           mov         rbx,r13
  0000000067121FEB: 0F 1F 44 00 00     nop         dword ptr [rax+rax]
  0000000067121FF0: 48 8B B4 DC A0 00  mov         rsi,qword ptr [rsp+rbx*8+00000000000000A0h]
                    00 00
  0000000067121FF8: 48 85 F6           test        rsi,rsi
  0000000067121FFB: 0F 84 D4 00 00 00  je          00000000671220D5
  0000000067122001: 33 C0              xor         eax,eax
  0000000067122003: 49 8B CE           mov         rcx,r14
  0000000067122006: 48 8B FE           mov         rdi,rsi
  0000000067122009: F2 AE              repne scas  byte ptr [rdi]
  000000006712200B: 48 F7 D1           not         rcx
  000000006712200E: 48 FF C9           dec         rcx
  0000000067122011: 48 8B E9           mov         rbp,rcx
  0000000067122014: 85 C9              test        ecx,ecx
  0000000067122016: 0F 8E B9 00 00 00  jle         00000000671220D5
  000000006712201C: 8B 94 24 C4 01 00  mov         edx,dword ptr [rsp+00000000000001C4h]
                    00
  0000000067122023: 8D 3C 0A           lea         edi,[rdx+rcx]
  0000000067122026: 89 BC 24 84 00 00  mov         dword ptr [rsp+0000000000000084h],edi
                    00
  000000006712202D: 3B BC 24 C0 01 00  cmp         edi,dword ptr [rsp+00000000000001C0h]
                    00
  0000000067122034: 7C 37              jl          000000006712206D
  0000000067122036: 8D 04 12           lea         eax,[rdx+rdx]
  0000000067122039: 89 84 24 80 00 00  mov         dword ptr [rsp+0000000000000080h],eax
                    00
  0000000067122040: 48 8D 8C 24 84 00  lea         rcx,[rsp+0000000000000084h]
                    00 00
  0000000067122048: 4C 8D 84 24 80 00  lea         r8,[rsp+0000000000000080h]
                    00 00
  0000000067122050: 3B C7              cmp         eax,edi
  0000000067122052: 49 0F 4D C8        cmovge      rcx,r8
  0000000067122056: 44 8B 01           mov         r8d,dword ptr [rcx]
  0000000067122059: 48 8D 8C 24 C0 01  lea         rcx,[rsp+00000000000001C0h]
                    00 00
  0000000067122061: E8 CA 2A EE FF     call        0000000067004B30
  0000000067122066: 8B 94 24 C4 01 00  mov         edx,dword ptr [rsp+00000000000001C4h]
                    00
  000000006712206D: 4C 63 C5           movsxd      r8,ebp
  0000000067122070: 48 63 CA           movsxd      rcx,edx
  0000000067122073: 48 03 8C 24 C8 01  add         rcx,qword ptr [rsp+00000000000001C8h]
                    00 00
  000000006712207B: 48 8B D6           mov         rdx,rsi
  000000006712207E: E8 CD 9F EF FF     call        000000006701C050
  0000000067122083: 89 BC 24 C4 01 00  mov         dword ptr [rsp+00000000000001C4h],edi
                    00
  000000006712208A: 3B BC 24 C0 01 00  cmp         edi,dword ptr [rsp+00000000000001C0h]
                    00
  0000000067122091: 75 1A              jne         00000000671220AD
  0000000067122093: 44 8D 04 3F        lea         r8d,[rdi+rdi]
  0000000067122097: 8B D7              mov         edx,edi
  0000000067122099: 48 8D 8C 24 C0 01  lea         rcx,[rsp+00000000000001C0h]
                    00 00
  00000000671220A1: E8 8A 2A EE FF     call        0000000067004B30
  00000000671220A6: 8B BC 24 C4 01 00  mov         edi,dword ptr [rsp+00000000000001C4h]
                    00
  00000000671220AD: 48 63 CF           movsxd      rcx,edi
  00000000671220B0: FF C7              inc         edi
  00000000671220B2: 89 BC 24 C4 01 00  mov         dword ptr [rsp+00000000000001C4h],edi
                    00
  00000000671220B9: 48 8B 84 24 C8 01  mov         rax,qword ptr [rsp+00000000000001C8h]
                    00 00
  00000000671220C1: C6 04 01 2C        mov         byte ptr [rcx+rax],2Ch
  00000000671220C5: 41 FF C5           inc         r13d
  00000000671220C8: 48 FF C3           inc         rbx
  00000000671220CB: 48 83 FB 0B        cmp         rbx,0Bh
  00000000671220CF: 0F 8C 1B FF FF FF  jl          0000000067121FF0
  00000000671220D5: 41 83 FD 01        cmp         r13d,1
  00000000671220D9: 75 47              jne         0000000067122122
  00000000671220DB: 8B 8C 24 C4 01 00  mov         ecx,dword ptr [rsp+00000000000001C4h]
                    00
  00000000671220E2: 3B 8C 24 C0 01 00  cmp         ecx,dword ptr [rsp+00000000000001C0h]
                    00
  00000000671220E9: 75 1A              jne         0000000067122105
  00000000671220EB: 44 8D 04 09        lea         r8d,[rcx+rcx]
  00000000671220EF: 8B D1              mov         edx,ecx
  00000000671220F1: 48 8D 8C 24 C0 01  lea         rcx,[rsp+00000000000001C0h]
                    00 00
  00000000671220F9: E8 32 2A EE FF     call        0000000067004B30
  00000000671220FE: 8B 8C 24 C4 01 00  mov         ecx,dword ptr [rsp+00000000000001C4h]
                    00
  0000000067122105: 48 63 C1           movsxd      rax,ecx
  0000000067122108: FF C1              inc         ecx
  000000006712210A: 89 8C 24 C4 01 00  mov         dword ptr [rsp+00000000000001C4h],ecx
                    00
  0000000067122111: 48 8B C8           mov         rcx,rax
  0000000067122114: 48 8B 84 24 C8 01  mov         rax,qword ptr [rsp+00000000000001C8h]
                    00 00
  000000006712211C: C6 04 01 29        mov         byte ptr [rcx+rax],29h
  0000000067122120: EB 15              jmp         0000000067122137
  0000000067122122: 48 63 8C 24 C4 01  movsxd      rcx,dword ptr [rsp+00000000000001C4h]
                    00 00
  000000006712212A: 48 8B 84 24 C8 01  mov         rax,qword ptr [rsp+00000000000001C8h]
                    00 00
  0000000067122132: C6 44 01 FF 29     mov         byte ptr [rcx+rax-1],29h
  0000000067122137: 8B 8C 24 C4 01 00  mov         ecx,dword ptr [rsp+00000000000001C4h]
                    00
  000000006712213E: 3B 8C 24 C0 01 00  cmp         ecx,dword ptr [rsp+00000000000001C0h]
                    00
  0000000067122145: 75 1A              jne         0000000067122161
  0000000067122147: 44 8D 04 09        lea         r8d,[rcx+rcx]
  000000006712214B: 8B D1              mov         edx,ecx
  000000006712214D: 48 8D 8C 24 C0 01  lea         rcx,[rsp+00000000000001C0h]
                    00 00
  0000000067122155: E8 D6 29 EE FF     call        0000000067004B30
  000000006712215A: 8B 8C 24 C4 01 00  mov         ecx,dword ptr [rsp+00000000000001C4h]
                    00
  0000000067122161: 48 63 C1           movsxd      rax,ecx
  0000000067122164: FF C1              inc         ecx
  0000000067122166: 89 8C 24 C4 01 00  mov         dword ptr [rsp+00000000000001C4h],ecx
                    00
  000000006712216D: 48 8B C8           mov         rcx,rax
  0000000067122170: 48 8B 84 24 C8 01  mov         rax,qword ptr [rsp+00000000000001C8h]
                    00 00
  0000000067122178: C6 04 01 00        mov         byte ptr [rcx+rax],0
  000000006712217C: 49 8B 04 24        mov         rax,qword ptr [r12]
  0000000067122180: 49 8B CC           mov         rcx,r12
  0000000067122183: FF 10              call        qword ptr [rax]
  0000000067122185: 48 8B C8           mov         rcx,rax
  0000000067122188: 48 8B 94 24 C8 01  mov         rdx,qword ptr [rsp+00000000000001C8h]
                    00 00
  0000000067122190: E8 4B F0 FF FF     call        00000000671211E0
  0000000067122195: 8B F8              mov         edi,eax
  0000000067122197: 85 C0              test        eax,eax
  0000000067122199: 79 54              jns         00000000671221EF
  000000006712219B: 48 8B 94 24 C8 01  mov         rdx,qword ptr [rsp+00000000000001C8h]
                    00 00
  00000000671221A3: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  00000000671221A8: E8 B3 DD FF FF     call        000000006711FF60
  00000000671221AD: 90                 nop
  00000000671221AE: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  00000000671221B3: 48 8B 78 10        mov         rdi,qword ptr [rax+10h]
  00000000671221B7: 49 8B 04 24        mov         rax,qword ptr [r12]
  00000000671221BB: 49 8B CC           mov         rcx,r12
  00000000671221BE: FF 10              call        qword ptr [rax]
  00000000671221C0: 48 8B C8           mov         rcx,rax
  00000000671221C3: 48 8B D7           mov         rdx,rdi
  00000000671221C6: E8 15 F0 FF FF     call        00000000671211E0
  00000000671221CB: 8B F8              mov         edi,eax
  00000000671221CD: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  00000000671221D2: F0 44 0F C1 30     lock xadd   dword ptr [rax],r14d
  00000000671221D7: 41 83 C6 FF        add         r14d,0FFFFFFFFh
  00000000671221DB: 75 0A              jne         00000000671221E7
  00000000671221DD: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  00000000671221E2: E8 C9 E1 EF FF     call        00000000670203B0
  00000000671221E7: 85 FF              test        edi,edi
  00000000671221E9: 0F 88 D3 02 00 00  js          00000000671224C2
  00000000671221EF: 49 8B 04 24        mov         rax,qword ptr [r12]
  00000000671221F3: 49 8B CC           mov         rcx,r12
  00000000671221F6: FF 10              call        qword ptr [rax]
  00000000671221F8: 48 8B 48 10        mov         rcx,qword ptr [rax+10h]
  00000000671221FC: 8B 51 10           mov         edx,dword ptr [rcx+10h]
  00000000671221FF: 48 8B 08           mov         rcx,qword ptr [rax]
  0000000067122202: 48 85 C9           test        rcx,rcx
  0000000067122205: 74 18              je          000000006712221F
  0000000067122207: 66 0F 1F 84 00 00  nop         word ptr [rax+rax+0000000000000000h]
                    00 00 00
  0000000067122210: 48 8B 41 10        mov         rax,qword ptr [rcx+10h]
  0000000067122214: 03 50 10           add         edx,dword ptr [rax+10h]
  0000000067122217: 48 8B 09           mov         rcx,qword ptr [rcx]
  000000006712221A: 48 85 C9           test        rcx,rcx
  000000006712221D: 75 F1              jne         0000000067122210
  000000006712221F: 3B FA              cmp         edi,edx
  0000000067122221: 0F 8D 9B 02 00 00  jge         00000000671224C2
  0000000067122227: 49 8B 04 24        mov         rax,qword ptr [r12]
  000000006712222B: 49 8B CC           mov         rcx,r12
  000000006712222E: FF 10              call        qword ptr [rax]
  0000000067122230: 4C 8B D8           mov         r11,rax
  0000000067122233: 33 ED              xor         ebp,ebp
  0000000067122235: 8B C5              mov         eax,ebp
  0000000067122237: 4D 8B 0B           mov         r9,qword ptr [r11]
  000000006712223A: 49 8B D1           mov         rdx,r9
  000000006712223D: 4D 85 C9           test        r9,r9
  0000000067122240: 74 1D              je          000000006712225F
  0000000067122242: 66 66 66 66 66 66  nop         word ptr [rax+rax+0000000000000000h]
                    0F 1F 84 00 00 00
                    00 00
  0000000067122250: 48 8B 4A 10        mov         rcx,qword ptr [rdx+10h]
  0000000067122254: 03 41 10           add         eax,dword ptr [rcx+10h]
  0000000067122257: 48 8B 12           mov         rdx,qword ptr [rdx]
  000000006712225A: 48 85 D2           test        rdx,rdx
  000000006712225D: 75 F1              jne         0000000067122250
  000000006712225F: 8B D7              mov         edx,edi
  0000000067122261: 2B D0              sub         edx,eax
  0000000067122263: 79 1A              jns         000000006712227F
  0000000067122265: 4D 85 C9           test        r9,r9
  0000000067122268: 74 15              je          000000006712227F
  000000006712226A: 44 8B C7           mov         r8d,edi
  000000006712226D: 48 8D 94 24 90 00  lea         rdx,[rsp+0000000000000090h]
                    00 00
  0000000067122275: 49 8B C9           mov         rcx,r9
  0000000067122278: E8 93 C1 FF FF     call        000000006711E410
  000000006712227D: EB 33              jmp         00000000671222B2
  000000006712227F: 48 89 6C 24 70     mov         qword ptr [rsp+70h],rbp
  0000000067122284: 89 6C 24 78        mov         dword ptr [rsp+78h],ebp
  0000000067122288: 85 D2              test        edx,edx
  000000006712228A: 78 18              js          00000000671222A4
  000000006712228C: 49 8B 4B 10        mov         rcx,qword ptr [r11+10h]
  0000000067122290: 3B 51 10           cmp         edx,dword ptr [rcx+10h]
  0000000067122293: 7D 0F              jge         00000000671222A4
  0000000067122295: 4C 89 5C 24 70     mov         qword ptr [rsp+70h],r11
  000000006712229A: 8D 04 92           lea         eax,[rdx+rdx*4]
  000000006712229D: 03 41 14           add         eax,dword ptr [rcx+14h]
  00000000671222A0: 89 44 24 78        mov         dword ptr [rsp+78h],eax
  00000000671222A4: 0F 28 44 24 70     movaps      xmm0,xmmword ptr [rsp+70h]
  00000000671222A9: 66 0F 7F 84 24 90  movdqa      xmmword ptr [rsp+0000000000000090h],xmm0
                    00 00 00
  00000000671222B2: 48 8D 8C 24 A0 01  lea         rcx,[rsp+00000000000001A0h]
                    00 00
  00000000671222BA: 48 8B AC 24 88 04  mov         rbp,qword ptr [rsp+0000000000000488h]
                    00 00
  00000000671222C2: 48 8B 45 00        mov         rax,qword ptr [rbp]
  00000000671222C6: 48 89 01           mov         qword ptr [rcx],rax
  00000000671222C9: 48 8B 45 08        mov         rax,qword ptr [rbp+8]
  00000000671222CD: 48 89 41 08        mov         qword ptr [rcx+8],rax
  00000000671222D1: 48 8D 8C 24 70 01  lea         rcx,[rsp+0000000000000170h]
                    00 00
  00000000671222D9: 48 8B AC 24 80 04  mov         rbp,qword ptr [rsp+0000000000000480h]
                    00 00
  00000000671222E1: 48 8B 45 00        mov         rax,qword ptr [rbp]
  00000000671222E5: 48 89 01           mov         qword ptr [rcx],rax
  00000000671222E8: 48 8B 45 08        mov         rax,qword ptr [rbp+8]
  00000000671222EC: 48 89 41 08        mov         qword ptr [rcx+8],rax
  00000000671222F0: 48 8D 8C 24 30 01  lea         rcx,[rsp+0000000000000130h]
                    00 00
  00000000671222F8: 48 8B AC 24 78 04  mov         rbp,qword ptr [rsp+0000000000000478h]
                    00 00
  0000000067122300: 48 8B 45 00        mov         rax,qword ptr [rbp]
  0000000067122304: 48 89 01           mov         qword ptr [rcx],rax
  0000000067122307: 48 8B 45 08        mov         rax,qword ptr [rbp+8]
  000000006712230B: 48 89 41 08        mov         qword ptr [rcx+8],rax
  000000006712230F: 48 8D 8C 24 00 01  lea         rcx,[rsp+0000000000000100h]
                    00 00
  0000000067122317: 48 8B AC 24 70 04  mov         rbp,qword ptr [rsp+0000000000000470h]
                    00 00
  000000006712231F: 48 8B 45 00        mov         rax,qword ptr [rbp]
  0000000067122323: 48 89 01           mov         qword ptr [rcx],rax
  0000000067122326: 48 8B 45 08        mov         rax,qword ptr [rbp+8]
  000000006712232A: 48 89 41 08        mov         qword ptr [rcx+8],rax
  000000006712232E: 48 8D 8C 24 50 01  lea         rcx,[rsp+0000000000000150h]
                    00 00
  0000000067122336: 48 8B AC 24 68 04  mov         rbp,qword ptr [rsp+0000000000000468h]
                    00 00
  000000006712233E: 48 8B 45 00        mov         rax,qword ptr [rbp]
  0000000067122342: 48 89 01           mov         qword ptr [rcx],rax
  0000000067122345: 48 8B 45 08        mov         rax,qword ptr [rbp+8]
  0000000067122349: 48 89 41 08        mov         qword ptr [rcx+8],rax
  000000006712234D: 48 8D 8C 24 90 01  lea         rcx,[rsp+0000000000000190h]
                    00 00
  0000000067122355: 48 8B AC 24 60 04  mov         rbp,qword ptr [rsp+0000000000000460h]
                    00 00
  000000006712235D: 48 8B 45 00        mov         rax,qword ptr [rbp]
  0000000067122361: 48 89 01           mov         qword ptr [rcx],rax
  0000000067122364: 48 8B 45 08        mov         rax,qword ptr [rbp+8]
  0000000067122368: 48 89 41 08        mov         qword ptr [rcx+8],rax
  000000006712236C: 48 8D 8C 24 10 01  lea         rcx,[rsp+0000000000000110h]
                    00 00
  0000000067122374: 48 8B AC 24 58 04  mov         rbp,qword ptr [rsp+0000000000000458h]
                    00 00
  000000006712237C: 48 8B 45 00        mov         rax,qword ptr [rbp]
  0000000067122380: 48 89 01           mov         qword ptr [rcx],rax
  0000000067122383: 48 8B 45 08        mov         rax,qword ptr [rbp+8]
  0000000067122387: 48 89 41 08        mov         qword ptr [rcx+8],rax
  000000006712238B: 48 8D 8C 24 20 01  lea         rcx,[rsp+0000000000000120h]
                    00 00
  0000000067122393: 48 8B AC 24 50 04  mov         rbp,qword ptr [rsp+0000000000000450h]
                    00 00
  000000006712239B: 48 8B 45 00        mov         rax,qword ptr [rbp]
  000000006712239F: 48 89 01           mov         qword ptr [rcx],rax
  00000000671223A2: 48 8B 45 08        mov         rax,qword ptr [rbp+8]
  00000000671223A6: 48 89 41 08        mov         qword ptr [rcx+8],rax
  00000000671223AA: 48 8D 8C 24 40 01  lea         rcx,[rsp+0000000000000140h]
                    00 00
  00000000671223B2: 48 8B AC 24 48 04  mov         rbp,qword ptr [rsp+0000000000000448h]
                    00 00
  00000000671223BA: 48 8B 45 00        mov         rax,qword ptr [rbp]
  00000000671223BE: 48 89 01           mov         qword ptr [rcx],rax
  00000000671223C1: 48 8B 45 08        mov         rax,qword ptr [rbp+8]
  00000000671223C5: 48 89 41 08        mov         qword ptr [rcx+8],rax
  00000000671223C9: 48 8D 8C 24 60 01  lea         rcx,[rsp+0000000000000160h]
                    00 00
  00000000671223D1: 48 8B AC 24 40 04  mov         rbp,qword ptr [rsp+0000000000000440h]
                    00 00
  00000000671223D9: 48 8B 45 00        mov         rax,qword ptr [rbp]
  00000000671223DD: 48 89 01           mov         qword ptr [rcx],rax
  00000000671223E0: 48 8B 45 08        mov         rax,qword ptr [rbp+8]
  00000000671223E4: 48 89 41 08        mov         qword ptr [rcx+8],rax
  00000000671223E8: 48 8D 8C 24 80 01  lea         rcx,[rsp+0000000000000180h]
                    00 00
  00000000671223F0: 49 8B 07           mov         rax,qword ptr [r15]
  00000000671223F3: 48 89 01           mov         qword ptr [rcx],rax
  00000000671223F6: 49 8B 47 08        mov         rax,qword ptr [r15+8]
  00000000671223FA: 48 89 41 08        mov         qword ptr [rcx+8],rax
  00000000671223FE: 48 8D 84 24 A0 01  lea         rax,[rsp+00000000000001A0h]
                    00 00
  0000000067122406: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  000000006712240B: 48 8D 84 24 70 01  lea         rax,[rsp+0000000000000170h]
                    00 00
  0000000067122413: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000067122418: 48 8D 84 24 30 01  lea         rax,[rsp+0000000000000130h]
                    00 00
  0000000067122420: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000067122425: 48 8D 84 24 00 01  lea         rax,[rsp+0000000000000100h]
                    00 00
  000000006712242D: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000067122432: 48 8D 84 24 50 01  lea         rax,[rsp+0000000000000150h]
                    00 00
  000000006712243A: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  000000006712243F: 48 8D 84 24 90 01  lea         rax,[rsp+0000000000000190h]
                    00 00
  0000000067122447: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  000000006712244C: 48 8D 84 24 10 01  lea         rax,[rsp+0000000000000110h]
                    00 00
  0000000067122454: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000067122459: 48 8D 84 24 20 01  lea         rax,[rsp+0000000000000120h]
                    00 00
  0000000067122461: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000067122466: 48 8D 84 24 40 01  lea         rax,[rsp+0000000000000140h]
                    00 00
  000000006712246E: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000067122473: 48 8D 84 24 60 01  lea         rax,[rsp+0000000000000160h]
                    00 00
  000000006712247B: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000067122480: 4C 8D 8C 24 80 01  lea         r9,[rsp+0000000000000180h]
                    00 00
  0000000067122488: 44 8B 84 24 30 04  mov         r8d,dword ptr [rsp+0000000000000430h]
                    00 00
  0000000067122490: 49 8B D4           mov         rdx,r12
  0000000067122493: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  000000006712249B: E8 30 DD FF FF     call        00000000671201D0
  00000000671224A0: 0F B6 F8           movzx       edi,al
  00000000671224A3: 48 8D 84 24 D0 01  lea         rax,[rsp+00000000000001D0h]
                    00 00
  00000000671224AB: 48 8B 8C 24 C8 01  mov         rcx,qword ptr [rsp+00000000000001C8h]
                    00 00
  00000000671224B3: 48 3B C8           cmp         rcx,rax
  00000000671224B6: 74 05              je          00000000671224BD
  00000000671224B8: E8 F3 DE EF FF     call        00000000670203B0
  00000000671224BD: 40 8A C7           mov         al,dil
  00000000671224C0: EB 45              jmp         0000000067122507
  00000000671224C2: 48 8B BC 24 C8 01  mov         rdi,qword ptr [rsp+00000000000001C8h]
                    00 00
  00000000671224CA: 49 8B 04 24        mov         rax,qword ptr [r12]
  00000000671224CE: 49 8B CC           mov         rcx,r12
  00000000671224D1: FF 10              call        qword ptr [rax]
  00000000671224D3: 4C 8B C7           mov         r8,rdi
  00000000671224D6: 48 8B 50 08        mov         rdx,qword ptr [rax+8]
  00000000671224DA: 48 8D 0D 1F 84 11  lea         rcx,[000000006723A900h]
                    00
  00000000671224E1: E8 FA A9 EF FF     call        000000006701CEE0
  00000000671224E6: 90                 nop
  00000000671224E7: 48 8D 84 24 D0 01  lea         rax,[rsp+00000000000001D0h]
                    00 00
  00000000671224EF: 48 8B 8C 24 C8 01  mov         rcx,qword ptr [rsp+00000000000001C8h]
                    00 00
  00000000671224F7: 48 3B C8           cmp         rcx,rax
  00000000671224FA: 74 05              je          0000000067122501
  00000000671224FC: E8 AF DE EF FF     call        00000000670203B0
  0000000067122501: 32 C0              xor         al,al
  0000000067122503: EB 02              jmp         0000000067122507
  0000000067122505: 32 C0              xor         al,al
  0000000067122507: 48 8B 9C 24 28 04  mov         rbx,qword ptr [rsp+0000000000000428h]
                    00 00
  000000006712250F: 48 81 C4 E0 03 00  add         rsp,3E0h
                    00
  0000000067122516: 41 5F              pop         r15
  0000000067122518: 41 5E              pop         r14
  000000006712251A: 41 5D              pop         r13
  000000006712251C: 41 5C              pop         r12
  000000006712251E: 5F                 pop         rdi
  000000006712251F: 5E                 pop         rsi
  0000000067122520: 5D                 pop         rbp
  0000000067122521: C3                 ret

  Summary

      1A7000 .text
