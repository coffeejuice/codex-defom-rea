Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006712EC70: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  000000006712EC75: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000006712EC7A: 53                 push        rbx
  000000006712EC7B: 56                 push        rsi
  000000006712EC7C: 57                 push        rdi
  000000006712EC7D: 41 54              push        r12
  000000006712EC7F: 41 55              push        r13
  000000006712EC81: 41 56              push        r14
  000000006712EC83: 41 57              push        r15
  000000006712EC85: 48 81 EC 90 00 00  sub         rsp,90h
                    00
  000000006712EC8C: 48 C7 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  000000006712EC98: 49 8B F1           mov         rsi,r9
  000000006712EC9B: 48 8B F9           mov         rdi,rcx
  000000006712EC9E: 45 33 C9           xor         r9d,r9d
  000000006712ECA1: 45 8B D9           mov         r11d,r9d
  000000006712ECA4: 45 8B D1           mov         r10d,r9d
  000000006712ECA7: 48 8B 02           mov         rax,qword ptr [rdx]
  000000006712ECAA: 48 85 C0           test        rax,rax
  000000006712ECAD: 74 23              je          000000006712ECD2
  000000006712ECAF: 90                 nop
  000000006712ECB0: 48 8B 50 10        mov         rdx,qword ptr [rax+10h]
  000000006712ECB4: 44 8B 4A 10        mov         r9d,dword ptr [rdx+10h]
  000000006712ECB8: 45 03 D9           add         r11d,r9d
  000000006712ECBB: 83 3A 04           cmp         dword ptr [rdx],4
  000000006712ECBE: 7C 04              jl          000000006712ECC4
  000000006712ECC0: 44 8B 4A 34        mov         r9d,dword ptr [rdx+34h]
  000000006712ECC4: 45 03 D1           add         r10d,r9d
  000000006712ECC7: 48 8B 00           mov         rax,qword ptr [rax]
  000000006712ECCA: 48 85 C0           test        rax,rax
  000000006712ECCD: 75 E1              jne         000000006712ECB0
  000000006712ECCF: 45 33 C9           xor         r9d,r9d
  000000006712ECD2: 43 8D 1C 02        lea         ebx,[r10+r8]
  000000006712ECD6: 48 8B 51 08        mov         rdx,qword ptr [rcx+8]
  000000006712ECDA: 83 FB 40           cmp         ebx,40h
  000000006712ECDD: 73 2F              jae         000000006712ED0E
  000000006712ECDF: 8B CB              mov         ecx,ebx
  000000006712ECE1: 83 E1 1F           and         ecx,1Fh
  000000006712ECE4: 8B C3              mov         eax,ebx
  000000006712ECE6: 48 C1 E8 05        shr         rax,5
  000000006712ECEA: 8B 44 82 60        mov         eax,dword ptr [rdx+rax*4+60h]
  000000006712ECEE: 0F A3 C8           bt          eax,ecx
  000000006712ECF1: 72 1B              jb          000000006712ED0E
  000000006712ECF3: 48 8B 05 06 55 1C  mov         rax,qword ptr [00000000672F4200h]
                    00
  000000006712ECFA: 48 85 C0           test        rax,rax
  000000006712ECFD: 75 16              jne         000000006712ED15
  000000006712ECFF: 48 39 05 0A 55 1C  cmp         qword ptr [00000000672F4210h],rax
                    00
  000000006712ED06: 0F 84 51 08 00 00  je          000000006712F55D
  000000006712ED0C: EB 07              jmp         000000006712ED15
  000000006712ED0E: 48 8B 05 EB 54 1C  mov         rax,qword ptr [00000000672F4200h]
                    00
  000000006712ED15: F6 42 20 04        test        byte ptr [rdx+20h],4
  000000006712ED19: 0F 85 3E 08 00 00  jne         000000006712F55D
  000000006712ED1F: 47 8D 2C 03        lea         r13d,[r11+r8]
  000000006712ED23: 44 89 AC 24 D8 00  mov         dword ptr [rsp+00000000000000D8h],r13d
                    00 00
  000000006712ED2B: 4C 89 4C 24 38     mov         qword ptr [rsp+38h],r9
  000000006712ED30: 48 85 C0           test        rax,rax
  000000006712ED33: 74 14              je          000000006712ED49
  000000006712ED35: 4C 8D 44 24 38     lea         r8,[rsp+38h]
  000000006712ED3A: 48 85 F6           test        rsi,rsi
  000000006712ED3D: 4C 0F 45 C6        cmovne      r8,rsi
  000000006712ED41: 41 8B D5           mov         edx,r13d
  000000006712ED44: 48 8B CF           mov         rcx,rdi
  000000006712ED47: FF D0              call        rax
  000000006712ED49: E8 02 A7 EF FF     call        0000000067029450
  000000006712ED4E: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  000000006712ED56: 48 8B CF           mov         rcx,rdi
  000000006712ED59: E8 62 B6 FF FF     call        000000006712A3C0
  000000006712ED5E: 4C 8B E0           mov         r12,rax
  000000006712ED61: 48 85 C0           test        rax,rax
  000000006712ED64: 74 40              je          000000006712EDA6
  000000006712ED66: 48 8B 08           mov         rcx,qword ptr [rax]
  000000006712ED69: F6 41 04 01        test        byte ptr [rcx+4],1
  000000006712ED6D: 74 0F              je          000000006712ED7E
  000000006712ED6F: 48 8B C8           mov         rcx,rax
  000000006712ED72: E8 39 70 EF FF     call        0000000067025DB0
  000000006712ED77: BF 01 00 00 00     mov         edi,1
  000000006712ED7C: EB 1A              jmp         000000006712ED98
  000000006712ED7E: BF 01 00 00 00     mov         edi,1
  000000006712ED83: 33 C0              xor         eax,eax
  000000006712ED85: F0 0F B1 39        lock cmpxchg dword ptr [rcx],edi
  000000006712ED89: 0F 94 C1           sete        cl
  000000006712ED8C: 84 C9              test        cl,cl
  000000006712ED8E: 75 08              jne         000000006712ED98
  000000006712ED90: 49 8B CC           mov         rcx,r12
  000000006712ED93: E8 18 6E EF FF     call        0000000067025BB0
  000000006712ED98: 49 83 CC 01        or          r12,1
  000000006712ED9C: 4C 89 64 24 30     mov         qword ptr [rsp+30h],r12
  000000006712EDA1: 45 33 C0           xor         r8d,r8d
  000000006712EDA4: EB 0F              jmp         000000006712EDB5
  000000006712EDA6: 45 33 C0           xor         r8d,r8d
  000000006712EDA9: 4D 8B E0           mov         r12,r8
  000000006712EDAC: 4C 89 44 24 30     mov         qword ptr [rsp+30h],r8
  000000006712EDB1: 41 8D 78 01        lea         edi,[r8+1]
  000000006712EDB5: 4C 8B 94 24 D0 00  mov         r10,qword ptr [rsp+00000000000000D0h]
                    00 00
  000000006712EDBD: 49 8B 42 08        mov         rax,qword ptr [r10+8]
  000000006712EDC1: 4C 8B 78 48        mov         r15,qword ptr [rax+48h]
  000000006712EDC5: 4C 89 7C 24 40     mov         qword ptr [rsp+40h],r15
  000000006712EDCA: 4D 85 FF           test        r15,r15
  000000006712EDCD: 0F 85 B0 00 00 00  jne         000000006712EE83
  000000006712EDD3: 41 F6 C4 01        test        r12b,1
  000000006712EDD7: 74 4A              je          000000006712EE23
  000000006712EDD9: 49 83 E4 FE        and         r12,0FFFFFFFFFFFFFFFEh
  000000006712EDDD: 4C 89 64 24 30     mov         qword ptr [rsp+30h],r12
  000000006712EDE2: 49 8B CC           mov         rcx,r12
  000000006712EDE5: 48 83 E1 FE        and         rcx,0FFFFFFFFFFFFFFFEh
  000000006712EDE9: 48 8B 11           mov         rdx,qword ptr [rcx]
  000000006712EDEC: F6 42 04 01        test        byte ptr [rdx+4],1
  000000006712EDF0: 74 0F              je          000000006712EE01
  000000006712EDF2: E8 39 6D EF FF     call        0000000067025B30
  000000006712EDF7: 4C 8B 94 24 D0 00  mov         r10,qword ptr [rsp+00000000000000D0h]
                    00 00
  000000006712EDFF: EB 22              jmp         000000006712EE23
  000000006712EE01: 8B C7              mov         eax,edi
  000000006712EE03: F0 44 0F B1 02     lock cmpxchg dword ptr [rdx],r8d
  000000006712EE08: 74 14              je          000000006712EE1E
  000000006712EE0A: E8 61 6D EF FF     call        0000000067025B70
  000000006712EE0F: 4C 8B 64 24 30     mov         r12,qword ptr [rsp+30h]
  000000006712EE14: 4C 8B 94 24 D0 00  mov         r10,qword ptr [rsp+00000000000000D0h]
                    00 00
  000000006712EE1C: EB 05              jmp         000000006712EE23
  000000006712EE1E: 4C 8B 64 24 30     mov         r12,qword ptr [rsp+30h]
  000000006712EE23: 48 8B 05 E6 53 1C  mov         rax,qword ptr [00000000672F4210h]
                    00
  000000006712EE2A: 48 85 C0           test        rax,rax
  000000006712EE2D: 74 09              je          000000006712EE38
  000000006712EE2F: 41 8B D5           mov         edx,r13d
  000000006712EE32: 49 8B CA           mov         rcx,r10
  000000006712EE35: FF D0              call        rax
  000000006712EE37: 90                 nop
  000000006712EE38: 41 F6 C4 01        test        r12b,1
  000000006712EE3C: 0F 84 1B 07 00 00  je          000000006712F55D
  000000006712EE42: 49 83 E4 FE        and         r12,0FFFFFFFFFFFFFFFEh
  000000006712EE46: 4C 89 64 24 30     mov         qword ptr [rsp+30h],r12
  000000006712EE4B: 49 83 E4 FE        and         r12,0FFFFFFFFFFFFFFFEh
  000000006712EE4F: 49 8B 0C 24        mov         rcx,qword ptr [r12]
  000000006712EE53: F6 41 04 01        test        byte ptr [rcx+4],1
  000000006712EE57: 74 0D              je          000000006712EE66
  000000006712EE59: 49 8B CC           mov         rcx,r12
  000000006712EE5C: E8 CF 6C EF FF     call        0000000067025B30
  000000006712EE61: E9 F7 06 00 00     jmp         000000006712F55D
  000000006712EE66: 8B C7              mov         eax,edi
  000000006712EE68: 45 33 C0           xor         r8d,r8d
  000000006712EE6B: F0 44 0F B1 01     lock cmpxchg dword ptr [rcx],r8d
  000000006712EE70: 0F 84 E7 06 00 00  je          000000006712F55D
  000000006712EE76: 49 8B CC           mov         rcx,r12
  000000006712EE79: E8 F2 6C EF FF     call        0000000067025B70
  000000006712EE7E: E9 DA 06 00 00     jmp         000000006712F55D
  000000006712EE83: 41 FF 47 0C        inc         dword ptr [r15+0Ch]
  000000006712EE87: 49 8B 07           mov         rax,qword ptr [r15]
  000000006712EE8A: 3B 58 08           cmp         ebx,dword ptr [rax+8]
  000000006712EE8D: 7D 14              jge         000000006712EEA3
  000000006712EE8F: 48 63 CB           movsxd      rcx,ebx
  000000006712EE92: 48 FF C1           inc         rcx
  000000006712EE95: 48 C1 E1 04        shl         rcx,4
  000000006712EE99: 48 03 C8           add         rcx,rax
  000000006712EE9C: 48 89 4C 24 58     mov         qword ptr [rsp+58h],rcx
  000000006712EEA1: EB 0D              jmp         000000006712EEB0
  000000006712EEA3: 49 8D 4F 10        lea         rcx,[r15+10h]
  000000006712EEA7: 48 89 4C 24 58     mov         qword ptr [rsp+58h],rcx
  000000006712EEAC: 0F 1F 40 00        nop         dword ptr [rax]
  000000006712EEB0: 4C 8B 31           mov         r14,qword ptr [rcx]
  000000006712EEB3: 4D 85 F6           test        r14,r14
  000000006712EEB6: 0F 84 52 05 00 00  je          000000006712F40E
  000000006712EEBC: 4C 8B 79 08        mov         r15,qword ptr [rcx+8]
  000000006712EEC0: 4C 89 7C 24 48     mov         qword ptr [rsp+48h],r15
  000000006712EEC5: 49 8B C7           mov         rax,r15
  000000006712EEC8: EB 09              jmp         000000006712EED3
  000000006712EECA: 66 0F 1F 44 00 00  nop         word ptr [rax+rax]
  000000006712EED0: 45 33 C0           xor         r8d,r8d
  000000006712EED3: 4D 8B 7E 08        mov         r15,qword ptr [r14+8]
  000000006712EED7: 4D 85 FF           test        r15,r15
  000000006712EEDA: 0F 84 EE 04 00 00  je          000000006712F3CE
  000000006712EEE0: 4C 89 7C 24 78     mov         qword ptr [rsp+78h],r15
  000000006712EEE5: 49 8B 47 08        mov         rax,qword ptr [r15+8]
  000000006712EEE9: 48 8B 48 40        mov         rcx,qword ptr [rax+40h]
  000000006712EEED: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  000000006712EEF5: 48 3B 41 10        cmp         rax,qword ptr [rcx+10h]
  000000006712EEF9: 41 0F 94 C5        sete        r13b
  000000006712EEFD: 44 88 AC 24 E0 00  mov         byte ptr [rsp+00000000000000E0h],r13b
                    00 00
  000000006712EF05: 41 0F B7 46 3C     movzx       eax,word ptr [r14+3Ch]
  000000006712EF0A: 66 83 E0 07        and         ax,7
  000000006712EF0E: 75 05              jne         000000006712EF15
  000000006712EF10: 45 84 ED           test        r13b,r13b
  000000006712EF13: 74 06              je          000000006712EF1B
  000000006712EF15: 66 83 F8 02        cmp         ax,2
  000000006712EF19: 75 39              jne         000000006712EF54
  000000006712EF1B: 4C 8D 4C 24 38     lea         r9,[rsp+38h]
  000000006712EF20: 48 85 F6           test        rsi,rsi
  000000006712EF23: 4C 0F 45 CE        cmovne      r9,rsi
  000000006712EF27: 4D 8B C6           mov         r8,r14
  000000006712EF2A: 44 8B AC 24 D8 00  mov         r13d,dword ptr [rsp+00000000000000D8h]
                    00 00
  000000006712EF32: 41 8B D5           mov         edx,r13d
  000000006712EF35: 49 8B CA           mov         rcx,r10
  000000006712EF38: E8 D3 D3 FF FF     call        000000006712C310
  000000006712EF3D: 4C 8B 94 24 D0 00  mov         r10,qword ptr [rsp+00000000000000D0h]
                    00 00
  000000006712EF45: 4C 8B 7C 24 40     mov         r15,qword ptr [rsp+40h]
  000000006712EF4A: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  000000006712EF4F: E9 7F 04 00 00     jmp         000000006712F3D3
  000000006712EF54: 66 83 F8 04        cmp         ax,4
  000000006712EF58: 0F 85 AA 01 00 00  jne         000000006712F108
  000000006712EF5E: 41 F6 C4 01        test        r12b,1
  000000006712EF62: 74 3A              je          000000006712EF9E
  000000006712EF64: 49 83 E4 FE        and         r12,0FFFFFFFFFFFFFFFEh
  000000006712EF68: 4C 89 64 24 30     mov         qword ptr [rsp+30h],r12
  000000006712EF6D: 49 8B CC           mov         rcx,r12
  000000006712EF70: 48 83 E1 FE        and         rcx,0FFFFFFFFFFFFFFFEh
  000000006712EF74: 48 8B 11           mov         rdx,qword ptr [rcx]
  000000006712EF77: F6 42 04 01        test        byte ptr [rdx+4],1
  000000006712EF7B: 74 07              je          000000006712EF84
  000000006712EF7D: E8 AE 6B EF FF     call        0000000067025B30
  000000006712EF82: EB 1A              jmp         000000006712EF9E
  000000006712EF84: 8B C7              mov         eax,edi
  000000006712EF86: F0 44 0F B1 02     lock cmpxchg dword ptr [rdx],r8d
  000000006712EF8B: 74 0C              je          000000006712EF99
  000000006712EF8D: E8 DE 6B EF FF     call        0000000067025B70
  000000006712EF92: 4C 8B 64 24 30     mov         r12,qword ptr [rsp+30h]
  000000006712EF97: EB 05              jmp         000000006712EF9E
  000000006712EF99: 4C 8B 64 24 30     mov         r12,qword ptr [rsp+30h]
  000000006712EF9E: 45 84 ED           test        r13b,r13b
  000000006712EFA1: 74 37              je          000000006712EFDA
  000000006712EFA3: 49 8B 07           mov         rax,qword ptr [r15]
  000000006712EFA6: 49 8B CF           mov         rcx,r15
  000000006712EFA9: FF 10              call        qword ptr [rax]
  000000006712EFAB: 48 8B 58 08        mov         rbx,qword ptr [rax+8]
  000000006712EFAF: 48 8B B4 24 D0 00  mov         rsi,qword ptr [rsp+00000000000000D0h]
                    00 00
  000000006712EFB7: 48 8B 06           mov         rax,qword ptr [rsi]
  000000006712EFBA: 48 8B CE           mov         rcx,rsi
  000000006712EFBD: FF 10              call        qword ptr [rax]
  000000006712EFBF: 4C 89 7C 24 20     mov         qword ptr [rsp+20h],r15
  000000006712EFC4: 4C 8B CB           mov         r9,rbx
  000000006712EFC7: 4C 8B C6           mov         r8,rsi
  000000006712EFCA: 48 8B 50 08        mov         rdx,qword ptr [rax+8]
  000000006712EFCE: 48 8D 0D CB DC 10  lea         rcx,[000000006723CCA0h]
                    00
  000000006712EFD5: E8 06 DF EE FF     call        000000006701CEE0
  000000006712EFDA: 33 D2              xor         edx,edx
  000000006712EFDC: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  000000006712EFE1: E8 0A 80 EF FF     call        0000000067026FF0
  000000006712EFE6: 90                 nop
  000000006712EFE7: B9 50 00 00 00     mov         ecx,50h
  000000006712EFEC: E8 95 9A 04 00     call        0000000067178A86
  000000006712EFF1: 4C 8B E8           mov         r13,rax
  000000006712EFF4: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  000000006712EFF9: 48 85 C0           test        rax,rax
  000000006712EFFC: 74 7D              je          000000006712F07B
  000000006712EFFE: 49 8B 7E 10        mov         rdi,qword ptr [r14+10h]
  000000006712F002: 41 0F B7 76 3A     movzx       esi,word ptr [r14+3Ah]
  000000006712F007: 41 0F B7 5E 38     movzx       ebx,word ptr [r14+38h]
  000000006712F00C: BA 2B 00 00 00     mov         edx,2Bh
  000000006712F011: 48 8B C8           mov         rcx,rax
  000000006712F014: E8 37 E5 FE FF     call        000000006711D550
  000000006712F019: 48 8D 05 B0 A3 07  lea         rax,[00000000671A93D0h]
                    00
  000000006712F020: 49 89 45 00        mov         qword ptr [r13],rax
  000000006712F024: 48 8B 84 24 D0 00  mov         rax,qword ptr [rsp+00000000000000D0h]
                    00 00
  000000006712F02C: 49 89 45 18        mov         qword ptr [r13+18h],rax
  000000006712F030: 8B 84 24 D8 00 00  mov         eax,dword ptr [rsp+00000000000000D8h]
                    00
  000000006712F037: 41 89 45 20        mov         dword ptr [r13+20h],eax
  000000006712F03B: 33 C0              xor         eax,eax
  000000006712F03D: 41 89 45 24        mov         dword ptr [r13+24h],eax
  000000006712F041: 49 89 45 28        mov         qword ptr [r13+28h],rax
  000000006712F045: 48 8D 44 24 38     lea         rax,[rsp+38h]
  000000006712F04A: 48 8B 8C 24 E8 00  mov         rcx,qword ptr [rsp+00000000000000E8h]
                    00 00
  000000006712F052: 48 85 C9           test        rcx,rcx
  000000006712F055: 48 0F 45 C1        cmovne      rax,rcx
  000000006712F059: 49 89 45 30        mov         qword ptr [r13+30h],rax
  000000006712F05D: 48 8D 44 24 60     lea         rax,[rsp+60h]
  000000006712F062: 49 89 45 38        mov         qword ptr [r13+38h],rax
  000000006712F066: 49 89 7D 40        mov         qword ptr [r13+40h],rdi
  000000006712F06A: 66 41 89 5D 48     mov         word ptr [r13+48h],bx
  000000006712F06F: 66 41 89 75 4A     mov         word ptr [r13+4Ah],si
  000000006712F074: BF 01 00 00 00     mov         edi,1
  000000006712F079: EB 06              jmp         000000006712F081
  000000006712F07B: 45 33 C0           xor         r8d,r8d
  000000006712F07E: 4D 8B E8           mov         r13,r8
  000000006712F081: 49 8B D5           mov         rdx,r13
  000000006712F084: 49 8B CF           mov         rcx,r15
  000000006712F087: E8 44 CF FE FF     call        000000006711BFD0
  000000006712F08C: 8B D7              mov         edx,edi
  000000006712F08E: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  000000006712F093: E8 08 80 EF FF     call        00000000670270A0
  000000006712F098: 4D 85 E4           test        r12,r12
  000000006712F09B: 74 4A              je          000000006712F0E7
  000000006712F09D: 41 F6 C4 01        test        r12b,1
  000000006712F0A1: 75 44              jne         000000006712F0E7
  000000006712F0A3: 49 8B D4           mov         rdx,r12
  000000006712F0A6: 48 83 E2 FE        and         rdx,0FFFFFFFFFFFFFFFEh
  000000006712F0AA: 48 8B 0A           mov         rcx,qword ptr [rdx]
  000000006712F0AD: F6 41 04 01        test        byte ptr [rcx+4],1
  000000006712F0B1: 74 0A              je          000000006712F0BD
  000000006712F0B3: 48 8B CA           mov         rcx,rdx
  000000006712F0B6: E8 F5 6C EF FF     call        0000000067025DB0
  000000006712F0BB: EB 21              jmp         000000006712F0DE
  000000006712F0BD: 33 C0              xor         eax,eax
  000000006712F0BF: F0 0F B1 39        lock cmpxchg dword ptr [rcx],edi
  000000006712F0C3: 0F 94 C1           sete        cl
  000000006712F0C6: 84 C9              test        cl,cl
  000000006712F0C8: 75 0F              jne         000000006712F0D9
  000000006712F0CA: 48 8B CA           mov         rcx,rdx
  000000006712F0CD: E8 DE 6A EF FF     call        0000000067025BB0
  000000006712F0D2: 4C 8B 64 24 30     mov         r12,qword ptr [rsp+30h]
  000000006712F0D7: EB 05              jmp         000000006712F0DE
  000000006712F0D9: 4C 8B 64 24 30     mov         r12,qword ptr [rsp+30h]
  000000006712F0DE: 49 83 CC 01        or          r12,1
  000000006712F0E2: 4C 89 64 24 30     mov         qword ptr [rsp+30h],r12
  000000006712F0E7: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  000000006712F0EC: E8 6F 7F EF FF     call        0000000067027060
  000000006712F0F1: 4C 8B 94 24 D0 00  mov         r10,qword ptr [rsp+00000000000000D0h]
                    00 00
  000000006712F0F9: 4C 8B 7C 24 40     mov         r15,qword ptr [rsp+40h]
  000000006712F0FE: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  000000006712F103: E9 CB 02 00 00     jmp         000000006712F3D3
  000000006712F108: 49 8B F0           mov         rsi,r8
  000000006712F10B: 4C 89 44 24 50     mov         qword ptr [rsp+50h],r8
  000000006712F110: 45 84 ED           test        r13b,r13b
  000000006712F113: 74 2A              je          000000006712F13F
  000000006712F115: 4C 89 54 24 68     mov         qword ptr [rsp+68h],r10
  000000006712F11A: 8B 84 24 D8 00 00  mov         eax,dword ptr [rsp+00000000000000D8h]
                    00
  000000006712F121: 89 44 24 70        mov         dword ptr [rsp+70h],eax
  000000006712F125: 89 7C 24 74        mov         dword ptr [rsp+74h],edi
  000000006712F129: 49 8B 47 08        mov         rax,qword ptr [r15+8]
  000000006712F12D: 48 8B 70 58        mov         rsi,qword ptr [rax+58h]
  000000006712F131: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  000000006712F136: 48 89 48 58        mov         qword ptr [rax+58h],rcx
  000000006712F13A: 48 89 74 24 50     mov         qword ptr [rsp+50h],rsi
  000000006712F13F: 49 8B 5E 10        mov         rbx,qword ptr [r14+10h]
  000000006712F143: 41 0F B7 7E 3A     movzx       edi,word ptr [r14+3Ah]
  000000006712F148: 48 85 DB           test        rbx,rbx
  000000006712F14B: 0F 84 37 01 00 00  je          000000006712F288
  000000006712F151: 49 8B 07           mov         rax,qword ptr [r15]
  000000006712F154: 49 8B CF           mov         rcx,r15
  000000006712F157: FF 10              call        qword ptr [rax]
  000000006712F159: 48 8B C8           mov         rcx,rax
  000000006712F15C: E8 BF EC FE FF     call        000000006711DE20
  000000006712F161: 41 0F B7 4E 38     movzx       ecx,word ptr [r14+38h]
  000000006712F166: 3B C8              cmp         ecx,eax
  000000006712F168: 0F 8F 17 01 00 00  jg          000000006712F285
  000000006712F16E: 41 F6 C4 01        test        r12b,1
  000000006712F172: 74 40              je          000000006712F1B4
  000000006712F174: 49 83 E4 FE        and         r12,0FFFFFFFFFFFFFFFEh
  000000006712F178: 4C 89 64 24 30     mov         qword ptr [rsp+30h],r12
  000000006712F17D: 49 8B CC           mov         rcx,r12
  000000006712F180: 48 83 E1 FE        and         rcx,0FFFFFFFFFFFFFFFEh
  000000006712F184: 48 8B 11           mov         rdx,qword ptr [rcx]
  000000006712F187: F6 42 04 01        test        byte ptr [rdx+4],1
  000000006712F18B: 74 07              je          000000006712F194
  000000006712F18D: E8 9E 69 EF FF     call        0000000067025B30
  000000006712F192: EB 20              jmp         000000006712F1B4
  000000006712F194: B8 01 00 00 00     mov         eax,1
  000000006712F199: 45 33 C0           xor         r8d,r8d
  000000006712F19C: F0 44 0F B1 02     lock cmpxchg dword ptr [rdx],r8d
  000000006712F1A1: 74 0C              je          000000006712F1AF
  000000006712F1A3: E8 C8 69 EF FF     call        0000000067025B70
  000000006712F1A8: 4C 8B 64 24 30     mov         r12,qword ptr [rsp+30h]
  000000006712F1AD: EB 05              jmp         000000006712F1B4
  000000006712F1AF: 4C 8B 64 24 30     mov         r12,qword ptr [rsp+30h]
  000000006712F1B4: 4C 8B 0D 4D 50 1C  mov         r9,qword ptr [00000000672F4208h]
                    00
  000000006712F1BB: 4D 85 C9           test        r9,r9
  000000006712F1BE: 74 26              je          000000006712F1E6
  000000006712F1C0: 4C 8D 44 24 38     lea         r8,[rsp+38h]
  000000006712F1C5: 48 8B 8C 24 E8 00  mov         rcx,qword ptr [rsp+00000000000000E8h]
                    00 00
  000000006712F1CD: 48 85 C9           test        rcx,rcx
  000000006712F1D0: 4C 0F 45 C1        cmovne      r8,rcx
  000000006712F1D4: 41 0F B7 56 38     movzx       edx,word ptr [r14+38h]
  000000006712F1D9: 41 0F B7 46 3A     movzx       eax,word ptr [r14+3Ah]
  000000006712F1DE: 03 D0              add         edx,eax
  000000006712F1E0: 49 8B CF           mov         rcx,r15
  000000006712F1E3: 41 FF D1           call        r9
  000000006712F1E6: 4C 8D 4C 24 38     lea         r9,[rsp+38h]
  000000006712F1EB: 48 8B 84 24 E8 00  mov         rax,qword ptr [rsp+00000000000000E8h]
                    00 00
  000000006712F1F3: 48 85 C0           test        rax,rax
  000000006712F1F6: 4C 0F 45 C8        cmovne      r9,rax
  000000006712F1FA: 44 8B C7           mov         r8d,edi
  000000006712F1FD: 33 D2              xor         edx,edx
  000000006712F1FF: 49 8B CF           mov         rcx,r15
  000000006712F202: FF D3              call        rbx
  000000006712F204: 4C 8B 1D 0D 50 1C  mov         r11,qword ptr [00000000672F4218h]
                    00
  000000006712F20B: 4D 85 DB           test        r11,r11
  000000006712F20E: 74 12              je          000000006712F222
  000000006712F210: 41 0F B7 56 38     movzx       edx,word ptr [r14+38h]
  000000006712F215: 41 0F B7 46 3A     movzx       eax,word ptr [r14+3Ah]
  000000006712F21A: 03 D0              add         edx,eax
  000000006712F21C: 49 8B CF           mov         rcx,r15
  000000006712F21F: 41 FF D3           call        r11
  000000006712F222: 4D 85 E4           test        r12,r12
  000000006712F225: 0F 84 61 01 00 00  je          000000006712F38C
  000000006712F22B: 41 F6 C4 01        test        r12b,1
  000000006712F22F: 0F 85 57 01 00 00  jne         000000006712F38C
  000000006712F235: 49 8B D4           mov         rdx,r12
  000000006712F238: 48 83 E2 FE        and         rdx,0FFFFFFFFFFFFFFFEh
  000000006712F23C: 48 8B 0A           mov         rcx,qword ptr [rdx]
  000000006712F23F: F6 41 04 01        test        byte ptr [rcx+4],1
  000000006712F243: 74 0A              je          000000006712F24F
  000000006712F245: 48 8B CA           mov         rcx,rdx
  000000006712F248: E8 63 6B EF FF     call        0000000067025DB0
  000000006712F24D: EB 28              jmp         000000006712F277
  000000006712F24F: 33 C0              xor         eax,eax
  000000006712F251: 41 B8 01 00 00 00  mov         r8d,1
  000000006712F257: F0 44 0F B1 01     lock cmpxchg dword ptr [rcx],r8d
  000000006712F25C: 0F 94 C1           sete        cl
  000000006712F25F: 84 C9              test        cl,cl
  000000006712F261: 75 0F              jne         000000006712F272
  000000006712F263: 48 8B CA           mov         rcx,rdx
  000000006712F266: E8 45 69 EF FF     call        0000000067025BB0
  000000006712F26B: 4C 8B 64 24 30     mov         r12,qword ptr [rsp+30h]
  000000006712F270: EB 05              jmp         000000006712F277
  000000006712F272: 4C 8B 64 24 30     mov         r12,qword ptr [rsp+30h]
  000000006712F277: 49 83 CC 01        or          r12,1
  000000006712F27B: 4C 89 64 24 30     mov         qword ptr [rsp+30h],r12
  000000006712F280: E9 07 01 00 00     jmp         000000006712F38C
  000000006712F285: 45 33 C0           xor         r8d,r8d
  000000006712F288: 41 0F B7 5E 38     movzx       ebx,word ptr [r14+38h]
  000000006712F28D: 03 DF              add         ebx,edi
  000000006712F28F: 41 F6 C4 01        test        r12b,1
  000000006712F293: 74 46              je          000000006712F2DB
  000000006712F295: 49 83 E4 FE        and         r12,0FFFFFFFFFFFFFFFEh
  000000006712F299: 4C 89 64 24 30     mov         qword ptr [rsp+30h],r12
  000000006712F29E: 49 8B CC           mov         rcx,r12
  000000006712F2A1: 48 83 E1 FE        and         rcx,0FFFFFFFFFFFFFFFEh
  000000006712F2A5: 48 8B 11           mov         rdx,qword ptr [rcx]
  000000006712F2A8: F6 42 04 01        test        byte ptr [rdx+4],1
  000000006712F2AC: 74 0C              je          000000006712F2BA
  000000006712F2AE: E8 7D 68 EF FF     call        0000000067025B30
  000000006712F2B3: BF 01 00 00 00     mov         edi,1
  000000006712F2B8: EB 26              jmp         000000006712F2E0
  000000006712F2BA: BF 01 00 00 00     mov         edi,1
  000000006712F2BF: 8B C7              mov         eax,edi
  000000006712F2C1: F0 44 0F B1 02     lock cmpxchg dword ptr [rdx],r8d
  000000006712F2C6: 74 0C              je          000000006712F2D4
  000000006712F2C8: E8 A3 68 EF FF     call        0000000067025B70
  000000006712F2CD: 4C 8B 64 24 30     mov         r12,qword ptr [rsp+30h]
  000000006712F2D2: EB 0C              jmp         000000006712F2E0
  000000006712F2D4: 4C 8B 64 24 30     mov         r12,qword ptr [rsp+30h]
  000000006712F2D9: EB 05              jmp         000000006712F2E0
  000000006712F2DB: BF 01 00 00 00     mov         edi,1
  000000006712F2E0: 48 8B 05 21 4F 1C  mov         rax,qword ptr [00000000672F4208h]
                    00
  000000006712F2E7: 48 85 C0           test        rax,rax
  000000006712F2EA: 74 1C              je          000000006712F308
  000000006712F2EC: 4C 8D 44 24 38     lea         r8,[rsp+38h]
  000000006712F2F1: 48 8B 8C 24 E8 00  mov         rcx,qword ptr [rsp+00000000000000E8h]
                    00 00
  000000006712F2F9: 48 85 C9           test        rcx,rcx
  000000006712F2FC: 4C 0F 45 C1        cmovne      r8,rcx
  000000006712F300: 8B D3              mov         edx,ebx
  000000006712F302: 49 8B CF           mov         rcx,r15
  000000006712F305: FF D0              call        rax
  000000006712F307: 90                 nop
  000000006712F308: 4C 8D 4C 24 38     lea         r9,[rsp+38h]
  000000006712F30D: 48 8B 84 24 E8 00  mov         rax,qword ptr [rsp+00000000000000E8h]
                    00 00
  000000006712F315: 48 85 C0           test        rax,rax
  000000006712F318: 4C 0F 45 C8        cmovne      r9,rax
  000000006712F31C: 44 8B C3           mov         r8d,ebx
  000000006712F31F: 33 D2              xor         edx,edx
  000000006712F321: 49 8B CF           mov         rcx,r15
  000000006712F324: E8 87 06 FF FF     call        000000006711F9B0
  000000006712F329: 90                 nop
  000000006712F32A: 48 8B 05 E7 4E 1C  mov         rax,qword ptr [00000000672F4218h]
                    00
  000000006712F331: 48 85 C0           test        rax,rax
  000000006712F334: 74 07              je          000000006712F33D
  000000006712F336: 8B D3              mov         edx,ebx
  000000006712F338: 49 8B CF           mov         rcx,r15
  000000006712F33B: FF D0              call        rax
  000000006712F33D: 4D 85 E4           test        r12,r12
  000000006712F340: 74 4A              je          000000006712F38C
  000000006712F342: 41 F6 C4 01        test        r12b,1
  000000006712F346: 75 44              jne         000000006712F38C
  000000006712F348: 49 8B D4           mov         rdx,r12
  000000006712F34B: 48 83 E2 FE        and         rdx,0FFFFFFFFFFFFFFFEh
  000000006712F34F: 48 8B 0A           mov         rcx,qword ptr [rdx]
  000000006712F352: F6 41 04 01        test        byte ptr [rcx+4],1
  000000006712F356: 74 0A              je          000000006712F362
  000000006712F358: 48 8B CA           mov         rcx,rdx
  000000006712F35B: E8 50 6A EF FF     call        0000000067025DB0
  000000006712F360: EB 21              jmp         000000006712F383
  000000006712F362: 33 C0              xor         eax,eax
  000000006712F364: F0 0F B1 39        lock cmpxchg dword ptr [rcx],edi
  000000006712F368: 0F 94 C1           sete        cl
  000000006712F36B: 84 C9              test        cl,cl
  000000006712F36D: 75 0F              jne         000000006712F37E
  000000006712F36F: 48 8B CA           mov         rcx,rdx
  000000006712F372: E8 39 68 EF FF     call        0000000067025BB0
  000000006712F377: 4C 8B 64 24 30     mov         r12,qword ptr [rsp+30h]
  000000006712F37C: EB 05              jmp         000000006712F383
  000000006712F37E: 4C 8B 64 24 30     mov         r12,qword ptr [rsp+30h]
  000000006712F383: 49 83 CC 01        or          r12,1
  000000006712F387: 4C 89 64 24 30     mov         qword ptr [rsp+30h],r12
  000000006712F38C: 45 84 ED           test        r13b,r13b
  000000006712F38F: 74 1D              je          000000006712F3AE
  000000006712F391: 8B 44 24 74        mov         eax,dword ptr [rsp+74h]
  000000006712F395: 83 F8 01           cmp         eax,1
  000000006712F398: 75 0C              jne         000000006712F3A6
  000000006712F39A: 49 8B 47 08        mov         rax,qword ptr [r15+8]
  000000006712F39E: 48 89 70 58        mov         qword ptr [rax+58h],rsi
  000000006712F3A2: 8B 44 24 74        mov         eax,dword ptr [rsp+74h]
  000000006712F3A6: 48 85 F6           test        rsi,rsi
  000000006712F3A9: 74 03              je          000000006712F3AE
  000000006712F3AB: 89 46 0C           mov         dword ptr [rsi+0Ch],eax
  000000006712F3AE: 4C 8B 7C 24 40     mov         r15,qword ptr [rsp+40h]
  000000006712F3B3: 41 80 7F 08 00     cmp         byte ptr [r15+8],0
  000000006712F3B8: 75 33              jne         000000006712F3ED
  000000006712F3BA: BF 01 00 00 00     mov         edi,1
  000000006712F3BF: 4C 8B 94 24 D0 00  mov         r10,qword ptr [rsp+00000000000000D0h]
                    00 00
  000000006712F3C7: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  000000006712F3CC: EB 05              jmp         000000006712F3D3
  000000006712F3CE: 4C 8B 7C 24 40     mov         r15,qword ptr [rsp+40h]
  000000006712F3D3: 4C 3B F0           cmp         r14,rax
  000000006712F3D6: 74 15              je          000000006712F3ED
  000000006712F3D8: 4D 8B 76 18        mov         r14,qword ptr [r14+18h]
  000000006712F3DC: 4D 85 F6           test        r14,r14
  000000006712F3DF: 48 8B B4 24 E8 00  mov         rsi,qword ptr [rsp+00000000000000E8h]
                    00 00
  000000006712F3E7: 0F 85 E3 FA FF FF  jne         000000006712EED0
  000000006712F3ED: 41 80 7F 08 00     cmp         byte ptr [r15+8],0
  000000006712F3F2: 75 33              jne         000000006712F427
  000000006712F3F4: BF 01 00 00 00     mov         edi,1
  000000006712F3F9: 48 8B B4 24 E8 00  mov         rsi,qword ptr [rsp+00000000000000E8h]
                    00 00
  000000006712F401: 4C 8B 94 24 D0 00  mov         r10,qword ptr [rsp+00000000000000D0h]
                    00 00
  000000006712F409: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  000000006712F40E: 49 8D 47 10        lea         rax,[r15+10h]
  000000006712F412: 48 3B C8           cmp         rcx,rax
  000000006712F415: 74 10              je          000000006712F427
  000000006712F417: 48 8B C8           mov         rcx,rax
  000000006712F41A: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  000000006712F41F: 45 33 C0           xor         r8d,r8d
  000000006712F422: E9 89 FA FF FF     jmp         000000006712EEB0
  000000006712F427: 41 FF 4F 0C        dec         dword ptr [r15+0Ch]
  000000006712F42B: 41 80 7F 08 00     cmp         byte ptr [r15+8],0
  000000006712F430: 74 62              je          000000006712F494
  000000006712F432: 41 83 7F 0C 00     cmp         dword ptr [r15+0Ch],0
  000000006712F437: 75 75              jne         000000006712F4AE
  000000006712F439: 49 8B 0F           mov         rcx,qword ptr [r15]
  000000006712F43C: 48 85 C9           test        rcx,rcx
  000000006712F43F: 74 41              je          000000006712F482
  000000006712F441: F0 83 01 FF        lock add    dword ptr [rcx],0FFFFFFFFh
  000000006712F445: 75 36              jne         000000006712F47D
  000000006712F447: 49 8B 0F           mov         rcx,qword ptr [r15]
  000000006712F44A: 4C 8D 41 10        lea         r8,[rcx+10h]
  000000006712F44E: 48 63 51 08        movsxd      rdx,dword ptr [rcx+8]
  000000006712F452: 48 C1 E2 04        shl         rdx,4
  000000006712F456: 49 03 D0           add         rdx,r8
  000000006712F459: 0F 1F 80 00 00 00  nop         dword ptr [rax+0000000000000000h]
                    00
  000000006712F460: 48 8B C2           mov         rax,rdx
  000000006712F463: 48 83 EA 10        sub         rdx,10h
  000000006712F467: 49 3B C0           cmp         rax,r8
  000000006712F46A: 75 F4              jne         000000006712F460
  000000006712F46C: BA 08 00 00 00     mov         edx,8
  000000006712F471: E8 1A D1 F4 FF     call        000000006707C590
  000000006712F476: 4C 8B 64 24 30     mov         r12,qword ptr [rsp+30h]
  000000006712F47B: EB 05              jmp         000000006712F482
  000000006712F47D: 4C 8B 64 24 30     mov         r12,qword ptr [rsp+30h]
  000000006712F482: 49 8B CF           mov         rcx,r15
  000000006712F485: E8 CC 94 04 00     call        0000000067178956
  000000006712F48A: 48 8B 9C 24 D0 00  mov         rbx,qword ptr [rsp+00000000000000D0h]
                    00 00
  000000006712F492: EB 2C              jmp         000000006712F4C0
  000000006712F494: 41 80 7F 09 00     cmp         byte ptr [r15+9],0
  000000006712F499: 74 1D              je          000000006712F4B8
  000000006712F49B: 48 8B 9C 24 D0 00  mov         rbx,qword ptr [rsp+00000000000000D0h]
                    00 00
  000000006712F4A3: 48 8B 4B 08        mov         rcx,qword ptr [rbx+8]
  000000006712F4A7: E8 54 F0 FF FF     call        000000006712E500
  000000006712F4AC: EB 12              jmp         000000006712F4C0
  000000006712F4AE: 48 8B 9C 24 D0 00  mov         rbx,qword ptr [rsp+00000000000000D0h]
                    00 00
  000000006712F4B6: EB 08              jmp         000000006712F4C0
  000000006712F4B8: 48 8B 9C 24 D0 00  mov         rbx,qword ptr [rsp+00000000000000D0h]
                    00 00
  000000006712F4C0: 41 F6 C4 01        test        r12b,1
  000000006712F4C4: 74 42              je          000000006712F508
  000000006712F4C6: 49 83 E4 FE        and         r12,0FFFFFFFFFFFFFFFEh
  000000006712F4CA: 4C 89 64 24 30     mov         qword ptr [rsp+30h],r12
  000000006712F4CF: 49 8B CC           mov         rcx,r12
  000000006712F4D2: 48 83 E1 FE        and         rcx,0FFFFFFFFFFFFFFFEh
  000000006712F4D6: 48 8B 11           mov         rdx,qword ptr [rcx]
  000000006712F4D9: F6 42 04 01        test        byte ptr [rdx+4],1
  000000006712F4DD: 74 09              je          000000006712F4E8
  000000006712F4DF: E8 4C 66 EF FF     call        0000000067025B30
  000000006712F4E4: 33 FF              xor         edi,edi
  000000006712F4E6: EB 22              jmp         000000006712F50A
  000000006712F4E8: B8 01 00 00 00     mov         eax,1
  000000006712F4ED: 33 FF              xor         edi,edi
  000000006712F4EF: F0 0F B1 3A        lock cmpxchg dword ptr [rdx],edi
  000000006712F4F3: 74 0C              je          000000006712F501
  000000006712F4F5: E8 76 66 EF FF     call        0000000067025B70
  000000006712F4FA: 4C 8B 64 24 30     mov         r12,qword ptr [rsp+30h]
  000000006712F4FF: EB 09              jmp         000000006712F50A
  000000006712F501: 4C 8B 64 24 30     mov         r12,qword ptr [rsp+30h]
  000000006712F506: EB 02              jmp         000000006712F50A
  000000006712F508: 33 FF              xor         edi,edi
  000000006712F50A: 48 8B 05 FF 4C 1C  mov         rax,qword ptr [00000000672F4210h]
                    00
  000000006712F511: 48 85 C0           test        rax,rax
  000000006712F514: 74 0D              je          000000006712F523
  000000006712F516: 8B 94 24 D8 00 00  mov         edx,dword ptr [rsp+00000000000000D8h]
                    00
  000000006712F51D: 48 8B CB           mov         rcx,rbx
  000000006712F520: FF D0              call        rax
  000000006712F522: 90                 nop
  000000006712F523: 41 F6 C4 01        test        r12b,1
  000000006712F527: 74 34              je          000000006712F55D
  000000006712F529: 49 83 E4 FE        and         r12,0FFFFFFFFFFFFFFFEh
  000000006712F52D: 4C 89 64 24 30     mov         qword ptr [rsp+30h],r12
  000000006712F532: 49 83 E4 FE        and         r12,0FFFFFFFFFFFFFFFEh
  000000006712F536: 49 8B 0C 24        mov         rcx,qword ptr [r12]
  000000006712F53A: F6 41 04 01        test        byte ptr [rcx+4],1
  000000006712F53E: 74 0A              je          000000006712F54A
  000000006712F540: 49 8B CC           mov         rcx,r12
  000000006712F543: E8 E8 65 EF FF     call        0000000067025B30
  000000006712F548: EB 13              jmp         000000006712F55D
  000000006712F54A: B8 01 00 00 00     mov         eax,1
  000000006712F54F: F0 0F B1 39        lock cmpxchg dword ptr [rcx],edi
  000000006712F553: 74 08              je          000000006712F55D
  000000006712F555: 49 8B CC           mov         rcx,r12
  000000006712F558: E8 13 66 EF FF     call        0000000067025B70
  000000006712F55D: 48 81 C4 90 00 00  add         rsp,90h
                    00
  000000006712F564: 41 5F              pop         r15
  000000006712F566: 41 5E              pop         r14
  000000006712F568: 41 5D              pop         r13
  000000006712F56A: 41 5C              pop         r12
  000000006712F56C: 5F                 pop         rdi
  000000006712F56D: 5E                 pop         rsi
  000000006712F56E: 5B                 pop         rbx
  000000006712F56F: C3                 ret

  Summary

      1A7000 .text
