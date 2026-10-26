Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001412F01D0: 48 89 5C 24 10     mov         qword ptr [rsp+10h],rbx
  00000001412F01D5: 48 89 6C 24 18     mov         qword ptr [rsp+18h],rbp
  00000001412F01DA: 56                 push        rsi
  00000001412F01DB: 57                 push        rdi
  00000001412F01DC: 41 54              push        r12
  00000001412F01DE: 41 56              push        r14
  00000001412F01E0: 41 57              push        r15
  00000001412F01E2: 48 83 EC 40        sub         rsp,40h
  00000001412F01E6: 48 8B 79 30        mov         rdi,qword ptr [rcx+30h]
  00000001412F01EA: 4D 8B F0           mov         r14,r8
  00000001412F01ED: 4C 8B E2           mov         r12,rdx
  00000001412F01F0: 48 8B 47 08        mov         rax,qword ptr [rdi+8]
  00000001412F01F4: 4C 8B F9           mov         r15,rcx
  00000001412F01F7: 40 B5 01           mov         bpl,1
  00000001412F01FA: 80 78 29 00        cmp         byte ptr [rax+29h],0
  00000001412F01FE: 75 2F              jne         00000001412F022F
  00000001412F0200: 4D 8B 08           mov         r9,qword ptr [r8]
  00000001412F0203: 66 66 66 66 66 0F  nop         word ptr [rax+rax+0000000000000000h]
                    1F 84 00 00 00 00
                    00
  00000001412F0210: 4C 3B 48 18        cmp         r9,qword ptr [rax+18h]
  00000001412F0214: 48 8B F8           mov         rdi,rax
  00000001412F0217: 40 0F 92 C5        setb        bpl
  00000001412F021B: 40 84 ED           test        bpl,bpl
  00000001412F021E: 74 05              je          00000001412F0225
  00000001412F0220: 48 8B 00           mov         rax,qword ptr [rax]
  00000001412F0223: EB 04              jmp         00000001412F0229
  00000001412F0225: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  00000001412F0229: 80 78 29 00        cmp         byte ptr [rax+29h],0
  00000001412F022D: 74 E1              je          00000001412F0210
  00000001412F022F: 48 8B 31           mov         rsi,qword ptr [rcx]
  00000001412F0232: 48 8B DF           mov         rbx,rdi
  00000001412F0235: 48 89 74 24 30     mov         qword ptr [rsp+30h],rsi
  00000001412F023A: 48 89 5C 24 38     mov         qword ptr [rsp+38h],rbx
  00000001412F023F: 40 84 ED           test        bpl,bpl
  00000001412F0242: 0F 84 E4 00 00 00  je          00000001412F032C
  00000001412F0248: 48 8B 41 30        mov         rax,qword ptr [rcx+30h]
  00000001412F024C: 4C 89 6C 24 70     mov         qword ptr [rsp+70h],r13
  00000001412F0251: 4C 8B 28           mov         r13,qword ptr [rax]
  00000001412F0254: 48 85 F6           test        rsi,rsi
  00000001412F0257: 74 05              je          00000001412F025E
  00000001412F0259: 48 3B F6           cmp         rsi,rsi
  00000001412F025C: 74 05              je          00000001412F0263
  00000001412F025E: E8 8D 03 18 00     call        00000001414705F0
  00000001412F0263: 49 3B FD           cmp         rdi,r13
  00000001412F0266: 4C 8B 6C 24 70     mov         r13,qword ptr [rsp+70h]
  00000001412F026B: 75 2E              jne         00000001412F029B
  00000001412F026D: 4C 89 74 24 20     mov         qword ptr [rsp+20h],r14
  00000001412F0272: 4C 8B CF           mov         r9,rdi
  00000001412F0275: 41 B0 01           mov         r8b,1
  00000001412F0278: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  00000001412F027D: 49 8B CF           mov         rcx,r15
  00000001412F0280: E8 DB F7 FF FF     call        00000001412EFA60
  00000001412F0285: 41 C6 44 24 10 01  mov         byte ptr [r12+10h],1
  00000001412F028B: 48 8B 08           mov         rcx,qword ptr [rax]
  00000001412F028E: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  00000001412F0292: 49 89 0C 24        mov         qword ptr [r12],rcx
  00000001412F0296: E9 C1 00 00 00     jmp         00000001412F035C
  00000001412F029B: 48 85 F6           test        rsi,rsi
  00000001412F029E: 75 05              jne         00000001412F02A5
  00000001412F02A0: E8 4B 03 18 00     call        00000001414705F0
  00000001412F02A5: 80 7F 29 00        cmp         byte ptr [rdi+29h],0
  00000001412F02A9: 74 16              je          00000001412F02C1
  00000001412F02AB: 48 8B 5F 10        mov         rbx,qword ptr [rdi+10h]
  00000001412F02AF: 48 89 5C 24 38     mov         qword ptr [rsp+38h],rbx
  00000001412F02B4: 80 7B 29 00        cmp         byte ptr [rbx+29h],0
  00000001412F02B8: 74 72              je          00000001412F032C
  00000001412F02BA: E8 31 03 18 00     call        00000001414705F0
  00000001412F02BF: EB 6B              jmp         00000001412F032C
  00000001412F02C1: 48 8B 07           mov         rax,qword ptr [rdi]
  00000001412F02C4: 80 78 29 00        cmp         byte ptr [rax+29h],0
  00000001412F02C8: 75 2A              jne         00000001412F02F4
  00000001412F02CA: 48 8B D8           mov         rbx,rax
  00000001412F02CD: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  00000001412F02D1: 80 78 29 00        cmp         byte ptr [rax+29h],0
  00000001412F02D5: 75 16              jne         00000001412F02ED
  00000001412F02D7: 66 0F 1F 84 00 00  nop         word ptr [rax+rax+0000000000000000h]
                    00 00 00
  00000001412F02E0: 48 8B D8           mov         rbx,rax
  00000001412F02E3: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  00000001412F02E7: 80 78 29 00        cmp         byte ptr [rax+29h],0
  00000001412F02EB: 74 F3              je          00000001412F02E0
  00000001412F02ED: 48 89 5C 24 38     mov         qword ptr [rsp+38h],rbx
  00000001412F02F2: EB 38              jmp         00000001412F032C
  00000001412F02F4: 48 8B 47 08        mov         rax,qword ptr [rdi+8]
  00000001412F02F8: 80 78 29 00        cmp         byte ptr [rax+29h],0
  00000001412F02FC: 75 19              jne         00000001412F0317
  00000001412F02FE: 66 90              nop
  00000001412F0300: 48 3B 18           cmp         rbx,qword ptr [rax]
  00000001412F0303: 75 12              jne         00000001412F0317
  00000001412F0305: 48 8B D8           mov         rbx,rax
  00000001412F0308: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001412F030D: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  00000001412F0311: 80 78 29 00        cmp         byte ptr [rax+29h],0
  00000001412F0315: 74 E9              je          00000001412F0300
  00000001412F0317: 80 7B 29 00        cmp         byte ptr [rbx+29h],0
  00000001412F031B: 74 07              je          00000001412F0324
  00000001412F031D: E8 CE 02 18 00     call        00000001414705F0
  00000001412F0322: EB 08              jmp         00000001412F032C
  00000001412F0324: 48 8B D8           mov         rbx,rax
  00000001412F0327: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001412F032C: 49 8B 06           mov         rax,qword ptr [r14]
  00000001412F032F: 48 39 43 18        cmp         qword ptr [rbx+18h],rax
  00000001412F0333: 73 11              jae         00000001412F0346
  00000001412F0335: 4C 89 74 24 20     mov         qword ptr [rsp+20h],r14
  00000001412F033A: 4C 8B CF           mov         r9,rdi
  00000001412F033D: 44 0F B6 C5        movzx       r8d,bpl
  00000001412F0341: E9 32 FF FF FF     jmp         00000001412F0278
  00000001412F0346: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412F034B: 41 C6 44 24 10 00  mov         byte ptr [r12+10h],0
  00000001412F0351: 48 8B 01           mov         rax,qword ptr [rcx]
  00000001412F0354: 49 89 04 24        mov         qword ptr [r12],rax
  00000001412F0358: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  00000001412F035C: 48 8B 5C 24 78     mov         rbx,qword ptr [rsp+78h]
  00000001412F0361: 48 8B AC 24 80 00  mov         rbp,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001412F0369: 49 89 44 24 08     mov         qword ptr [r12+8],rax
  00000001412F036E: 49 8B C4           mov         rax,r12
  00000001412F0371: 48 83 C4 40        add         rsp,40h
  00000001412F0375: 41 5F              pop         r15
  00000001412F0377: 41 5E              pop         r14
  00000001412F0379: 41 5C              pop         r12
  00000001412F037B: 5F                 pop         rdi
  00000001412F037C: 5E                 pop         rsi
  00000001412F037D: C3                 ret
  00000001412F037E: CC                 int         3
  00000001412F037F: CC                 int         3

  Summary

     1E02000 .text
