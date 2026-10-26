Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067118470: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000067118475: 53                 push        rbx
  0000000067118476: 56                 push        rsi
  0000000067118477: 57                 push        rdi
  0000000067118478: 41 54              push        r12
  000000006711847A: 41 55              push        r13
  000000006711847C: 41 56              push        r14
  000000006711847E: 41 57              push        r15
  0000000067118480: 48 83 EC 40        sub         rsp,40h
  0000000067118484: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006711848D: 4C 8B F1           mov         r14,rcx
  0000000067118490: 48 8B 71 08        mov         rsi,qword ptr [rcx+8]
  0000000067118494: 48 89 74 24 28     mov         qword ptr [rsp+28h],rsi
  0000000067118499: 48 8B 46 40        mov         rax,qword ptr [rsi+40h]
  000000006711849D: 4C 8B 40 08        mov         r8,qword ptr [rax+8]
  00000000671184A1: 49 8B 58 08        mov         rbx,qword ptr [r8+8]
  00000000671184A5: 48 81 C3 88 00 00  add         rbx,88h
                    00
  00000000671184AC: 74 44              je          00000000671184F2
  00000000671184AE: 4C 8B 03           mov         r8,qword ptr [rbx]
  00000000671184B1: 41 F6 40 04 01     test        byte ptr [r8+4],1
  00000000671184B6: 74 10              je          00000000671184C8
  00000000671184B8: 48 8B CB           mov         rcx,rbx
  00000000671184BB: E8 F0 D8 F0 FF     call        0000000067025DB0
  00000000671184C0: 41 BD 01 00 00 00  mov         r13d,1
  00000000671184C6: EB 1C              jmp         00000000671184E4
  00000000671184C8: 33 C0              xor         eax,eax
  00000000671184CA: 41 BD 01 00 00 00  mov         r13d,1
  00000000671184D0: F0 45 0F B1 28     lock cmpxchg dword ptr [r8],r13d
  00000000671184D5: 0F 94 C1           sete        cl
  00000000671184D8: 84 C9              test        cl,cl
  00000000671184DA: 75 08              jne         00000000671184E4
  00000000671184DC: 48 8B CB           mov         rcx,rbx
  00000000671184DF: E8 CC D6 F0 FF     call        0000000067025BB0
  00000000671184E4: 48 83 CB 01        or          rbx,1
  00000000671184E8: 48 89 5C 24 20     mov         qword ptr [rsp+20h],rbx
  00000000671184ED: 45 33 FF           xor         r15d,r15d
  00000000671184F0: EB 0F              jmp         0000000067118501
  00000000671184F2: 45 33 FF           xor         r15d,r15d
  00000000671184F5: 49 8B DF           mov         rbx,r15
  00000000671184F8: 48 89 5C 24 20     mov         qword ptr [rsp+20h],rbx
  00000000671184FD: 45 8D 6F 01        lea         r13d,[r15+1]
  0000000067118501: 48 8B 46 40        mov         rax,qword ptr [rsi+40h]
  0000000067118505: 80 78 18 00        cmp         byte ptr [rax+18h],0
  0000000067118509: 74 3F              je          000000006711854A
  000000006711850B: F6 C3 01           test        bl,1
  000000006711850E: 74 32              je          0000000067118542
  0000000067118510: 48 83 E3 FE        and         rbx,0FFFFFFFFFFFFFFFEh
  0000000067118514: 48 89 5C 24 20     mov         qword ptr [rsp+20h],rbx
  0000000067118519: 48 83 E3 FE        and         rbx,0FFFFFFFFFFFFFFFEh
  000000006711851D: 48 8B 0B           mov         rcx,qword ptr [rbx]
  0000000067118520: F6 41 04 01        test        byte ptr [rcx+4],1
  0000000067118524: 74 0A              je          0000000067118530
  0000000067118526: 48 8B CB           mov         rcx,rbx
  0000000067118529: E8 02 D6 F0 FF     call        0000000067025B30
  000000006711852E: EB 12              jmp         0000000067118542
  0000000067118530: 41 8B C5           mov         eax,r13d
  0000000067118533: F0 44 0F B1 39     lock cmpxchg dword ptr [rcx],r15d
  0000000067118538: 74 08              je          0000000067118542
  000000006711853A: 48 8B CB           mov         rcx,rbx
  000000006711853D: E8 2E D6 F0 FF     call        0000000067025B70
  0000000067118542: 83 C8 FF           or          eax,0FFFFFFFFh
  0000000067118545: E9 52 02 00 00     jmp         000000006711879C
  000000006711854A: 80 BE 89 00 00 00  cmp         byte ptr [rsi+0000000000000089h],0
                    00
  0000000067118551: 74 4F              je          00000000671185A2
  0000000067118553: 49 8B D6           mov         rdx,r14
  0000000067118556: 48 8D 0D 7B 12 12  lea         rcx,[00000000672397D8h]
                    00
  000000006711855D: E8 7E 49 F0 FF     call        000000006701CEE0
  0000000067118562: 90                 nop
  0000000067118563: F6 C3 01           test        bl,1
  0000000067118566: 74 32              je          000000006711859A
  0000000067118568: 48 83 E3 FE        and         rbx,0FFFFFFFFFFFFFFFEh
  000000006711856C: 48 89 5C 24 20     mov         qword ptr [rsp+20h],rbx
  0000000067118571: 48 83 E3 FE        and         rbx,0FFFFFFFFFFFFFFFEh
  0000000067118575: 48 8B 0B           mov         rcx,qword ptr [rbx]
  0000000067118578: F6 41 04 01        test        byte ptr [rcx+4],1
  000000006711857C: 74 0A              je          0000000067118588
  000000006711857E: 48 8B CB           mov         rcx,rbx
  0000000067118581: E8 AA D5 F0 FF     call        0000000067025B30
  0000000067118586: EB 12              jmp         000000006711859A
  0000000067118588: 41 8B C5           mov         eax,r13d
  000000006711858B: F0 44 0F B1 39     lock cmpxchg dword ptr [rcx],r15d
  0000000067118590: 74 08              je          000000006711859A
  0000000067118592: 48 8B CB           mov         rcx,rbx
  0000000067118595: E8 D6 D5 F0 FF     call        0000000067025B70
  000000006711859A: 83 C8 FF           or          eax,0FFFFFFFFh
  000000006711859D: E9 FA 01 00 00     jmp         000000006711879C
  00000000671185A2: C6 86 89 00 00 00  mov         byte ptr [rsi+0000000000000089h],1
                    01
  00000000671185A9: C6 86 88 00 00 00  mov         byte ptr [rsi+0000000000000088h],0
                    00
  00000000671185B0: FF 40 1C           inc         dword ptr [rax+1Ch]
  00000000671185B3: 48 8B 5E 40        mov         rbx,qword ptr [rsi+40h]
  00000000671185B7: 48 8B 43 28        mov         rax,qword ptr [rbx+28h]
  00000000671185BB: 8B 08              mov         ecx,dword ptr [rax]
  00000000671185BD: 83 F9 01           cmp         ecx,1
  00000000671185C0: 75 17              jne         00000000671185D9
  00000000671185C2: 48 8B 4B 28        mov         rcx,qword ptr [rbx+28h]
  00000000671185C6: 48 63 51 08        movsxd      rdx,dword ptr [rcx+8]
  00000000671185CA: 8D 42 01           lea         eax,[rdx+1]
  00000000671185CD: 3B 41 04           cmp         eax,dword ptr [rcx+4]
  00000000671185D0: 7F 07              jg          00000000671185D9
  00000000671185D2: 4C 89 74 D1 10     mov         qword ptr [rcx+rdx*8+10h],r14
  00000000671185D7: EB 39              jmp         0000000067118612
  00000000671185D9: 48 8B 43 28        mov         rax,qword ptr [rbx+28h]
  00000000671185DD: 8B 50 08           mov         edx,dword ptr [rax+8]
  00000000671185E0: FF C2              inc         edx
  00000000671185E2: 45 33 C9           xor         r9d,r9d
  00000000671185E5: 41 8D 49 18        lea         ecx,[r9+18h]
  00000000671185E9: 45 8D 41 08        lea         r8d,[r9+8]
  00000000671185ED: E8 AE 3F F6 FF     call        000000006707C5A0
  00000000671185F2: 44 8B C0           mov         r8d,eax
  00000000671185F5: 48 8B 43 28        mov         rax,qword ptr [rbx+28h]
  00000000671185F9: 8B 50 08           mov         edx,dword ptr [rax+8]
  00000000671185FC: 48 8D 4B 28        lea         rcx,[rbx+28h]
  0000000067118600: E8 8B F9 FF FF     call        0000000067117F90
  0000000067118605: 4C 8B 5B 28        mov         r11,qword ptr [rbx+28h]
  0000000067118609: 49 63 43 08        movsxd      rax,dword ptr [r11+8]
  000000006711860D: 4D 89 74 C3 10     mov         qword ptr [r11+rax*8+10h],r14
  0000000067118612: 48 8B 43 28        mov         rax,qword ptr [rbx+28h]
  0000000067118616: FF 40 08           inc         dword ptr [rax+8]
  0000000067118619: 48 8B 7C 24 20     mov         rdi,qword ptr [rsp+20h]
  000000006711861E: 40 F6 C7 01        test        dil,1
  0000000067118622: 74 3B              je          000000006711865F
  0000000067118624: 48 83 E7 FE        and         rdi,0FFFFFFFFFFFFFFFEh
  0000000067118628: 48 89 7C 24 20     mov         qword ptr [rsp+20h],rdi
  000000006711862D: 48 8B CF           mov         rcx,rdi
  0000000067118630: 48 83 E1 FE        and         rcx,0FFFFFFFFFFFFFFFEh
  0000000067118634: 48 8B 11           mov         rdx,qword ptr [rcx]
  0000000067118637: F6 42 04 01        test        byte ptr [rdx+4],1
  000000006711863B: 74 07              je          0000000067118644
  000000006711863D: E8 EE D4 F0 FF     call        0000000067025B30
  0000000067118642: EB 1B              jmp         000000006711865F
  0000000067118644: 41 8B C5           mov         eax,r13d
  0000000067118647: F0 44 0F B1 3A     lock cmpxchg dword ptr [rdx],r15d
  000000006711864C: 74 0C              je          000000006711865A
  000000006711864E: E8 1D D5 F0 FF     call        0000000067025B70
  0000000067118653: 48 8B 7C 24 20     mov         rdi,qword ptr [rsp+20h]
  0000000067118658: EB 05              jmp         000000006711865F
  000000006711865A: 48 8B 7C 24 20     mov         rdi,qword ptr [rsp+20h]
  000000006711865F: 4C 8B 25 BA BB 1D  mov         r12,qword ptr [00000000672F4220h]
                    00
  0000000067118666: 4D 85 E4           test        r12,r12
  0000000067118669: 74 26              je          0000000067118691
  000000006711866B: 49 8B CC           mov         rcx,r12
  000000006711866E: E8 9D 2A 01 00     call        000000006712B110
  0000000067118673: 48 8B D8           mov         rbx,rax
  0000000067118676: 49 8B CE           mov         rcx,r14
  0000000067118679: E8 92 2A 01 00     call        000000006712B110
  000000006711867E: 48 3B D8           cmp         rbx,rax
  0000000067118681: 75 0E              jne         0000000067118691
  0000000067118683: BA 14 00 00 00     mov         edx,14h
  0000000067118688: 49 8B CC           mov         rcx,r12
  000000006711868B: E8 70 2E 00 00     call        000000006711B500
  0000000067118690: 90                 nop
  0000000067118691: 80 BE 88 00 00 00  cmp         byte ptr [rsi+0000000000000088h],0
                    00
  0000000067118698: 75 57              jne         00000000671186F1
  000000006711869A: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  00000000671186A2: 8B 18              mov         ebx,dword ptr [rax]
  00000000671186A4: 83 CB 24           or          ebx,24h
  00000000671186A7: 89 9C 24 90 00 00  mov         dword ptr [rsp+0000000000000090h],ebx
                    00
  00000000671186AE: 4D 8B 66 08        mov         r12,qword ptr [r14+8]
  00000000671186B2: 49 8B 44 24 40     mov         rax,qword ptr [r12+40h]
  00000000671186B7: 48 83 78 20 00     cmp         qword ptr [rax+20h],0
  00000000671186BC: 74 D3              je          0000000067118691
  00000000671186BE: 8B C3              mov         eax,ebx
  00000000671186C0: 83 E0 10           and         eax,10h
  00000000671186C3: 74 0C              je          00000000671186D1
  00000000671186C5: BA 34 00 00 00     mov         edx,34h
  00000000671186CA: 33 C9              xor         ecx,ecx
  00000000671186CC: E8 0F 39 00 00     call        000000006711BFE0
  00000000671186D1: 89 9C 24 80 00 00  mov         dword ptr [rsp+0000000000000080h],ebx
                    00
  00000000671186D8: 49 8B 44 24 40     mov         rax,qword ptr [r12+40h]
  00000000671186DD: 48 8B 48 20        mov         rcx,qword ptr [rax+20h]
  00000000671186E1: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000671186E4: 48 8D 94 24 80 00  lea         rdx,[rsp+0000000000000080h]
                    00 00
  00000000671186EC: FF 50 58           call        qword ptr [rax+58h]
  00000000671186EF: EB A0              jmp         0000000067118691
  00000000671186F1: 48 85 FF           test        rdi,rdi
  00000000671186F4: 74 4B              je          0000000067118741
  00000000671186F6: 40 F6 C7 01        test        dil,1
  00000000671186FA: 75 45              jne         0000000067118741
  00000000671186FC: 48 8B D7           mov         rdx,rdi
  00000000671186FF: 48 83 E2 FE        and         rdx,0FFFFFFFFFFFFFFFEh
  0000000067118703: 48 8B 0A           mov         rcx,qword ptr [rdx]
  0000000067118706: F6 41 04 01        test        byte ptr [rcx+4],1
  000000006711870A: 74 0A              je          0000000067118716
  000000006711870C: 48 8B CA           mov         rcx,rdx
  000000006711870F: E8 9C D6 F0 FF     call        0000000067025DB0
  0000000067118714: EB 22              jmp         0000000067118738
  0000000067118716: 33 C0              xor         eax,eax
  0000000067118718: F0 44 0F B1 29     lock cmpxchg dword ptr [rcx],r13d
  000000006711871D: 0F 94 C1           sete        cl
  0000000067118720: 84 C9              test        cl,cl
  0000000067118722: 75 0F              jne         0000000067118733
  0000000067118724: 48 8B CA           mov         rcx,rdx
  0000000067118727: E8 84 D4 F0 FF     call        0000000067025BB0
  000000006711872C: 48 8B 7C 24 20     mov         rdi,qword ptr [rsp+20h]
  0000000067118731: EB 05              jmp         0000000067118738
  0000000067118733: 48 8B 7C 24 20     mov         rdi,qword ptr [rsp+20h]
  0000000067118738: 48 83 CF 01        or          rdi,1
  000000006711873C: 48 89 7C 24 20     mov         qword ptr [rsp+20h],rdi
  0000000067118741: 48 8B 4E 40        mov         rcx,qword ptr [rsi+40h]
  0000000067118745: 48 83 C1 28        add         rcx,28h
  0000000067118749: E8 92 FC FF FF     call        00000000671183E0
  000000006711874E: C6 86 89 00 00 00  mov         byte ptr [rsi+0000000000000089h],0
                    00
  0000000067118755: 48 8B 46 40        mov         rax,qword ptr [rsi+40h]
  0000000067118759: FF 48 1C           dec         dword ptr [rax+1Ch]
  000000006711875C: 8B 9E 8C 00 00 00  mov         ebx,dword ptr [rsi+000000000000008Ch]
  0000000067118762: 40 F6 C7 01        test        dil,1
  0000000067118766: 74 32              je          000000006711879A
  0000000067118768: 48 83 E7 FE        and         rdi,0FFFFFFFFFFFFFFFEh
  000000006711876C: 48 89 7C 24 20     mov         qword ptr [rsp+20h],rdi
  0000000067118771: 48 83 E7 FE        and         rdi,0FFFFFFFFFFFFFFFEh
  0000000067118775: 48 8B 0F           mov         rcx,qword ptr [rdi]
  0000000067118778: F6 41 04 01        test        byte ptr [rcx+4],1
  000000006711877C: 74 0A              je          0000000067118788
  000000006711877E: 48 8B CF           mov         rcx,rdi
  0000000067118781: E8 AA D3 F0 FF     call        0000000067025B30
  0000000067118786: EB 12              jmp         000000006711879A
  0000000067118788: 41 8B C5           mov         eax,r13d
  000000006711878B: F0 44 0F B1 39     lock cmpxchg dword ptr [rcx],r15d
  0000000067118790: 74 08              je          000000006711879A
  0000000067118792: 48 8B CF           mov         rcx,rdi
  0000000067118795: E8 D6 D3 F0 FF     call        0000000067025B70
  000000006711879A: 8B C3              mov         eax,ebx
  000000006711879C: 48 83 C4 40        add         rsp,40h
  00000000671187A0: 41 5F              pop         r15
  00000000671187A2: 41 5E              pop         r14
  00000000671187A4: 41 5D              pop         r13
  00000000671187A6: 41 5C              pop         r12
  00000000671187A8: 5F                 pop         rdi
  00000000671187A9: 5E                 pop         rsi
  00000000671187AA: 5B                 pop         rbx
  00000000671187AB: C3                 ret

  Summary

      1A7000 .text
