Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000650776C0: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  00000000650776C5: 48 89 6C 24 10     mov         qword ptr [rsp+10h],rbp
  00000000650776CA: 48 89 74 24 18     mov         qword ptr [rsp+18h],rsi
  00000000650776CF: 48 89 7C 24 20     mov         qword ptr [rsp+20h],rdi
  00000000650776D4: 41 54              push        r12
  00000000650776D6: 48 83 EC 20        sub         rsp,20h
  00000000650776DA: 48 8B F1           mov         rsi,rcx
  00000000650776DD: 48 8B 09           mov         rcx,qword ptr [rcx]
  00000000650776E0: 48 8B EA           mov         rbp,rdx
  00000000650776E3: BA 01 00 00 00     mov         edx,1
  00000000650776E8: 48 83 C1 10        add         rcx,10h
  00000000650776EC: 4D 8B E0           mov         r12,r8
  00000000650776EF: FF 15 7B F3 61 00  call        qword ptr [0000000065696A70h]
  00000000650776F5: 84 C0              test        al,al
  00000000650776F7: 74 08              je          0000000065077701
  00000000650776F9: 48 8B CE           mov         rcx,rsi
  00000000650776FC: E8 8F 46 1E 00     call        000000006525BD90
  0000000065077701: 4D 8B 0C 24        mov         r9,qword ptr [r12]
  0000000065077705: 4C 8B 06           mov         r8,qword ptr [rsi]
  0000000065077708: 41 8B 48 20        mov         ecx,dword ptr [r8+20h]
  000000006507770C: 49 8B F9           mov         rdi,r9
  000000006507770F: 48 C1 EF 1F        shr         rdi,1Fh
  0000000065077713: 41 33 F9           xor         edi,r9d
  0000000065077716: 85 C9              test        ecx,ecx
  0000000065077718: 74 2F              je          0000000065077749
  000000006507771A: 33 D2              xor         edx,edx
  000000006507771C: 8B C7              mov         eax,edi
  000000006507771E: F7 F1              div         eax,ecx
  0000000065077720: 49 8B 40 08        mov         rax,qword ptr [r8+8]
  0000000065077724: 48 8D 1C D0        lea         rbx,[rax+rdx*8]
  0000000065077728: 48 8B 04 D0        mov         rax,qword ptr [rax+rdx*8]
  000000006507772C: 49 3B C0           cmp         rax,r8
  000000006507772F: 74 1B              je          000000006507774C
  0000000065077731: 3B 78 08           cmp         edi,dword ptr [rax+8]
  0000000065077734: 75 06              jne         000000006507773C
  0000000065077736: 4C 3B 48 10        cmp         r9,qword ptr [rax+10h]
  000000006507773A: 74 10              je          000000006507774C
  000000006507773C: 48 8B D8           mov         rbx,rax
  000000006507773F: 48 8B 00           mov         rax,qword ptr [rax]
  0000000065077742: 49 3B C0           cmp         rax,r8
  0000000065077745: 75 EA              jne         0000000065077731
  0000000065077747: EB 03              jmp         000000006507774C
  0000000065077749: 48 8B DE           mov         rbx,rsi
  000000006507774C: 48 8B 03           mov         rax,qword ptr [rbx]
  000000006507774F: 49 3B C0           cmp         rax,r8
  0000000065077752: 0F 85 99 00 00 00  jne         00000000650777F1
  0000000065077758: 49 8B C8           mov         rcx,r8
  000000006507775B: FF 15 B7 F4 61 00  call        qword ptr [0000000065696C18h]
  0000000065077761: 84 C0              test        al,al
  0000000065077763: 74 50              je          00000000650777B5
  0000000065077765: 4D 8B 0C 24        mov         r9,qword ptr [r12]
  0000000065077769: 4C 8B 16           mov         r10,qword ptr [rsi]
  000000006507776C: 41 8B 4A 20        mov         ecx,dword ptr [r10+20h]
  0000000065077770: 4D 8B C1           mov         r8,r9
  0000000065077773: 49 C1 E8 1F        shr         r8,1Fh
  0000000065077777: 45 33 C1           xor         r8d,r9d
  000000006507777A: 85 C9              test        ecx,ecx
  000000006507777C: 74 31              je          00000000650777AF
  000000006507777E: 33 D2              xor         edx,edx
  0000000065077780: 41 8B C0           mov         eax,r8d
  0000000065077783: F7 F1              div         eax,ecx
  0000000065077785: 49 8B 42 08        mov         rax,qword ptr [r10+8]
  0000000065077789: 48 8D 1C D0        lea         rbx,[rax+rdx*8]
  000000006507778D: 48 8B 04 D0        mov         rax,qword ptr [rax+rdx*8]
  0000000065077791: 49 3B C2           cmp         rax,r10
  0000000065077794: 74 1C              je          00000000650777B2
  0000000065077796: 44 3B 40 08        cmp         r8d,dword ptr [rax+8]
  000000006507779A: 75 06              jne         00000000650777A2
  000000006507779C: 4C 3B 48 10        cmp         r9,qword ptr [rax+10h]
  00000000650777A0: 74 10              je          00000000650777B2
  00000000650777A2: 48 8B D8           mov         rbx,rax
  00000000650777A5: 48 8B 00           mov         rax,qword ptr [rax]
  00000000650777A8: 49 3B C2           cmp         rax,r10
  00000000650777AB: 75 E9              jne         0000000065077796
  00000000650777AD: EB 03              jmp         00000000650777B2
  00000000650777AF: 48 8B DE           mov         rbx,rsi
  00000000650777B2: 41 8B F8           mov         edi,r8d
  00000000650777B5: 48 8B 0E           mov         rcx,qword ptr [rsi]
  00000000650777B8: BA 08 00 00 00     mov         edx,8
  00000000650777BD: FF 15 15 F4 61 00  call        qword ptr [0000000065696BD8h]
  00000000650777C3: 4C 8B D8           mov         r11,rax
  00000000650777C6: 48 85 C0           test        rax,rax
  00000000650777C9: 74 0A              je          00000000650777D5
  00000000650777CB: 49 8B 0C 24        mov         rcx,qword ptr [r12]
  00000000650777CF: 48 89 48 10        mov         qword ptr [rax+10h],rcx
  00000000650777D3: EB 03              jmp         00000000650777D8
  00000000650777D5: 45 33 DB           xor         r11d,r11d
  00000000650777D8: 41 89 7B 08        mov         dword ptr [r11+8],edi
  00000000650777DC: 48 8B 03           mov         rax,qword ptr [rbx]
  00000000650777DF: 4C 89 5D 00        mov         qword ptr [rbp],r11
  00000000650777E3: 49 89 03           mov         qword ptr [r11],rax
  00000000650777E6: 4C 89 1B           mov         qword ptr [rbx],r11
  00000000650777E9: 48 8B 06           mov         rax,qword ptr [rsi]
  00000000650777EC: FF 40 14           inc         dword ptr [rax+14h]
  00000000650777EF: EB 04              jmp         00000000650777F5
  00000000650777F1: 48 89 45 00        mov         qword ptr [rbp],rax
  00000000650777F5: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  00000000650777FA: 48 8B 74 24 40     mov         rsi,qword ptr [rsp+40h]
  00000000650777FF: 48 8B 7C 24 48     mov         rdi,qword ptr [rsp+48h]
  0000000065077804: 48 8B C5           mov         rax,rbp
  0000000065077807: 48 8B 6C 24 38     mov         rbp,qword ptr [rsp+38h]
  000000006507780C: 48 83 C4 20        add         rsp,20h
  0000000065077810: 41 5C              pop         r12
  0000000065077812: C3                 ret

  Summary

      695000 .text
