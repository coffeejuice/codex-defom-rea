Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670BB750: 48 89 5C 24 20     mov         qword ptr [rsp+20h],rbx
  00000000670BB755: 57                 push        rdi
  00000000670BB756: 41 54              push        r12
  00000000670BB758: 41 56              push        r14
  00000000670BB75A: 48 83 EC 20        sub         rsp,20h
  00000000670BB75E: 48 8B 59 08        mov         rbx,qword ptr [rcx+8]
  00000000670BB762: 49 8B F8           mov         rdi,r8
  00000000670BB765: 4C 8B E2           mov         r12,rdx
  00000000670BB768: 8B 83 88 00 00 00  mov         eax,dword ptr [rbx+0000000000000088h]
  00000000670BB76E: 4C 8B F1           mov         r14,rcx
  00000000670BB771: A8 02              test        al,2
  00000000670BB773: 75 23              jne         00000000670BB798
  00000000670BB775: 85 C0              test        eax,eax
  00000000670BB777: 74 0C              je          00000000670BB785
  00000000670BB779: 48 8D 0D 48 F7 16  lea         rcx,[000000006722AEC8h]
                    00
  00000000670BB780: E8 5B 17 F6 FF     call        000000006701CEE0
  00000000670BB785: 48 83 C8 FF        or          rax,0FFFFFFFFFFFFFFFFh
  00000000670BB789: 48 8B 5C 24 58     mov         rbx,qword ptr [rsp+58h]
  00000000670BB78E: 48 83 C4 20        add         rsp,20h
  00000000670BB792: 41 5E              pop         r14
  00000000670BB794: 41 5C              pop         r12
  00000000670BB796: 5F                 pop         rdi
  00000000670BB797: C3                 ret
  00000000670BB798: 4D 85 C0           test        r8,r8
  00000000670BB79B: 79 1F              jns         00000000670BB7BC
  00000000670BB79D: 48 8D 0D F4 F6 16  lea         rcx,[000000006722AE98h]
                    00
  00000000670BB7A4: E8 37 17 F6 FF     call        000000006701CEE0
  00000000670BB7A9: 48 83 C8 FF        or          rax,0FFFFFFFFFFFFFFFFh
  00000000670BB7AD: 48 8B 5C 24 58     mov         rbx,qword ptr [rsp+58h]
  00000000670BB7B2: 48 83 C4 20        add         rsp,20h
  00000000670BB7B6: 41 5E              pop         r14
  00000000670BB7B8: 41 5C              pop         r12
  00000000670BB7BA: 5F                 pop         rdi
  00000000670BB7BB: C3                 ret
  00000000670BB7BC: 83 BB E4 00 00 00  cmp         dword ptr [rbx+00000000000000E4h],0
                    00
  00000000670BB7C3: 4C 89 6C 24 50     mov         qword ptr [rsp+50h],r13
  00000000670BB7C8: 75 17              jne         00000000670BB7E1
  00000000670BB7CA: 48 8B 4B 08        mov         rcx,qword ptr [rbx+8]
  00000000670BB7CE: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000670BB7D1: FF 50 58           call        qword ptr [rax+58h]
  00000000670BB7D4: F6 D8              neg         al
  00000000670BB7D6: 1B C9              sbb         ecx,ecx
  00000000670BB7D8: 83 C1 02           add         ecx,2
  00000000670BB7DB: 89 8B E4 00 00 00  mov         dword ptr [rbx+00000000000000E4h],ecx
  00000000670BB7E1: 83 BB E4 00 00 00  cmp         dword ptr [rbx+00000000000000E4h],1
                    01
  00000000670BB7E8: 48 8B 93 B8 00 00  mov         rdx,qword ptr [rbx+00000000000000B8h]
                    00
  00000000670BB7EF: 41 0F 94 C5        sete        r13b
  00000000670BB7F3: 48 3B 93 C0 00 00  cmp         rdx,qword ptr [rbx+00000000000000C0h]
                    00
  00000000670BB7FA: 74 1E              je          00000000670BB81A
  00000000670BB7FC: 45 84 ED           test        r13b,r13b
  00000000670BB7FF: 75 19              jne         00000000670BB81A
  00000000670BB801: 49 8B 06           mov         rax,qword ptr [r14]
  00000000670BB804: 49 8B CE           mov         rcx,r14
  00000000670BB807: FF 90 80 00 00 00  call        qword ptr [rax+0000000000000080h]
  00000000670BB80D: 84 C0              test        al,al
  00000000670BB80F: 75 09              jne         00000000670BB81A
  00000000670BB811: 48 83 C8 FF        or          rax,0FFFFFFFFFFFFFFFFh
  00000000670BB815: E9 36 01 00 00     jmp         00000000670BB950
  00000000670BB81A: F6 83 88 00 00 00  test        byte ptr [rbx+0000000000000088h],10h
                    10
  00000000670BB821: 0F 84 85 01 00 00  je          00000000670BB9AC
  00000000670BB827: 48 89 6C 24 40     mov         qword ptr [rsp+40h],rbp
  00000000670BB82C: 48 89 74 24 48     mov         qword ptr [rsp+48h],rsi
  00000000670BB831: 33 F6              xor         esi,esi
  00000000670BB833: 49 8D 2C 3C        lea         rbp,[r12+rdi]
  00000000670BB837: 49 8B FC           mov         rdi,r12
  00000000670BB83A: 4C 3B E5           cmp         r12,rbp
  00000000670BB83D: 73 0E              jae         00000000670BB84D
  00000000670BB83F: 90                 nop
  00000000670BB840: 80 3F 0A           cmp         byte ptr [rdi],0Ah
  00000000670BB843: 74 08              je          00000000670BB84D
  00000000670BB845: 48 FF C7           inc         rdi
  00000000670BB848: 48 3B FD           cmp         rdi,rbp
  00000000670BB84B: 72 F3              jb          00000000670BB840
  00000000670BB84D: 4C 8B C7           mov         r8,rdi
  00000000670BB850: 4D 2B C4           sub         r8,r12
  00000000670BB853: 4D 85 C0           test        r8,r8
  00000000670BB856: 7E 2D              jle         00000000670BB885
  00000000670BB858: 49 8B 06           mov         rax,qword ptr [r14]
  00000000670BB85B: 49 8B D4           mov         rdx,r12
  00000000670BB85E: 49 8B CE           mov         rcx,r14
  00000000670BB861: FF 90 D0 00 00 00  call        qword ptr [rax+00000000000000D0h]
  00000000670BB867: 4C 8B D8           mov         r11,rax
  00000000670BB86A: 48 85 C0           test        rax,rax
  00000000670BB86D: 7E 5F              jle         00000000670BB8CE
  00000000670BB86F: 45 84 ED           test        r13b,r13b
  00000000670BB872: 75 0E              jne         00000000670BB882
  00000000670BB874: 48 01 83 B8 00 00  add         qword ptr [rbx+00000000000000B8h],rax
                    00
  00000000670BB87B: 48 01 83 C0 00 00  add         qword ptr [rbx+00000000000000C0h],rax
                    00
  00000000670BB882: 48 03 F0           add         rsi,rax
  00000000670BB885: 48 3B FD           cmp         rdi,rbp
  00000000670BB888: 0F 84 D6 00 00 00  je          00000000670BB964
  00000000670BB88E: 49 8B 06           mov         rax,qword ptr [r14]
  00000000670BB891: 48 8D 15 F8 F5 16  lea         rdx,[000000006722AE90h]
                    00
  00000000670BB898: 41 B8 02 00 00 00  mov         r8d,2
  00000000670BB89E: 49 8B CE           mov         rcx,r14
  00000000670BB8A1: FF 90 D0 00 00 00  call        qword ptr [rax+00000000000000D0h]
  00000000670BB8A7: 4C 8B D8           mov         r11,rax
  00000000670BB8AA: 48 85 C0           test        rax,rax
  00000000670BB8AD: 7E 4D              jle         00000000670BB8FC
  00000000670BB8AF: 45 84 ED           test        r13b,r13b
  00000000670BB8B2: 75 0E              jne         00000000670BB8C2
  00000000670BB8B4: 48 01 83 B8 00 00  add         qword ptr [rbx+00000000000000B8h],rax
                    00
  00000000670BB8BB: 48 01 83 C0 00 00  add         qword ptr [rbx+00000000000000C0h],rax
                    00
  00000000670BB8C2: 48 FF C6           inc         rsi
  00000000670BB8C5: 4C 8D 67 01        lea         r12,[rdi+1]
  00000000670BB8C9: E9 69 FF FF FF     jmp         00000000670BB837
  00000000670BB8CE: 48 85 F6           test        rsi,rsi
  00000000670BB8D1: 74 69              je          00000000670BB93C
  00000000670BB8D3: 45 84 ED           test        r13b,r13b
  00000000670BB8D6: 75 64              jne         00000000670BB93C
  00000000670BB8D8: 8B 83 98 00 00 00  mov         eax,dword ptr [rbx+0000000000000098h]
  00000000670BB8DE: 3B F0              cmp         esi,eax
  00000000670BB8E0: 7C 48              jl          00000000670BB92A
  00000000670BB8E2: 48 8B 83 A8 00 00  mov         rax,qword ptr [rbx+00000000000000A8h]
                    00
  00000000670BB8E9: C7 83 98 00 00 00  mov         dword ptr [rbx+0000000000000098h],0
                    00 00 00 00
  00000000670BB8F3: 48 89 83 A0 00 00  mov         qword ptr [rbx+00000000000000A0h],rax
                    00
  00000000670BB8FA: EB 40              jmp         00000000670BB93C
  00000000670BB8FC: 48 85 F6           test        rsi,rsi
  00000000670BB8FF: 74 3B              je          00000000670BB93C
  00000000670BB901: 45 84 ED           test        r13b,r13b
  00000000670BB904: 75 36              jne         00000000670BB93C
  00000000670BB906: 8B 83 98 00 00 00  mov         eax,dword ptr [rbx+0000000000000098h]
  00000000670BB90C: 3B F0              cmp         esi,eax
  00000000670BB90E: 7C 1A              jl          00000000670BB92A
  00000000670BB910: 48 8B 83 A8 00 00  mov         rax,qword ptr [rbx+00000000000000A8h]
                    00
  00000000670BB917: C7 83 98 00 00 00  mov         dword ptr [rbx+0000000000000098h],0
                    00 00 00 00
  00000000670BB921: 48 89 83 A0 00 00  mov         qword ptr [rbx+00000000000000A0h],rax
                    00
  00000000670BB928: EB 12              jmp         00000000670BB93C
  00000000670BB92A: 2B C6              sub         eax,esi
  00000000670BB92C: 89 83 98 00 00 00  mov         dword ptr [rbx+0000000000000098h],eax
  00000000670BB932: 48 63 C6           movsxd      rax,esi
  00000000670BB935: 48 01 83 A0 00 00  add         qword ptr [rbx+00000000000000A0h],rax
                    00
  00000000670BB93C: 48 85 F6           test        rsi,rsi
  00000000670BB93F: 4C 0F 45 DE        cmovne      r11,rsi
  00000000670BB943: 49 8B C3           mov         rax,r11
  00000000670BB946: 48 8B 74 24 48     mov         rsi,qword ptr [rsp+48h]
  00000000670BB94B: 48 8B 6C 24 40     mov         rbp,qword ptr [rsp+40h]
  00000000670BB950: 4C 8B 6C 24 50     mov         r13,qword ptr [rsp+50h]
  00000000670BB955: 48 8B 5C 24 58     mov         rbx,qword ptr [rsp+58h]
  00000000670BB95A: 48 83 C4 20        add         rsp,20h
  00000000670BB95E: 41 5E              pop         r14
  00000000670BB960: 41 5C              pop         r12
  00000000670BB962: 5F                 pop         rdi
  00000000670BB963: C3                 ret
  00000000670BB964: 48 85 F6           test        rsi,rsi
  00000000670BB967: 74 3E              je          00000000670BB9A7
  00000000670BB969: 45 84 ED           test        r13b,r13b
  00000000670BB96C: 75 39              jne         00000000670BB9A7
  00000000670BB96E: 8B 83 98 00 00 00  mov         eax,dword ptr [rbx+0000000000000098h]
  00000000670BB974: 3B F0              cmp         esi,eax
  00000000670BB976: 7C 1D              jl          00000000670BB995
  00000000670BB978: 48 8B 83 A8 00 00  mov         rax,qword ptr [rbx+00000000000000A8h]
                    00
  00000000670BB97F: C7 83 98 00 00 00  mov         dword ptr [rbx+0000000000000098h],0
                    00 00 00 00
  00000000670BB989: 48 89 83 A0 00 00  mov         qword ptr [rbx+00000000000000A0h],rax
                    00
  00000000670BB990: 48 8B C6           mov         rax,rsi
  00000000670BB993: EB B1              jmp         00000000670BB946
  00000000670BB995: 2B C6              sub         eax,esi
  00000000670BB997: 48 63 CE           movsxd      rcx,esi
  00000000670BB99A: 48 01 8B A0 00 00  add         qword ptr [rbx+00000000000000A0h],rcx
                    00
  00000000670BB9A1: 89 83 98 00 00 00  mov         dword ptr [rbx+0000000000000098h],eax
  00000000670BB9A7: 48 8B C6           mov         rax,rsi
  00000000670BB9AA: EB 9A              jmp         00000000670BB946
  00000000670BB9AC: 49 8B 06           mov         rax,qword ptr [r14]
  00000000670BB9AF: 4C 8B C7           mov         r8,rdi
  00000000670BB9B2: 49 8B D4           mov         rdx,r12
  00000000670BB9B5: 49 8B CE           mov         rcx,r14
  00000000670BB9B8: FF 90 D0 00 00 00  call        qword ptr [rax+00000000000000D0h]
  00000000670BB9BE: 4C 8B D8           mov         r11,rax
  00000000670BB9C1: 48 85 C0           test        rax,rax
  00000000670BB9C4: 7E 5F              jle         00000000670BBA25
  00000000670BB9C6: 45 84 ED           test        r13b,r13b
  00000000670BB9C9: 75 0E              jne         00000000670BB9D9
  00000000670BB9CB: 48 01 83 B8 00 00  add         qword ptr [rbx+00000000000000B8h],rax
                    00
  00000000670BB9D2: 48 01 83 C0 00 00  add         qword ptr [rbx+00000000000000C0h],rax
                    00
  00000000670BB9D9: 83 BB 98 00 00 00  cmp         dword ptr [rbx+0000000000000098h],0
                    00
  00000000670BB9E0: 74 43              je          00000000670BBA25
  00000000670BB9E2: 45 84 ED           test        r13b,r13b
  00000000670BB9E5: 75 3E              jne         00000000670BBA25
  00000000670BB9E7: 8B 83 98 00 00 00  mov         eax,dword ptr [rbx+0000000000000098h]
  00000000670BB9ED: 44 3B D8           cmp         r11d,eax
  00000000670BB9F0: 7C 20              jl          00000000670BBA12
  00000000670BB9F2: 48 8B 83 A8 00 00  mov         rax,qword ptr [rbx+00000000000000A8h]
                    00
  00000000670BB9F9: C7 83 98 00 00 00  mov         dword ptr [rbx+0000000000000098h],0
                    00 00 00 00
  00000000670BBA03: 48 89 83 A0 00 00  mov         qword ptr [rbx+00000000000000A0h],rax
                    00
  00000000670BBA0A: 49 8B C3           mov         rax,r11
  00000000670BBA0D: E9 3E FF FF FF     jmp         00000000670BB950
  00000000670BBA12: 41 2B C3           sub         eax,r11d
  00000000670BBA15: 89 83 98 00 00 00  mov         dword ptr [rbx+0000000000000098h],eax
  00000000670BBA1B: 49 63 C3           movsxd      rax,r11d
  00000000670BBA1E: 48 01 83 A0 00 00  add         qword ptr [rbx+00000000000000A0h],rax
                    00
  00000000670BBA25: 49 8B C3           mov         rax,r11
  00000000670BBA28: E9 23 FF FF FF     jmp         00000000670BB950
  00000000670BBA2D: CC                 int         3
  00000000670BBA2E: CC                 int         3
  00000000670BBA2F: CC                 int         3
  00000000670BBA30: 48 83 EC 28        sub         rsp,28h
  00000000670BBA34: 4C 8B C9           mov         r9,rcx
  00000000670BBA37: 48 85 D2           test        rdx,rdx
  00000000670BBA3A: 74 2A              je          00000000670BBA66
  00000000670BBA3C: 48 83 C9 FF        or          rcx,0FFFFFFFFFFFFFFFFh
  00000000670BBA40: 48 89 7C 24 20     mov         qword ptr [rsp+20h],rdi
  00000000670BBA45: 33 C0              xor         eax,eax
  00000000670BBA47: 48 8B FA           mov         rdi,rdx
  00000000670BBA4A: F2 AE              repne scas  byte ptr [rdi]
  00000000670BBA4C: 48 8B 7C 24 20     mov         rdi,qword ptr [rsp+20h]
  00000000670BBA51: 48 F7 D1           not         rcx
  00000000670BBA54: 48 FF C9           dec         rcx
  00000000670BBA57: 44 8B C1           mov         r8d,ecx
  00000000670BBA5A: 49 8B C9           mov         rcx,r9
  00000000670BBA5D: 48 83 C4 28        add         rsp,28h
  00000000670BBA61: E9 EA FC FF FF     jmp         00000000670BB750
  00000000670BBA66: 33 C9              xor         ecx,ecx
  00000000670BBA68: 44 8B C1           mov         r8d,ecx
  00000000670BBA6B: 49 8B C9           mov         rcx,r9
  00000000670BBA6E: 48 83 C4 28        add         rsp,28h
  00000000670BBA72: E9 D9 FC FF FF     jmp         00000000670BB750
  00000000670BBA77: CC                 int         3
  00000000670BBA78: CC                 int         3
  00000000670BBA79: CC                 int         3
  00000000670BBA7A: CC                 int         3
  00000000670BBA7B: CC                 int         3
  00000000670BBA7C: CC                 int         3
  00000000670BBA7D: CC                 int         3
  00000000670BBA7E: CC                 int         3
  00000000670BBA7F: CC                 int         3
  00000000670BBA80: 48 89 5C 24 10     mov         qword ptr [rsp+10h],rbx
  00000000670BBA85: 56                 push        rsi
  00000000670BBA86: 48 83 EC 20        sub         rsp,20h
  00000000670BBA8A: 48 8B 59 08        mov         rbx,qword ptr [rcx+8]
  00000000670BBA8E: 0F B6 F2           movzx       esi,dl
  00000000670BBA91: 8B 83 88 00 00 00  mov         eax,dword ptr [rbx+0000000000000088h]
  00000000670BBA97: A8 01              test        al,1
  00000000670BBA99: 75 1E              jne         00000000670BBAB9
  00000000670BBA9B: 85 C0              test        eax,eax
  00000000670BBA9D: 0F 84 91 00 00 00  je          00000000670BBB34
  00000000670BBAA3: 48 8D 0D 76 F2 16  lea         rcx,[000000006722AD20h]
                    00
  00000000670BBAAA: 48 8B 5C 24 38     mov         rbx,qword ptr [rsp+38h]
  00000000670BBAAF: 48 83 C4 20        add         rsp,20h
  00000000670BBAB3: 5E                 pop         rsi
  00000000670BBAB4: E9 27 14 F6 FF     jmp         000000006701CEE0
  00000000670BBAB9: 48 8B 83 A8 00 00  mov         rax,qword ptr [rbx+00000000000000A8h]
                    00
  00000000670BBAC0: 48 89 7C 24 30     mov         qword ptr [rsp+30h],rdi
  00000000670BBAC5: 48 39 83 A0 00 00  cmp         qword ptr [rbx+00000000000000A0h],rax
                    00
  00000000670BBACC: 75 1A              jne         00000000670BBAE8
  00000000670BBACE: 8B 83 98 00 00 00  mov         eax,dword ptr [rbx+0000000000000098h]
  00000000670BBAD4: 48 8D 8B 98 00 00  lea         rcx,[rbx+0000000000000098h]
                    00
  00000000670BBADB: 45 33 C0           xor         r8d,r8d
  00000000670BBADE: FF C0              inc         eax
  00000000670BBAE0: 48 63 D0           movsxd      rdx,eax
  00000000670BBAE3: E8 18 E6 FF FF     call        00000000670BA100
  00000000670BBAE8: 48 FF 8B A0 00 00  dec         qword ptr [rbx+00000000000000A0h]
                    00
  00000000670BBAEF: FF 83 98 00 00 00  inc         dword ptr [rbx+0000000000000098h]
  00000000670BBAF5: 48 8B 83 A0 00 00  mov         rax,qword ptr [rbx+00000000000000A0h]
                    00
  00000000670BBAFC: 48 8B 7C 24 30     mov         rdi,qword ptr [rsp+30h]
  00000000670BBB01: 40 88 30           mov         byte ptr [rax],sil
  00000000670BBB04: 83 BB E4 00 00 00  cmp         dword ptr [rbx+00000000000000E4h],0
                    00
  00000000670BBB0B: 75 17              jne         00000000670BBB24
  00000000670BBB0D: 48 8B 4B 08        mov         rcx,qword ptr [rbx+8]
  00000000670BBB11: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000670BBB14: FF 50 58           call        qword ptr [rax+58h]
  00000000670BBB17: F6 D8              neg         al
  00000000670BBB19: 1B C9              sbb         ecx,ecx
  00000000670BBB1B: 83 C1 02           add         ecx,2
  00000000670BBB1E: 89 8B E4 00 00 00  mov         dword ptr [rbx+00000000000000E4h],ecx
  00000000670BBB24: 83 BB E4 00 00 00  cmp         dword ptr [rbx+00000000000000E4h],1
                    01
  00000000670BBB2B: 74 07              je          00000000670BBB34
  00000000670BBB2D: 48 FF 8B B8 00 00  dec         qword ptr [rbx+00000000000000B8h]
                    00
  00000000670BBB34: 48 8B 5C 24 38     mov         rbx,qword ptr [rsp+38h]
  00000000670BBB39: 48 83 C4 20        add         rsp,20h
  00000000670BBB3D: 5E                 pop         rsi
  00000000670BBB3E: C3                 ret
  00000000670BBB3F: CC                 int         3
  00000000670BBB40: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  00000000670BBB44: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000670BBB47: 48 FF 60 08        jmp         qword ptr [rax+8]
  00000000670BBB4B: CC                 int         3
  00000000670BBB4C: CC                 int         3
  00000000670BBB4D: CC                 int         3
  00000000670BBB4E: CC                 int         3
  00000000670BBB4F: CC                 int         3
  00000000670BBB50: 88 54 24 10        mov         byte ptr [rsp+10h],dl
  00000000670BBB54: 48 83 EC 28        sub         rsp,28h
  00000000670BBB58: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  00000000670BBB5C: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  00000000670BBB61: 41 B8 01 00 00 00  mov         r8d,1
  00000000670BBB67: E8 E4 FB FF FF     call        00000000670BB750
  00000000670BBB6C: 48 83 F8 01        cmp         rax,1
  00000000670BBB70: 0F 94 C0           sete        al
  00000000670BBB73: 48 83 C4 28        add         rsp,28h
  00000000670BBB77: C3                 ret
  00000000670BBB78: CC                 int         3
  00000000670BBB79: CC                 int         3
  00000000670BBB7A: CC                 int         3
  00000000670BBB7B: CC                 int         3
  00000000670BBB7C: CC                 int         3
  00000000670BBB7D: CC                 int         3
  00000000670BBB7E: CC                 int         3
  00000000670BBB7F: CC                 int         3
  00000000670BBB80: 48 89 6C 24 18     mov         qword ptr [rsp+18h],rbp
  00000000670BBB85: 48 89 7C 24 20     mov         qword ptr [rsp+20h],rdi
  00000000670BBB8A: 41 54              push        r12
  00000000670BBB8C: 48 83 EC 20        sub         rsp,20h
  00000000670BBB90: 48 8B E9           mov         rbp,rcx
  00000000670BBB93: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  00000000670BBB97: 4C 8B E2           mov         r12,rdx
  00000000670BBB9A: E8 D1 EB FF FF     call        00000000670BA770
  00000000670BBB9F: 48 8B F8           mov         rdi,rax
  00000000670BBBA2: 48 85 C0           test        rax,rax
  00000000670BBBA5: 7E 70              jle         00000000670BBC17
  00000000670BBBA7: 48 89 5C 24 30     mov         qword ptr [rsp+30h],rbx
  00000000670BBBAC: 48 89 74 24 38     mov         qword ptr [rsp+38h],rsi
  00000000670BBBB1: 48 63 F0           movsxd      rsi,eax
  00000000670BBBB4: 48 8B 85 A0 00 00  mov         rax,qword ptr [rbp+00000000000000A0h]
                    00
  00000000670BBBBB: 48 2B 85 A8 00 00  sub         rax,qword ptr [rbp+00000000000000A8h]
                    00
  00000000670BBBC2: 48 3B C6           cmp         rax,rsi
  00000000670BBBC5: 7D 1A              jge         00000000670BBBE1
  00000000670BBBC7: 8B 85 98 00 00 00  mov         eax,dword ptr [rbp+0000000000000098h]
  00000000670BBBCD: 48 8D 8D 98 00 00  lea         rcx,[rbp+0000000000000098h]
                    00
  00000000670BBBD4: 45 33 C0           xor         r8d,r8d
  00000000670BBBD7: 03 C7              add         eax,edi
  00000000670BBBD9: 48 63 D0           movsxd      rdx,eax
  00000000670BBBDC: E8 1F E5 FF FF     call        00000000670BA100
  00000000670BBBE1: 48 29 B5 A0 00 00  sub         qword ptr [rbp+00000000000000A0h],rsi
                    00
  00000000670BBBE8: 48 8B 8D A0 00 00  mov         rcx,qword ptr [rbp+00000000000000A0h]
                    00
  00000000670BBBEF: 01 BD 98 00 00 00  add         dword ptr [rbp+0000000000000098h],edi
  00000000670BBBF5: 4C 8B C6           mov         r8,rsi
  00000000670BBBF8: 49 8B D4           mov         rdx,r12
  00000000670BBBFB: E8 9E CE 0B 00     call        0000000067178A9E
  00000000670BBC00: 4C 8B 9D D0 00 00  mov         r11,qword ptr [rbp+00000000000000D0h]
                    00
  00000000670BBC07: 48 8B 74 24 38     mov         rsi,qword ptr [rsp+38h]
  00000000670BBC0C: 49 29 3B           sub         qword ptr [r11],rdi
  00000000670BBC0F: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  00000000670BBC14: 48 8B C7           mov         rax,rdi
  00000000670BBC17: 48 8B 6C 24 40     mov         rbp,qword ptr [rsp+40h]
  00000000670BBC1C: 48 8B 7C 24 48     mov         rdi,qword ptr [rsp+48h]

  Summary

      1A7000 .text
