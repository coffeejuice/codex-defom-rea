Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001411E2DD0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001411E2DD5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001411E2DDA: 48 81 EC 48 02 00  sub         rsp,248h
                    00
  00000001411E2DE1: 48 C7 84 24 60 01  mov         qword ptr [rsp+0000000000000160h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  00000001411E2DED: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  00000001411E2DF2: 48 8B 8C 24 58 02  mov         rcx,qword ptr [rsp+0000000000000258h]
                    00 00
  00000001411E2DFA: FF 15 80 15 C2 00  call        qword ptr [0000000141E04380h]
  00000001411E2E00: 48 89 84 24 68 01  mov         qword ptr [rsp+0000000000000168h],rax
                    00 00
  00000001411E2E08: 48 8B 84 24 68 01  mov         rax,qword ptr [rsp+0000000000000168h]
                    00 00
  00000001411E2E10: 48 89 84 24 70 01  mov         qword ptr [rsp+0000000000000170h],rax
                    00 00
  00000001411E2E18: 48 8B 8C 24 70 01  mov         rcx,qword ptr [rsp+0000000000000170h]
                    00 00
  00000001411E2E20: FF 15 A2 11 C2 00  call        qword ptr [0000000141E03FC8h]
  00000001411E2E26: 88 44 24 30        mov         byte ptr [rsp+30h],al
  00000001411E2E2A: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  00000001411E2E2F: FF 15 AB 2B C2 00  call        qword ptr [0000000141E059E0h]
  00000001411E2E35: 44 0F B6 5C 24 30  movzx       r11d,byte ptr [rsp+30h]
  00000001411E2E3B: 45 85 DB           test        r11d,r11d
  00000001411E2E3E: 74 52              je          00000001411E2E92
  00000001411E2E40: 41 B8 FF FF FF FF  mov         r8d,0FFFFFFFFh
  00000001411E2E46: 48 8D 15 1B 6F AF  lea         rdx,[0000000142CD9D68h]
                    01
  00000001411E2E4D: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000001411E2E52: FF 15 F8 12 C2 00  call        qword ptr [0000000141E04150h]
  00000001411E2E58: 48 89 84 24 78 01  mov         qword ptr [rsp+0000000000000178h],rax
                    00 00
  00000001411E2E60: 48 8B 84 24 78 01  mov         rax,qword ptr [rsp+0000000000000178h]
                    00 00
  00000001411E2E68: 48 89 84 24 80 01  mov         qword ptr [rsp+0000000000000180h],rax
                    00 00
  00000001411E2E70: 48 8B 94 24 80 01  mov         rdx,qword ptr [rsp+0000000000000180h]
                    00 00
  00000001411E2E78: 48 8B 8C 24 58 02  mov         rcx,qword ptr [rsp+0000000000000258h]
                    00 00
  00000001411E2E80: FF 15 D2 12 C2 00  call        qword ptr [0000000141E04158h]
  00000001411E2E86: 90                 nop
  00000001411E2E87: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000001411E2E8C: FF 15 4E 2B C2 00  call        qword ptr [0000000141E059E0h]
  00000001411E2E92: 41 B8 22 01 00 00  mov         r8d,122h
  00000001411E2E98: BA 49 01 00 00     mov         edx,149h
  00000001411E2E9D: 48 8B 8C 24 58 02  mov         rcx,qword ptr [rsp+0000000000000258h]
                    00 00
  00000001411E2EA5: FF 15 9D 43 C2 00  call        qword ptr [0000000141E07248h]
  00000001411E2EAB: B9 20 00 00 00     mov         ecx,20h
  00000001411E2EB0: E8 07 BC 28 00     call        000000014146EABC
  00000001411E2EB5: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000001411E2EBA: 48 83 7C 24 50 00  cmp         qword ptr [rsp+50h],0
  00000001411E2EC0: 74 41              je          00000001411E2F03
  00000001411E2EC2: 48 8B 94 24 58 02  mov         rdx,qword ptr [rsp+0000000000000258h]
                    00 00
  00000001411E2ECA: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001411E2ECF: FF 15 93 33 C2 00  call        qword ptr [0000000141E06268h]
  00000001411E2ED5: 4C 8B 5C 24 50     mov         r11,qword ptr [rsp+50h]
  00000001411E2EDA: 48 8D 05 D7 E6 F1  lea         rax,[00000001421015B8h]
                    00
  00000001411E2EE1: 49 89 03           mov         qword ptr [r11],rax
  00000001411E2EE4: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001411E2EE9: 48 8D 05 48 E6 F1  lea         rax,[0000000142101538h]
                    00
  00000001411E2EF0: 48 89 41 10        mov         qword ptr [rcx+10h],rax
  00000001411E2EF4: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000001411E2EF9: 48 89 84 24 88 01  mov         qword ptr [rsp+0000000000000188h],rax
                    00 00
  00000001411E2F01: EB 0C              jmp         00000001411E2F0F
  00000001411E2F03: 48 C7 84 24 88 01  mov         qword ptr [rsp+0000000000000188h],0
                    00 00 00 00 00 00
  00000001411E2F0F: 48 8B 84 24 88 01  mov         rax,qword ptr [rsp+0000000000000188h]
                    00 00
  00000001411E2F17: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001411E2F1C: 48 8B 8C 24 50 02  mov         rcx,qword ptr [rsp+0000000000000250h]
                    00 00
  00000001411E2F24: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  00000001411E2F29: 48 89 01           mov         qword ptr [rcx],rax
  00000001411E2F2C: BA 0A 00 00 00     mov         edx,0Ah
  00000001411E2F31: 48 8B 8C 24 50 02  mov         rcx,qword ptr [rsp+0000000000000250h]
                    00 00
  00000001411E2F39: 48 8B 09           mov         rcx,qword ptr [rcx]
  00000001411E2F3C: FF 15 1E 43 C2 00  call        qword ptr [0000000141E07260h]
  00000001411E2F42: C7 44 24 20 0B 00  mov         dword ptr [rsp+20h],0Bh
                    00 00
  00000001411E2F4A: 41 B9 0B 00 00 00  mov         r9d,0Bh
  00000001411E2F50: 41 B8 0B 00 00 00  mov         r8d,0Bh
  00000001411E2F56: BA 0B 00 00 00     mov         edx,0Bh
  00000001411E2F5B: 48 8B 8C 24 50 02  mov         rcx,qword ptr [rsp+0000000000000250h]
                    00 00
  00000001411E2F63: 48 8B 09           mov         rcx,qword ptr [rcx]
  00000001411E2F66: FF 15 3C 84 C2 00  call        qword ptr [0000000141E0B3A8h]
  00000001411E2F6C: 41 B8 FF FF FF FF  mov         r8d,0FFFFFFFFh
  00000001411E2F72: 48 8D 15 8F F4 FA  lea         rdx,[0000000142192408h]
                    00
  00000001411E2F79: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  00000001411E2F7E: FF 15 CC 11 C2 00  call        qword ptr [0000000141E04150h]
  00000001411E2F84: 48 89 84 24 90 01  mov         qword ptr [rsp+0000000000000190h],rax
                    00 00
  00000001411E2F8C: 48 8B 84 24 90 01  mov         rax,qword ptr [rsp+0000000000000190h]
                    00 00
  00000001411E2F94: 48 89 84 24 98 01  mov         qword ptr [rsp+0000000000000198h],rax
                    00 00
  00000001411E2F9C: 48 8B 94 24 98 01  mov         rdx,qword ptr [rsp+0000000000000198h]
                    00 00
  00000001411E2FA4: 48 8B 8C 24 50 02  mov         rcx,qword ptr [rsp+0000000000000250h]
                    00 00
  00000001411E2FAC: 48 8B 09           mov         rcx,qword ptr [rcx]
  00000001411E2FAF: FF 15 A3 11 C2 00  call        qword ptr [0000000141E04158h]
  00000001411E2FB5: 90                 nop
  00000001411E2FB6: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  00000001411E2FBB: FF 15 1F 2A C2 00  call        qword ptr [0000000141E059E0h]
  00000001411E2FC1: B9 20 00 00 00     mov         ecx,20h
  00000001411E2FC6: E8 F1 BA 28 00     call        000000014146EABC
  00000001411E2FCB: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  00000001411E2FD0: 48 83 7C 24 68 00  cmp         qword ptr [rsp+68h],0
  00000001411E2FD6: 74 39              je          00000001411E3011
  00000001411E2FD8: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  00000001411E2FDD: FF 15 75 42 C2 00  call        qword ptr [0000000141E07258h]
  00000001411E2FE3: 4C 8B 5C 24 68     mov         r11,qword ptr [rsp+68h]
  00000001411E2FE8: 48 8D 05 B9 0B F9  lea         rax,[0000000142173BA8h]
                    00
  00000001411E2FEF: 49 89 03           mov         qword ptr [r11],rax
  00000001411E2FF2: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  00000001411E2FF7: 48 8D 05 2A 0B F9  lea         rax,[0000000142173B28h]
                    00
  00000001411E2FFE: 48 89 41 10        mov         qword ptr [rcx+10h],rax
  00000001411E3002: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  00000001411E3007: 48 89 84 24 A0 01  mov         qword ptr [rsp+00000000000001A0h],rax
                    00 00
  00000001411E300F: EB 0C              jmp         00000001411E301D
  00000001411E3011: 48 C7 84 24 A0 01  mov         qword ptr [rsp+00000000000001A0h],0
                    00 00 00 00 00 00
  00000001411E301D: 48 8B 84 24 A0 01  mov         rax,qword ptr [rsp+00000000000001A0h]
                    00 00
  00000001411E3025: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  00000001411E302A: 48 8B 8C 24 50 02  mov         rcx,qword ptr [rsp+0000000000000250h]
                    00 00
  00000001411E3032: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  00000001411E3037: 48 89 41 08        mov         qword ptr [rcx+8],rax
  00000001411E303B: BA 06 00 00 00     mov         edx,6
  00000001411E3040: 48 8B 8C 24 50 02  mov         rcx,qword ptr [rsp+0000000000000250h]
                    00 00
  00000001411E3048: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  00000001411E304C: FF 15 0E 42 C2 00  call        qword ptr [0000000141E07260h]
  00000001411E3052: 41 B8 FF FF FF FF  mov         r8d,0FFFFFFFFh
  00000001411E3058: 48 8D 15 A1 0A F9  lea         rdx,[0000000142173B00h]
                    00
  00000001411E305F: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  00000001411E3064: FF 15 E6 10 C2 00  call        qword ptr [0000000141E04150h]
  00000001411E306A: 48 89 84 24 A8 01  mov         qword ptr [rsp+00000000000001A8h],rax
                    00 00
  00000001411E3072: 48 8B 84 24 A8 01  mov         rax,qword ptr [rsp+00000000000001A8h]
                    00 00
  00000001411E307A: 48 89 84 24 B0 01  mov         qword ptr [rsp+00000000000001B0h],rax
                    00 00
  00000001411E3082: 48 8B 94 24 B0 01  mov         rdx,qword ptr [rsp+00000000000001B0h]
                    00 00
  00000001411E308A: 48 8B 8C 24 50 02  mov         rcx,qword ptr [rsp+0000000000000250h]
                    00 00
  00000001411E3092: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  00000001411E3096: FF 15 BC 10 C2 00  call        qword ptr [0000000141E04158h]
  00000001411E309C: 90                 nop
  00000001411E309D: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  00000001411E30A2: FF 15 38 29 C2 00  call        qword ptr [0000000141E059E0h]
  00000001411E30A8: B9 28 00 00 00     mov         ecx,28h
  00000001411E30AD: E8 0A BA 28 00     call        000000014146EABC
  00000001411E30B2: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  00000001411E30BA: 48 83 BC 24 80 00  cmp         qword ptr [rsp+0000000000000080h],0
                    00 00 00
  00000001411E30C3: 74 6F              je          00000001411E3134
  00000001411E30C5: 48 8D 84 24 88 00  lea         rax,[rsp+0000000000000088h]
                    00 00
  00000001411E30CD: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  00000001411E30D5: 33 D2              xor         edx,edx
  00000001411E30D7: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001411E30DF: E8 8C 3B 36 FF     call        0000000140546C70
  00000001411E30E4: 4C 8B C0           mov         r8,rax
  00000001411E30E7: 48 8B 94 24 58 02  mov         rdx,qword ptr [rsp+0000000000000258h]
                    00 00
  00000001411E30EF: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001411E30F7: FF 15 7B 3C C2 00  call        qword ptr [0000000141E06D78h]
  00000001411E30FD: 4C 8B 9C 24 80 00  mov         r11,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001411E3105: 48 8D 05 8C 98 FA  lea         rax,[000000014218C998h]
                    00
  00000001411E310C: 49 89 03           mov         qword ptr [r11],rax
  00000001411E310F: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001411E3117: 48 8D 05 3A 98 FA  lea         rax,[000000014218C958h]
                    00
  00000001411E311E: 48 89 41 10        mov         qword ptr [rcx+10h],rax
  00000001411E3122: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001411E312A: 48 89 84 24 B8 01  mov         qword ptr [rsp+00000000000001B8h],rax
                    00 00
  00000001411E3132: EB 0C              jmp         00000001411E3140
  00000001411E3134: 48 C7 84 24 B8 01  mov         qword ptr [rsp+00000000000001B8h],0
                    00 00 00 00 00 00
  00000001411E3140: 48 8B 84 24 B8 01  mov         rax,qword ptr [rsp+00000000000001B8h]
                    00 00
  00000001411E3148: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  00000001411E314D: 48 8B 8C 24 50 02  mov         rcx,qword ptr [rsp+0000000000000250h]
                    00 00
  00000001411E3155: 48 8B 44 24 78     mov         rax,qword ptr [rsp+78h]
  00000001411E315A: 48 89 41 10        mov         qword ptr [rcx+10h],rax
  00000001411E315E: 41 B8 FF FF FF FF  mov         r8d,0FFFFFFFFh
  00000001411E3164: 48 8D 15 DD 97 FA  lea         rdx,[000000014218C948h]
                    00
  00000001411E316B: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  00000001411E3173: FF 15 D7 0F C2 00  call        qword ptr [0000000141E04150h]
  00000001411E3179: 48 89 84 24 C0 01  mov         qword ptr [rsp+00000000000001C0h],rax
                    00 00
  00000001411E3181: 48 8B 84 24 C0 01  mov         rax,qword ptr [rsp+00000000000001C0h]
                    00 00
  00000001411E3189: 48 89 84 24 C8 01  mov         qword ptr [rsp+00000000000001C8h],rax
                    00 00
  00000001411E3191: 48 8B 94 24 C8 01  mov         rdx,qword ptr [rsp+00000000000001C8h]
                    00 00
  00000001411E3199: 48 8B 8C 24 50 02  mov         rcx,qword ptr [rsp+0000000000000250h]
                    00 00
  00000001411E31A1: 48 8B 49 10        mov         rcx,qword ptr [rcx+10h]
  00000001411E31A5: FF 15 AD 0F C2 00  call        qword ptr [0000000141E04158h]
  00000001411E31AB: 90                 nop
  00000001411E31AC: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  00000001411E31B4: FF 15 26 28 C2 00  call        qword ptr [0000000141E059E0h]
  00000001411E31BA: 48 8D 84 24 A0 00  lea         rax,[rsp+00000000000000A0h]
                    00 00
  00000001411E31C2: 48 89 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],rax
                    00 00
  00000001411E31CA: 33 D2              xor         edx,edx
  00000001411E31CC: 48 8B 8C 24 A8 00  mov         rcx,qword ptr [rsp+00000000000000A8h]
                    00 00
  00000001411E31D4: E8 97 3A 36 FF     call        0000000140546C70
  00000001411E31D9: 4C 8B C8           mov         r9,rax
  00000001411E31DC: 45 33 C0           xor         r8d,r8d
  00000001411E31DF: 48 8B 94 24 50 02  mov         rdx,qword ptr [rsp+0000000000000250h]
                    00 00
  00000001411E31E7: 48 8B 52 10        mov         rdx,qword ptr [rdx+10h]
  00000001411E31EB: 48 8B 8C 24 50 02  mov         rcx,qword ptr [rsp+0000000000000250h]
                    00 00
  00000001411E31F3: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  00000001411E31F7: FF 15 AB 40 C2 00  call        qword ptr [0000000141E072A8h]
  00000001411E31FD: B9 28 00 00 00     mov         ecx,28h
  00000001411E3202: E8 B5 B8 28 00     call        000000014146EABC
  00000001411E3207: 48 89 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],rax
                    00 00
  00000001411E320F: 48 83 BC 24 B8 00  cmp         qword ptr [rsp+00000000000000B8h],0
                    00 00 00
  00000001411E3218: 74 4D              je          00000001411E3267
  00000001411E321A: 48 8B 94 24 58 02  mov         rdx,qword ptr [rsp+0000000000000258h]
                    00 00
  00000001411E3222: 48 8B 8C 24 B8 00  mov         rcx,qword ptr [rsp+00000000000000B8h]
                    00 00
  00000001411E322A: FF 15 50 3B C2 00  call        qword ptr [0000000141E06D80h]
  00000001411E3230: 4C 8B 9C 24 B8 00  mov         r11,qword ptr [rsp+00000000000000B8h]
                    00 00
  00000001411E3238: 48 8D 05 69 95 FA  lea         rax,[000000014218C7A8h]
                    00
  00000001411E323F: 49 89 03           mov         qword ptr [r11],rax
  00000001411E3242: 48 8B 8C 24 B8 00  mov         rcx,qword ptr [rsp+00000000000000B8h]
                    00 00
  00000001411E324A: 48 8D 05 1F 95 FA  lea         rax,[000000014218C770h]
                    00
  00000001411E3251: 48 89 41 10        mov         qword ptr [rcx+10h],rax
  00000001411E3255: 48 8B 84 24 B8 00  mov         rax,qword ptr [rsp+00000000000000B8h]
                    00 00
  00000001411E325D: 48 89 84 24 D0 01  mov         qword ptr [rsp+00000000000001D0h],rax
                    00 00
  00000001411E3265: EB 0C              jmp         00000001411E3273
  00000001411E3267: 48 C7 84 24 D0 01  mov         qword ptr [rsp+00000000000001D0h],0
                    00 00 00 00 00 00
  00000001411E3273: 48 8B 84 24 D0 01  mov         rax,qword ptr [rsp+00000000000001D0h]
                    00 00
  00000001411E327B: 48 89 84 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],rax
                    00 00
  00000001411E3283: 48 8B 8C 24 50 02  mov         rcx,qword ptr [rsp+0000000000000250h]
                    00 00
  00000001411E328B: 48 8B 84 24 B0 00  mov         rax,qword ptr [rsp+00000000000000B0h]
                    00 00
  00000001411E3293: 48 89 41 18        mov         qword ptr [rcx+18h],rax
  00000001411E3297: 41 B8 FF FF FF FF  mov         r8d,0FFFFFFFFh
  00000001411E329D: 48 8D 15 B4 6A AF  lea         rdx,[0000000142CD9D58h]
                    01
  00000001411E32A4: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  00000001411E32AC: FF 15 9E 0E C2 00  call        qword ptr [0000000141E04150h]
  00000001411E32B2: 48 89 84 24 D8 01  mov         qword ptr [rsp+00000000000001D8h],rax
                    00 00
  00000001411E32BA: 48 8B 84 24 D8 01  mov         rax,qword ptr [rsp+00000000000001D8h]
                    00 00
  00000001411E32C2: 48 89 84 24 E0 01  mov         qword ptr [rsp+00000000000001E0h],rax
                    00 00
  00000001411E32CA: 48 8B 94 24 E0 01  mov         rdx,qword ptr [rsp+00000000000001E0h]
                    00 00
  00000001411E32D2: 48 8B 8C 24 50 02  mov         rcx,qword ptr [rsp+0000000000000250h]
                    00 00
  00000001411E32DA: 48 8B 49 18        mov         rcx,qword ptr [rcx+18h]
  00000001411E32DE: FF 15 74 0E C2 00  call        qword ptr [0000000141E04158h]
  00000001411E32E4: 90                 nop
  00000001411E32E5: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  00000001411E32ED: FF 15 ED 26 C2 00  call        qword ptr [0000000141E059E0h]
  00000001411E32F3: 48 8D 84 24 C8 00  lea         rax,[rsp+00000000000000C8h]
                    00 00
  00000001411E32FB: 48 89 84 24 D0 00  mov         qword ptr [rsp+00000000000000D0h],rax
                    00 00
  00000001411E3303: 33 D2              xor         edx,edx
  00000001411E3305: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  00000001411E330D: E8 5E 39 36 FF     call        0000000140546C70
  00000001411E3312: 4C 8B C8           mov         r9,rax
  00000001411E3315: 45 33 C0           xor         r8d,r8d
  00000001411E3318: 48 8B 94 24 50 02  mov         rdx,qword ptr [rsp+0000000000000250h]
                    00 00
  00000001411E3320: 48 8B 52 18        mov         rdx,qword ptr [rdx+18h]
  00000001411E3324: 48 8B 8C 24 50 02  mov         rcx,qword ptr [rsp+0000000000000250h]
                    00 00
  00000001411E332C: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  00000001411E3330: FF 15 72 3F C2 00  call        qword ptr [0000000141E072A8h]
  00000001411E3336: 45 33 C0           xor         r8d,r8d
  00000001411E3339: 48 8B 94 24 50 02  mov         rdx,qword ptr [rsp+0000000000000250h]
                    00 00
  00000001411E3341: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  00000001411E3345: 48 8B 8C 24 50 02  mov         rcx,qword ptr [rsp+0000000000000250h]
                    00 00
  00000001411E334D: 48 8B 09           mov         rcx,qword ptr [rcx]
  00000001411E3350: FF 15 A2 2D C2 00  call        qword ptr [0000000141E060F8h]
  00000001411E3356: B9 20 00 00 00     mov         ecx,20h
  00000001411E335B: E8 5C B7 28 00     call        000000014146EABC
  00000001411E3360: 48 89 84 24 E0 00  mov         qword ptr [rsp+00000000000000E0h],rax
                    00 00
  00000001411E3368: 48 83 BC 24 E0 00  cmp         qword ptr [rsp+00000000000000E0h],0
                    00 00 00
  00000001411E3371: 74 45              je          00000001411E33B8
  00000001411E3373: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001411E337B: FF 15 D7 3E C2 00  call        qword ptr [0000000141E07258h]
  00000001411E3381: 4C 8B 9C 24 E0 00  mov         r11,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001411E3389: 48 8D 05 18 08 F9  lea         rax,[0000000142173BA8h]
                    00
  00000001411E3390: 49 89 03           mov         qword ptr [r11],rax
  00000001411E3393: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001411E339B: 48 8D 05 86 07 F9  lea         rax,[0000000142173B28h]
                    00
  00000001411E33A2: 48 89 41 10        mov         qword ptr [rcx+10h],rax
  00000001411E33A6: 48 8B 84 24 E0 00  mov         rax,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001411E33AE: 48 89 84 24 E8 01  mov         qword ptr [rsp+00000000000001E8h],rax
                    00 00
  00000001411E33B6: EB 0C              jmp         00000001411E33C4
  00000001411E33B8: 48 C7 84 24 E8 01  mov         qword ptr [rsp+00000000000001E8h],0
                    00 00 00 00 00 00
  00000001411E33C4: 48 8B 84 24 E8 01  mov         rax,qword ptr [rsp+00000000000001E8h]
                    00 00
  00000001411E33CC: 48 89 84 24 D8 00  mov         qword ptr [rsp+00000000000000D8h],rax
                    00 00
  00000001411E33D4: 48 8B 8C 24 50 02  mov         rcx,qword ptr [rsp+0000000000000250h]
                    00 00
  00000001411E33DC: 48 8B 84 24 D8 00  mov         rax,qword ptr [rsp+00000000000000D8h]
                    00 00
  00000001411E33E4: 48 89 41 20        mov         qword ptr [rcx+20h],rax
  00000001411E33E8: BA 06 00 00 00     mov         edx,6
  00000001411E33ED: 48 8B 8C 24 50 02  mov         rcx,qword ptr [rsp+0000000000000250h]
                    00 00
  00000001411E33F5: 48 8B 49 20        mov         rcx,qword ptr [rcx+20h]
  00000001411E33F9: FF 15 61 3E C2 00  call        qword ptr [0000000141E07260h]
  00000001411E33FF: 41 B8 FF FF FF FF  mov         r8d,0FFFFFFFFh
  00000001411E3405: 48 8D 15 4C 91 FA  lea         rdx,[000000014218C558h]
                    00
  00000001411E340C: 48 8D 8C 24 E8 00  lea         rcx,[rsp+00000000000000E8h]
                    00 00
  00000001411E3414: FF 15 36 0D C2 00  call        qword ptr [0000000141E04150h]
  00000001411E341A: 48 89 84 24 F0 01  mov         qword ptr [rsp+00000000000001F0h],rax
                    00 00
  00000001411E3422: 48 8B 84 24 F0 01  mov         rax,qword ptr [rsp+00000000000001F0h]
                    00 00
  00000001411E342A: 48 89 84 24 F8 01  mov         qword ptr [rsp+00000000000001F8h],rax
                    00 00
  00000001411E3432: 48 8B 94 24 F8 01  mov         rdx,qword ptr [rsp+00000000000001F8h]
                    00 00
  00000001411E343A: 48 8B 8C 24 50 02  mov         rcx,qword ptr [rsp+0000000000000250h]
                    00 00
  00000001411E3442: 48 8B 49 20        mov         rcx,qword ptr [rcx+20h]
  00000001411E3446: FF 15 0C 0D C2 00  call        qword ptr [0000000141E04158h]
  00000001411E344C: 90                 nop
  00000001411E344D: 48 8D 8C 24 E8 00  lea         rcx,[rsp+00000000000000E8h]
                    00 00
  00000001411E3455: FF 15 85 25 C2 00  call        qword ptr [0000000141E059E0h]
  00000001411E345B: B9 28 00 00 00     mov         ecx,28h
  00000001411E3460: E8 57 B6 28 00     call        000000014146EABC
  00000001411E3465: 48 89 84 24 F8 00  mov         qword ptr [rsp+00000000000000F8h],rax
                    00 00
  00000001411E346D: 48 83 BC 24 F8 00  cmp         qword ptr [rsp+00000000000000F8h],0
                    00 00 00
  00000001411E3476: 74 6F              je          00000001411E34E7
  00000001411E3478: 48 8D 84 24 00 01  lea         rax,[rsp+0000000000000100h]
                    00 00
  00000001411E3480: 48 89 84 24 08 01  mov         qword ptr [rsp+0000000000000108h],rax
                    00 00
  00000001411E3488: 33 D2              xor         edx,edx
  00000001411E348A: 48 8B 8C 24 08 01  mov         rcx,qword ptr [rsp+0000000000000108h]
                    00 00
  00000001411E3492: E8 D9 37 36 FF     call        0000000140546C70
  00000001411E3497: 4C 8B C0           mov         r8,rax
  00000001411E349A: 48 8B 94 24 58 02  mov         rdx,qword ptr [rsp+0000000000000258h]
                    00 00
  00000001411E34A2: 48 8B 8C 24 F8 00  mov         rcx,qword ptr [rsp+00000000000000F8h]
                    00 00
  00000001411E34AA: FF 15 C8 38 C2 00  call        qword ptr [0000000141E06D78h]
  00000001411E34B0: 4C 8B 9C 24 F8 00  mov         r11,qword ptr [rsp+00000000000000F8h]
                    00 00
  00000001411E34B8: 48 8D 05 D9 94 FA  lea         rax,[000000014218C998h]
                    00
  00000001411E34BF: 49 89 03           mov         qword ptr [r11],rax
  00000001411E34C2: 48 8B 8C 24 F8 00  mov         rcx,qword ptr [rsp+00000000000000F8h]
                    00 00
  00000001411E34CA: 48 8D 05 87 94 FA  lea         rax,[000000014218C958h]
                    00
  00000001411E34D1: 48 89 41 10        mov         qword ptr [rcx+10h],rax
  00000001411E34D5: 48 8B 84 24 F8 00  mov         rax,qword ptr [rsp+00000000000000F8h]
                    00 00
  00000001411E34DD: 48 89 84 24 00 02  mov         qword ptr [rsp+0000000000000200h],rax
                    00 00
  00000001411E34E5: EB 0C              jmp         00000001411E34F3
  00000001411E34E7: 48 C7 84 24 00 02  mov         qword ptr [rsp+0000000000000200h],0
                    00 00 00 00 00 00
  00000001411E34F3: 48 8B 84 24 00 02  mov         rax,qword ptr [rsp+0000000000000200h]
                    00 00
  00000001411E34FB: 48 89 84 24 F0 00  mov         qword ptr [rsp+00000000000000F0h],rax
                    00 00
  00000001411E3503: 48 8B 8C 24 50 02  mov         rcx,qword ptr [rsp+0000000000000250h]
                    00 00
  00000001411E350B: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  00000001411E3513: 48 89 41 28        mov         qword ptr [rcx+28h],rax
  00000001411E3517: 41 B8 FF FF FF FF  mov         r8d,0FFFFFFFFh
  00000001411E351D: 48 8D 15 34 ED FA  lea         rdx,[0000000142192258h]
                    00
  00000001411E3524: 48 8D 8C 24 10 01  lea         rcx,[rsp+0000000000000110h]
                    00 00
  00000001411E352C: FF 15 1E 0C C2 00  call        qword ptr [0000000141E04150h]
  00000001411E3532: 48 89 84 24 08 02  mov         qword ptr [rsp+0000000000000208h],rax
                    00 00
  00000001411E353A: 48 8B 84 24 08 02  mov         rax,qword ptr [rsp+0000000000000208h]
                    00 00
  00000001411E3542: 48 89 84 24 10 02  mov         qword ptr [rsp+0000000000000210h],rax
                    00 00
  00000001411E354A: 48 8B 94 24 10 02  mov         rdx,qword ptr [rsp+0000000000000210h]
                    00 00
  00000001411E3552: 48 8B 8C 24 50 02  mov         rcx,qword ptr [rsp+0000000000000250h]
                    00 00
  00000001411E355A: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  00000001411E355E: FF 15 F4 0B C2 00  call        qword ptr [0000000141E04158h]
  00000001411E3564: 90                 nop
  00000001411E3565: 48 8D 8C 24 10 01  lea         rcx,[rsp+0000000000000110h]
                    00 00
  00000001411E356D: FF 15 6D 24 C2 00  call        qword ptr [0000000141E059E0h]
  00000001411E3573: 48 8D 84 24 18 01  lea         rax,[rsp+0000000000000118h]
                    00 00
  00000001411E357B: 48 89 84 24 20 01  mov         qword ptr [rsp+0000000000000120h],rax
                    00 00
  00000001411E3583: 33 D2              xor         edx,edx
  00000001411E3585: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  00000001411E358D: E8 DE 36 36 FF     call        0000000140546C70
  00000001411E3592: 4C 8B C8           mov         r9,rax
  00000001411E3595: 45 33 C0           xor         r8d,r8d
  00000001411E3598: 48 8B 94 24 50 02  mov         rdx,qword ptr [rsp+0000000000000250h]
                    00 00
  00000001411E35A0: 48 8B 52 28        mov         rdx,qword ptr [rdx+28h]
  00000001411E35A4: 48 8B 8C 24 50 02  mov         rcx,qword ptr [rsp+0000000000000250h]
                    00 00
  00000001411E35AC: 48 8B 49 20        mov         rcx,qword ptr [rcx+20h]
  00000001411E35B0: FF 15 F2 3C C2 00  call        qword ptr [0000000141E072A8h]
  00000001411E35B6: B9 28 00 00 00     mov         ecx,28h
  00000001411E35BB: E8 FC B4 28 00     call        000000014146EABC
  00000001411E35C0: 48 89 84 24 30 01  mov         qword ptr [rsp+0000000000000130h],rax
                    00 00
  00000001411E35C8: 48 83 BC 24 30 01  cmp         qword ptr [rsp+0000000000000130h],0
                    00 00 00
  00000001411E35D1: 74 4D              je          00000001411E3620
  00000001411E35D3: 48 8B 94 24 58 02  mov         rdx,qword ptr [rsp+0000000000000258h]
                    00 00
  00000001411E35DB: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  00000001411E35E3: FF 15 97 37 C2 00  call        qword ptr [0000000141E06D80h]
  00000001411E35E9: 4C 8B 9C 24 30 01  mov         r11,qword ptr [rsp+0000000000000130h]
                    00 00
  00000001411E35F1: 48 8D 05 B0 91 FA  lea         rax,[000000014218C7A8h]
                    00
  00000001411E35F8: 49 89 03           mov         qword ptr [r11],rax
  00000001411E35FB: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  00000001411E3603: 48 8D 05 66 91 FA  lea         rax,[000000014218C770h]
                    00
  00000001411E360A: 48 89 41 10        mov         qword ptr [rcx+10h],rax
  00000001411E360E: 48 8B 84 24 30 01  mov         rax,qword ptr [rsp+0000000000000130h]
                    00 00
  00000001411E3616: 48 89 84 24 18 02  mov         qword ptr [rsp+0000000000000218h],rax
                    00 00
  00000001411E361E: EB 0C              jmp         00000001411E362C
  00000001411E3620: 48 C7 84 24 18 02  mov         qword ptr [rsp+0000000000000218h],0
                    00 00 00 00 00 00
  00000001411E362C: 48 8B 84 24 18 02  mov         rax,qword ptr [rsp+0000000000000218h]
                    00 00
  00000001411E3634: 48 89 84 24 28 01  mov         qword ptr [rsp+0000000000000128h],rax
                    00 00
  00000001411E363C: 48 8B 8C 24 50 02  mov         rcx,qword ptr [rsp+0000000000000250h]
                    00 00
  00000001411E3644: 48 8B 84 24 28 01  mov         rax,qword ptr [rsp+0000000000000128h]
                    00 00
  00000001411E364C: 48 89 41 30        mov         qword ptr [rcx+30h],rax
  00000001411E3650: 41 B8 FF FF FF FF  mov         r8d,0FFFFFFFFh
  00000001411E3656: 48 8D 15 EB 66 AF  lea         rdx,[0000000142CD9D48h]
                    01
  00000001411E365D: 48 8D 8C 24 38 01  lea         rcx,[rsp+0000000000000138h]
                    00 00
  00000001411E3665: FF 15 E5 0A C2 00  call        qword ptr [0000000141E04150h]
  00000001411E366B: 48 89 84 24 20 02  mov         qword ptr [rsp+0000000000000220h],rax
                    00 00
  00000001411E3673: 48 8B 84 24 20 02  mov         rax,qword ptr [rsp+0000000000000220h]
                    00 00
  00000001411E367B: 48 89 84 24 28 02  mov         qword ptr [rsp+0000000000000228h],rax
                    00 00
  00000001411E3683: 48 8B 94 24 28 02  mov         rdx,qword ptr [rsp+0000000000000228h]
                    00 00
  00000001411E368B: 48 8B 8C 24 50 02  mov         rcx,qword ptr [rsp+0000000000000250h]
                    00 00
  00000001411E3693: 48 8B 49 30        mov         rcx,qword ptr [rcx+30h]
  00000001411E3697: FF 15 BB 0A C2 00  call        qword ptr [0000000141E04158h]
  00000001411E369D: 90                 nop
  00000001411E369E: 48 8D 8C 24 38 01  lea         rcx,[rsp+0000000000000138h]
                    00 00
  00000001411E36A6: FF 15 34 23 C2 00  call        qword ptr [0000000141E059E0h]
  00000001411E36AC: 48 8D 84 24 40 01  lea         rax,[rsp+0000000000000140h]
                    00 00
  00000001411E36B4: 48 89 84 24 48 01  mov         qword ptr [rsp+0000000000000148h],rax
                    00 00
  00000001411E36BC: 33 D2              xor         edx,edx
  00000001411E36BE: 48 8B 8C 24 48 01  mov         rcx,qword ptr [rsp+0000000000000148h]
                    00 00
  00000001411E36C6: E8 A5 35 36 FF     call        0000000140546C70
  00000001411E36CB: 4C 8B C8           mov         r9,rax
  00000001411E36CE: 45 33 C0           xor         r8d,r8d
  00000001411E36D1: 48 8B 94 24 50 02  mov         rdx,qword ptr [rsp+0000000000000250h]
                    00 00
  00000001411E36D9: 48 8B 52 30        mov         rdx,qword ptr [rdx+30h]
  00000001411E36DD: 48 8B 8C 24 50 02  mov         rcx,qword ptr [rsp+0000000000000250h]
                    00 00
  00000001411E36E5: 48 8B 49 20        mov         rcx,qword ptr [rcx+20h]
  00000001411E36E9: FF 15 B9 3B C2 00  call        qword ptr [0000000141E072A8h]
  00000001411E36EF: 45 33 C0           xor         r8d,r8d
  00000001411E36F2: 48 8B 94 24 50 02  mov         rdx,qword ptr [rsp+0000000000000250h]
                    00 00
  00000001411E36FA: 48 8B 52 20        mov         rdx,qword ptr [rdx+20h]
  00000001411E36FE: 48 8B 8C 24 50 02  mov         rcx,qword ptr [rsp+0000000000000250h]
                    00 00
  00000001411E3706: 48 8B 09           mov         rcx,qword ptr [rcx]
  00000001411E3709: FF 15 E9 29 C2 00  call        qword ptr [0000000141E060F8h]
  00000001411E370F: B9 30 00 00 00     mov         ecx,30h
  00000001411E3714: E8 A3 B3 28 00     call        000000014146EABC
  00000001411E3719: 48 89 84 24 58 01  mov         qword ptr [rsp+0000000000000158h],rax
                    00 00
  00000001411E3721: 48 83 BC 24 58 01  cmp         qword ptr [rsp+0000000000000158h],0
                    00 00 00
  00000001411E372A: 74 4B              je          00000001411E3777
  00000001411E372C: C7 44 24 20 07 00  mov         dword ptr [rsp+20h],7
                    00 00
  00000001411E3734: 41 B9 01 00 00 00  mov         r9d,1
  00000001411E373A: 41 B8 28 00 00 00  mov         r8d,28h
  00000001411E3740: BA 14 00 00 00     mov         edx,14h
  00000001411E3745: 48 8B 8C 24 58 01  mov         rcx,qword ptr [rsp+0000000000000158h]
                    00 00
  00000001411E374D: FF 15 1D 36 C2 00  call        qword ptr [0000000141E06D70h]
  00000001411E3753: 4C 8B 9C 24 58 01  mov         r11,qword ptr [rsp+0000000000000158h]
                    00 00
  00000001411E375B: 48 8D 05 D6 93 FA  lea         rax,[000000014218CB38h]
                    00
  00000001411E3762: 49 89 03           mov         qword ptr [r11],rax
  00000001411E3765: 48 8B 84 24 58 01  mov         rax,qword ptr [rsp+0000000000000158h]
                    00 00
  00000001411E376D: 48 89 84 24 30 02  mov         qword ptr [rsp+0000000000000230h],rax
                    00 00
  00000001411E3775: EB 0C              jmp         00000001411E3783
  00000001411E3777: 48 C7 84 24 30 02  mov         qword ptr [rsp+0000000000000230h],0
                    00 00 00 00 00 00
  00000001411E3783: 48 8B 84 24 30 02  mov         rax,qword ptr [rsp+0000000000000230h]
                    00 00
  00000001411E378B: 48 89 84 24 50 01  mov         qword ptr [rsp+0000000000000150h],rax
                    00 00
  00000001411E3793: 48 8B 8C 24 50 02  mov         rcx,qword ptr [rsp+0000000000000250h]
                    00 00
  00000001411E379B: 48 8B 84 24 50 01  mov         rax,qword ptr [rsp+0000000000000150h]
                    00 00
  00000001411E37A3: 48 89 41 38        mov         qword ptr [rcx+38h],rax
  00000001411E37A7: 48 8B 84 24 50 02  mov         rax,qword ptr [rsp+0000000000000250h]
                    00 00
  00000001411E37AF: 48 8B 00           mov         rax,qword ptr [rax]
  00000001411E37B2: 48 8B 8C 24 50 02  mov         rcx,qword ptr [rsp+0000000000000250h]
                    00 00
  00000001411E37BA: 48 8B 09           mov         rcx,qword ptr [rcx]
  00000001411E37BD: 48 8B 00           mov         rax,qword ptr [rax]
  00000001411E37C0: 48 8B 94 24 50 02  mov         rdx,qword ptr [rsp+0000000000000250h]
                    00 00
  00000001411E37C8: 48 8B 52 38        mov         rdx,qword ptr [rdx+38h]
  00000001411E37CC: FF 50 58           call        qword ptr [rax+58h]
  00000001411E37CF: 48 8B 94 24 58 02  mov         rdx,qword ptr [rsp+0000000000000258h]
                    00 00
  00000001411E37D7: 48 8B 8C 24 50 02  mov         rcx,qword ptr [rsp+0000000000000250h]
                    00 00
  00000001411E37DF: E8 1C 00 00 00     call        00000001411E3800
  00000001411E37E4: 48 8B 8C 24 58 02  mov         rcx,qword ptr [rsp+0000000000000258h]
                    00 00
  00000001411E37EC: FF 15 2E 09 C2 00  call        qword ptr [0000000141E04120h]
  00000001411E37F2: 48 81 C4 48 02 00  add         rsp,248h
                    00
  00000001411E37F9: C3                 ret

  Summary

     1E02000 .text
