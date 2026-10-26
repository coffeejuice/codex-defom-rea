Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006539CD40: 48 8B C4           mov         rax,rsp
  000000006539CD43: 89 50 10           mov         dword ptr [rax+10h],edx
  000000006539CD46: 55                 push        rbp
  000000006539CD47: 56                 push        rsi
  000000006539CD48: 57                 push        rdi
  000000006539CD49: 41 54              push        r12
  000000006539CD4B: 41 55              push        r13
  000000006539CD4D: 41 56              push        r14
  000000006539CD4F: 41 57              push        r15
  000000006539CD51: 48 83 EC 40        sub         rsp,40h
  000000006539CD55: 48 C7 44 24 38 FE  mov         qword ptr [rsp+38h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006539CD5E: 48 89 58 18        mov         qword ptr [rax+18h],rbx
  000000006539CD62: 45 0F B6 F8        movzx       r15d,r8b
  000000006539CD66: 8B DA              mov         ebx,edx
  000000006539CD68: 4C 8B E1           mov         r12,rcx
  000000006539CD6B: 45 33 ED           xor         r13d,r13d
  000000006539CD6E: 44 89 68 08        mov         dword ptr [rax+8],r13d
  000000006539CD72: 4C 8B 71 08        mov         r14,qword ptr [rcx+8]
  000000006539CD76: 33 FF              xor         edi,edi
  000000006539CD78: 81 FA 00 00 20 00  cmp         edx,200000h
  000000006539CD7E: 7F 73              jg          000000006539CDF3
  000000006539CD80: 81 FA 00 00 20 00  cmp         edx,200000h
  000000006539CD86: 74 64              je          000000006539CDEC
  000000006539CD88: 81 FA 00 20 00 00  cmp         edx,2000h
  000000006539CD8E: 7F 3E              jg          000000006539CDCE
  000000006539CD90: 81 FA 00 20 00 00  cmp         edx,2000h
  000000006539CD96: 74 2F              je          000000006539CDC7
  000000006539CD98: 85 D2              test        edx,edx
  000000006539CD9A: 74 24              je          000000006539CDC0
  000000006539CD9C: 81 FA 00 04 00 00  cmp         edx,400h
  000000006539CDA2: 74 15              je          000000006539CDB9
  000000006539CDA4: 81 FA 00 08 00 00  cmp         edx,800h
  000000006539CDAA: 0F 85 8C 00 00 00  jne         000000006539CE3C
  000000006539CDB0: 41 8D 7D 2A        lea         edi,[r13+2Ah]
  000000006539CDB4: E9 83 00 00 00     jmp         000000006539CE3C
  000000006539CDB9: BF 26 00 00 00     mov         edi,26h
  000000006539CDBE: EB 7C              jmp         000000006539CE3C
  000000006539CDC0: 33 C0              xor         eax,eax
  000000006539CDC2: E9 B3 01 00 00     jmp         000000006539CF7A
  000000006539CDC7: BF 29 00 00 00     mov         edi,29h
  000000006539CDCC: EB 6E              jmp         000000006539CE3C
  000000006539CDCE: 81 FA 00 40 00 00  cmp         edx,4000h
  000000006539CDD4: 74 0F              je          000000006539CDE5
  000000006539CDD6: 81 FA 00 00 01 00  cmp         edx,10000h
  000000006539CDDC: 75 5E              jne         000000006539CE3C
  000000006539CDDE: BF 30 00 00 00     mov         edi,30h
  000000006539CDE3: EB 57              jmp         000000006539CE3C
  000000006539CDE5: BF 2F 00 00 00     mov         edi,2Fh
  000000006539CDEA: EB 50              jmp         000000006539CE3C
  000000006539CDEC: BF 2B 00 00 00     mov         edi,2Bh
  000000006539CDF1: EB 49              jmp         000000006539CE3C
  000000006539CDF3: 81 FA 00 00 40 00  cmp         edx,400000h
  000000006539CDF9: 74 3C              je          000000006539CE37
  000000006539CDFB: 81 FA 00 00 80 00  cmp         edx,800000h
  000000006539CE01: 74 2D              je          000000006539CE30
  000000006539CE03: 81 FA 00 00 00 01  cmp         edx,1000000h
  000000006539CE09: 74 1E              je          000000006539CE29
  000000006539CE0B: 81 FA 00 00 00 02  cmp         edx,2000000h
  000000006539CE11: 74 0F              je          000000006539CE22
  000000006539CE13: 81 FA 00 00 00 04  cmp         edx,4000000h
  000000006539CE19: 75 21              jne         000000006539CE3C
  000000006539CE1B: BF 2D 00 00 00     mov         edi,2Dh
  000000006539CE20: EB 1A              jmp         000000006539CE3C
  000000006539CE22: BF 2C 00 00 00     mov         edi,2Ch
  000000006539CE27: EB 13              jmp         000000006539CE3C
  000000006539CE29: BF 28 00 00 00     mov         edi,28h
  000000006539CE2E: EB 0C              jmp         000000006539CE3C
  000000006539CE30: BF 2E 00 00 00     mov         edi,2Eh
  000000006539CE35: EB 05              jmp         000000006539CE3C
  000000006539CE37: BF 27 00 00 00     mov         edi,27h
  000000006539CE3C: E8 EF DF FF FF     call        000000006539AE30
  000000006539CE41: 48 8B E8           mov         rbp,rax
  000000006539CE44: B9 28 00 00 00     mov         ecx,28h
  000000006539CE49: E8 24 43 24 00     call        00000000655E1172
  000000006539CE4E: 48 8B F0           mov         rsi,rax
  000000006539CE51: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000006539CE56: 48 85 C0           test        rax,rax
  000000006539CE59: 74 35              je          000000006539CE90
  000000006539CE5B: 45 33 C0           xor         r8d,r8d
  000000006539CE5E: 48 8B D5           mov         rdx,rbp
  000000006539CE61: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  000000006539CE69: E8 82 1C F1 FF     call        00000000652AEAF0
  000000006539CE6E: 90                 nop
  000000006539CE6F: 41 BD 01 00 00 00  mov         r13d,1
  000000006539CE75: 44 89 AC 24 80 00  mov         dword ptr [rsp+0000000000000080h],r13d
                    00 00
  000000006539CE7D: 4D 8B C6           mov         r8,r14
  000000006539CE80: 48 8B D0           mov         rdx,rax
  000000006539CE83: 48 8B CE           mov         rcx,rsi
  000000006539CE86: E8 E5 B3 05 00     call        00000000653F8270
  000000006539CE8B: 48 8B F0           mov         rsi,rax
  000000006539CE8E: EB 02              jmp         000000006539CE92
  000000006539CE90: 33 F6              xor         esi,esi
  000000006539CE92: 48 89 74 24 30     mov         qword ptr [rsp+30h],rsi
  000000006539CE97: 41 F6 C5 01        test        r13b,1
  000000006539CE9B: 74 0E              je          000000006539CEAB
  000000006539CE9D: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  000000006539CEA5: FF 15 B5 98 2F 00  call        qword ptr [0000000065696760h]
  000000006539CEAB: 49 8B CE           mov         rcx,r14
  000000006539CEAE: E8 AD 07 CC FF     call        000000006505D660
  000000006539CEB3: 48 8B E8           mov         rbp,rax
  000000006539CEB6: 4C 8B 10           mov         r10,qword ptr [rax]
  000000006539CEB9: 48 C7 44 24 20 00  mov         qword ptr [rsp+20h],0
                    00 00 00
  000000006539CEC2: 4D 8B CE           mov         r9,r14
  000000006539CEC5: 45 33 C0           xor         r8d,r8d
  000000006539CEC8: 41 8D 50 48        lea         edx,[r8+48h]
  000000006539CECC: 48 8B C8           mov         rcx,rax
  000000006539CECF: 41 FF 92 E8 00 00  call        qword ptr [r10+00000000000000E8h]
                    00
  000000006539CED6: 85 C0              test        eax,eax
  000000006539CED8: 74 39              je          000000006539CF13
  000000006539CEDA: 85 FF              test        edi,edi
  000000006539CEDC: 74 35              je          000000006539CF13
  000000006539CEDE: 4C 89 74 24 20     mov         qword ptr [rsp+20h],r14
  000000006539CEE3: 45 33 C9           xor         r9d,r9d
  000000006539CEE6: 44 8B C7           mov         r8d,edi
  000000006539CEE9: 48 8D 94 24 80 00  lea         rdx,[rsp+0000000000000080h]
                    00 00
  000000006539CEF1: 48 8B CD           mov         rcx,rbp
  000000006539CEF4: E8 B7 A8 F0 FF     call        00000000652A77B0
  000000006539CEF9: 90                 nop
  000000006539CEFA: 48 8B D0           mov         rdx,rax
  000000006539CEFD: 48 8B CE           mov         rcx,rsi
  000000006539CF00: E8 DB 41 FD FF     call        00000000653710E0
  000000006539CF05: 90                 nop
  000000006539CF06: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  000000006539CF0E: E8 1D 45 D0 FF     call        00000000650A1430
  000000006539CF13: E8 E8 91 C8 FF     call        0000000065026100
  000000006539CF18: 48 3B E8           cmp         rbp,rax
  000000006539CF1B: 74 0B              je          000000006539CF28
  000000006539CF1D: 48 8B D5           mov         rdx,rbp
  000000006539CF20: 48 8B CE           mov         rcx,rsi
  000000006539CF23: E8 68 CE CC FF     call        0000000065069D90
  000000006539CF28: 49 8D 8C 24 28 02  lea         rcx,[r12+0000000000000228h]
                    00 00
  000000006539CF30: 4C 8D 8C 24 88 00  lea         r9,[rsp+0000000000000088h]
                    00 00
  000000006539CF38: 4C 8D 44 24 30     lea         r8,[rsp+30h]
  000000006539CF3D: 48 8D 94 24 80 00  lea         rdx,[rsp+0000000000000080h]
                    00 00
  000000006539CF45: E8 56 EC FF FF     call        000000006539BBA0
  000000006539CF4A: 8B CB              mov         ecx,ebx
  000000006539CF4C: E8 0F DE FF FF     call        000000006539AD60
  000000006539CF51: 83 F8 FF           cmp         eax,0FFFFFFFFh
  000000006539CF54: 74 14              je          000000006539CF6A
  000000006539CF56: 45 0F B6 CF        movzx       r9d,r15b
  000000006539CF5A: 44 8B C0           mov         r8d,eax
  000000006539CF5D: 48 8B D6           mov         rdx,rsi
  000000006539CF60: 49 8B CC           mov         rcx,r12
  000000006539CF63: E8 28 FC FF FF     call        000000006539CB90
  000000006539CF68: EB 0D              jmp         000000006539CF77
  000000006539CF6A: 48 8D 0D 5F 01 36  lea         rcx,[00000000656FD0D0h]
                    00
  000000006539CF71: FF 15 21 9A 2F 00  call        qword ptr [0000000065696998h]
  000000006539CF77: 48 8B C6           mov         rax,rsi
  000000006539CF7A: 48 8B 9C 24 90 00  mov         rbx,qword ptr [rsp+0000000000000090h]
                    00 00
  000000006539CF82: 48 83 C4 40        add         rsp,40h
  000000006539CF86: 41 5F              pop         r15
  000000006539CF88: 41 5E              pop         r14
  000000006539CF8A: 41 5D              pop         r13
  000000006539CF8C: 41 5C              pop         r12
  000000006539CF8E: 5F                 pop         rdi
  000000006539CF8F: 5E                 pop         rsi
  000000006539CF90: 5D                 pop         rbp
  000000006539CF91: C3                 ret

  Summary

      695000 .text
