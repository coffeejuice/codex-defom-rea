Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001412F0380: 48 89 6C 24 20     mov         qword ptr [rsp+20h],rbp
  00000001412F0385: 56                 push        rsi
  00000001412F0386: 57                 push        rdi
  00000001412F0387: 41 54              push        r12
  00000001412F0389: 48 83 EC 50        sub         rsp,50h
  00000001412F038D: 48 83 79 38 00     cmp         qword ptr [rcx+38h],0
  00000001412F0392: 49 8B E9           mov         rbp,r9
  00000001412F0395: 4D 8B E0           mov         r12,r8
  00000001412F0398: 48 8B FA           mov         rdi,rdx
  00000001412F039B: 48 8B F1           mov         rsi,rcx
  00000001412F039E: 75 16              jne         00000001412F03B6
  00000001412F03A0: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  00000001412F03A5: 4C 8B 49 30        mov         r9,qword ptr [rcx+30h]
  00000001412F03A9: 41 B0 01           mov         r8b,1
  00000001412F03AC: E8 AF F6 FF FF     call        00000001412EFA60
  00000001412F03B1: E9 D7 02 00 00     jmp         00000001412F068D
  00000001412F03B6: 48 8B 41 30        mov         rax,qword ptr [rcx+30h]
  00000001412F03BA: 48 89 5C 24 70     mov         qword ptr [rsp+70h],rbx
  00000001412F03BF: 48 8B 18           mov         rbx,qword ptr [rax]
  00000001412F03C2: 49 8B 00           mov         rax,qword ptr [r8]
  00000001412F03C5: 48 85 C0           test        rax,rax
  00000001412F03C8: 74 05              je          00000001412F03CF
  00000001412F03CA: 48 3B 01           cmp         rax,qword ptr [rcx]
  00000001412F03CD: 74 05              je          00000001412F03D4
  00000001412F03CF: E8 1C 02 18 00     call        00000001414705F0
  00000001412F03D4: 4D 8B 4C 24 08     mov         r9,qword ptr [r12+8]
  00000001412F03D9: 4C 3B CB           cmp         r9,rbx
  00000001412F03DC: 75 26              jne         00000001412F0404
  00000001412F03DE: 49 8B 41 18        mov         rax,qword ptr [r9+18h]
  00000001412F03E2: 48 39 45 00        cmp         qword ptr [rbp],rax
  00000001412F03E6: 0F 83 7E 02 00 00  jae         00000001412F066A
  00000001412F03EC: 41 B0 01           mov         r8b,1
  00000001412F03EF: 48 8B D7           mov         rdx,rdi
  00000001412F03F2: 48 8B CE           mov         rcx,rsi
  00000001412F03F5: 48 89 6C 24 20     mov         qword ptr [rsp+20h],rbp
  00000001412F03FA: E8 61 F6 FF FF     call        00000001412EFA60
  00000001412F03FF: E9 84 02 00 00     jmp         00000001412F0688
  00000001412F0404: 49 8B 04 24        mov         rax,qword ptr [r12]
  00000001412F0408: 48 8B 5E 30        mov         rbx,qword ptr [rsi+30h]
  00000001412F040C: 48 85 C0           test        rax,rax
  00000001412F040F: 74 05              je          00000001412F0416
  00000001412F0411: 48 3B 06           cmp         rax,qword ptr [rsi]
  00000001412F0414: 74 05              je          00000001412F041B
  00000001412F0416: E8 D5 01 18 00     call        00000001414705F0
  00000001412F041B: 49 8B 44 24 08     mov         rax,qword ptr [r12+8]
  00000001412F0420: 48 3B C3           cmp         rax,rbx
  00000001412F0423: 75 2E              jne         00000001412F0453
  00000001412F0425: 48 8B 46 30        mov         rax,qword ptr [rsi+30h]
  00000001412F0429: 4C 8B 48 10        mov         r9,qword ptr [rax+10h]
  00000001412F042D: 48 8B 45 00        mov         rax,qword ptr [rbp]
  00000001412F0431: 49 39 41 18        cmp         qword ptr [r9+18h],rax
  00000001412F0435: 0F 83 2F 02 00 00  jae         00000001412F066A
  00000001412F043B: 45 33 C0           xor         r8d,r8d
  00000001412F043E: 48 8B D7           mov         rdx,rdi
  00000001412F0441: 48 8B CE           mov         rcx,rsi
  00000001412F0444: 48 89 6C 24 20     mov         qword ptr [rsp+20h],rbp
  00000001412F0449: E8 12 F6 FF FF     call        00000001412EFA60
  00000001412F044E: E9 35 02 00 00     jmp         00000001412F0688
  00000001412F0453: 48 8B 4D 00        mov         rcx,qword ptr [rbp]
  00000001412F0457: 48 3B 48 18        cmp         rcx,qword ptr [rax+18h]
  00000001412F045B: 0F 83 F7 00 00 00  jae         00000001412F0558
  00000001412F0461: 49 8B 04 24        mov         rax,qword ptr [r12]
  00000001412F0465: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412F046A: 48 89 01           mov         qword ptr [rcx],rax
  00000001412F046D: 49 8B 44 24 08     mov         rax,qword ptr [r12+8]
  00000001412F0472: 48 89 41 08        mov         qword ptr [rcx+8],rax
  00000001412F0476: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  00000001412F047C: 75 05              jne         00000001412F0483
  00000001412F047E: E8 6D 01 18 00     call        00000001414705F0
  00000001412F0483: 48 8B 5C 24 38     mov         rbx,qword ptr [rsp+38h]
  00000001412F0488: 80 7B 29 00        cmp         byte ptr [rbx+29h],0
  00000001412F048C: 74 16              je          00000001412F04A4
  00000001412F048E: 48 8B 5B 10        mov         rbx,qword ptr [rbx+10h]
  00000001412F0492: 48 89 5C 24 38     mov         qword ptr [rsp+38h],rbx
  00000001412F0497: 80 7B 29 00        cmp         byte ptr [rbx+29h],0
  00000001412F049B: 74 6F              je          00000001412F050C
  00000001412F049D: E8 4E 01 18 00     call        00000001414705F0
  00000001412F04A2: EB 68              jmp         00000001412F050C
  00000001412F04A4: 48 8B 03           mov         rax,qword ptr [rbx]
  00000001412F04A7: 80 78 29 00        cmp         byte ptr [rax+29h],0
  00000001412F04AB: 75 27              jne         00000001412F04D4
  00000001412F04AD: 48 8B D8           mov         rbx,rax
  00000001412F04B0: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  00000001412F04B4: 80 78 29 00        cmp         byte ptr [rax+29h],0
  00000001412F04B8: 75 13              jne         00000001412F04CD
  00000001412F04BA: 66 0F 1F 44 00 00  nop         word ptr [rax+rax]
  00000001412F04C0: 48 8B D8           mov         rbx,rax
  00000001412F04C3: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  00000001412F04C7: 80 78 29 00        cmp         byte ptr [rax+29h],0
  00000001412F04CB: 74 F3              je          00000001412F04C0
  00000001412F04CD: 48 89 5C 24 38     mov         qword ptr [rsp+38h],rbx
  00000001412F04D2: EB 38              jmp         00000001412F050C
  00000001412F04D4: 48 8B 43 08        mov         rax,qword ptr [rbx+8]
  00000001412F04D8: 80 78 29 00        cmp         byte ptr [rax+29h],0
  00000001412F04DC: 75 19              jne         00000001412F04F7
  00000001412F04DE: 66 90              nop
  00000001412F04E0: 48 3B 18           cmp         rbx,qword ptr [rax]
  00000001412F04E3: 75 12              jne         00000001412F04F7
  00000001412F04E5: 48 8B D8           mov         rbx,rax
  00000001412F04E8: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001412F04ED: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  00000001412F04F1: 80 78 29 00        cmp         byte ptr [rax+29h],0
  00000001412F04F5: 74 E9              je          00000001412F04E0
  00000001412F04F7: 80 7B 29 00        cmp         byte ptr [rbx+29h],0
  00000001412F04FB: 74 07              je          00000001412F0504
  00000001412F04FD: E8 EE 00 18 00     call        00000001414705F0
  00000001412F0502: EB 08              jmp         00000001412F050C
  00000001412F0504: 48 8B D8           mov         rbx,rax
  00000001412F0507: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001412F050C: 48 8B 4D 00        mov         rcx,qword ptr [rbp]
  00000001412F0510: 48 39 4B 18        cmp         qword ptr [rbx+18h],rcx
  00000001412F0514: 73 42              jae         00000001412F0558
  00000001412F0516: 48 8B 43 10        mov         rax,qword ptr [rbx+10h]
  00000001412F051A: 80 78 29 00        cmp         byte ptr [rax+29h],0
  00000001412F051E: 74 1B              je          00000001412F053B
  00000001412F0520: 4C 8B CB           mov         r9,rbx
  00000001412F0523: 45 33 C0           xor         r8d,r8d
  00000001412F0526: 48 8B D7           mov         rdx,rdi
  00000001412F0529: 48 8B CE           mov         rcx,rsi
  00000001412F052C: 48 89 6C 24 20     mov         qword ptr [rsp+20h],rbp
  00000001412F0531: E8 2A F5 FF FF     call        00000001412EFA60
  00000001412F0536: E9 4D 01 00 00     jmp         00000001412F0688
  00000001412F053B: 4D 8B 4C 24 08     mov         r9,qword ptr [r12+8]
  00000001412F0540: 41 B0 01           mov         r8b,1
  00000001412F0543: 48 8B D7           mov         rdx,rdi
  00000001412F0546: 48 8B CE           mov         rcx,rsi
  00000001412F0549: 48 89 6C 24 20     mov         qword ptr [rsp+20h],rbp
  00000001412F054E: E8 0D F5 FF FF     call        00000001412EFA60
  00000001412F0553: E9 30 01 00 00     jmp         00000001412F0688
  00000001412F0558: 49 8B 44 24 08     mov         rax,qword ptr [r12+8]
  00000001412F055D: 48 39 48 18        cmp         qword ptr [rax+18h],rcx
  00000001412F0561: 0F 83 03 01 00 00  jae         00000001412F066A
  00000001412F0567: 49 8B 04 24        mov         rax,qword ptr [r12]
  00000001412F056B: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412F0570: 4C 89 74 24 78     mov         qword ptr [rsp+78h],r14
  00000001412F0575: 4C 8B 76 30        mov         r14,qword ptr [rsi+30h]
  00000001412F0579: 48 89 01           mov         qword ptr [rcx],rax
  00000001412F057C: 49 8B 44 24 08     mov         rax,qword ptr [r12+8]
  00000001412F0581: 48 89 41 08        mov         qword ptr [rcx+8],rax
  00000001412F0585: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  00000001412F058B: 4C 89 BC 24 80 00  mov         qword ptr [rsp+0000000000000080h],r15
                    00 00
  00000001412F0593: 4C 8B 3E           mov         r15,qword ptr [rsi]
  00000001412F0596: 75 05              jne         00000001412F059D
  00000001412F0598: E8 53 00 18 00     call        00000001414705F0
  00000001412F059D: 48 8B 5C 24 38     mov         rbx,qword ptr [rsp+38h]
  00000001412F05A2: 80 7B 29 00        cmp         byte ptr [rbx+29h],0
  00000001412F05A6: 74 07              je          00000001412F05AF
  00000001412F05A8: E8 43 00 18 00     call        00000001414705F0
  00000001412F05AD: EB 4F              jmp         00000001412F05FE
  00000001412F05AF: 48 8B 43 10        mov         rax,qword ptr [rbx+10h]
  00000001412F05B3: 80 78 29 00        cmp         byte ptr [rax+29h],0
  00000001412F05B7: 75 25              jne         00000001412F05DE
  00000001412F05B9: 48 8B D8           mov         rbx,rax
  00000001412F05BC: 48 8B 00           mov         rax,qword ptr [rax]
  00000001412F05BF: 80 78 29 00        cmp         byte ptr [rax+29h],0
  00000001412F05C3: 75 39              jne         00000001412F05FE
  00000001412F05C5: 66 66 66 0F 1F 84  nop         word ptr [rax+rax+0000000000000000h]
                    00 00 00 00 00
  00000001412F05D0: 48 8B D8           mov         rbx,rax
  00000001412F05D3: 48 8B 00           mov         rax,qword ptr [rax]
  00000001412F05D6: 80 78 29 00        cmp         byte ptr [rax+29h],0
  00000001412F05DA: 74 F4              je          00000001412F05D0
  00000001412F05DC: EB 20              jmp         00000001412F05FE
  00000001412F05DE: 48 8B 43 08        mov         rax,qword ptr [rbx+8]
  00000001412F05E2: 80 78 29 00        cmp         byte ptr [rax+29h],0
  00000001412F05E6: 75 13              jne         00000001412F05FB
  00000001412F05E8: 48 3B 58 10        cmp         rbx,qword ptr [rax+10h]
  00000001412F05EC: 75 0D              jne         00000001412F05FB
  00000001412F05EE: 48 8B D8           mov         rbx,rax
  00000001412F05F1: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  00000001412F05F5: 80 78 29 00        cmp         byte ptr [rax+29h],0
  00000001412F05F9: 74 ED              je          00000001412F05E8
  00000001412F05FB: 48 8B D8           mov         rbx,rax
  00000001412F05FE: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  00000001412F0604: 74 07              je          00000001412F060D
  00000001412F0606: 4C 39 7C 24 30     cmp         qword ptr [rsp+30h],r15
  00000001412F060B: 74 05              je          00000001412F0612
  00000001412F060D: E8 DE FF 17 00     call        00000001414705F0
  00000001412F0612: 4C 8B BC 24 80 00  mov         r15,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001412F061A: 49 3B DE           cmp         rbx,r14
  00000001412F061D: 4C 8B 74 24 78     mov         r14,qword ptr [rsp+78h]
  00000001412F0622: 74 0A              je          00000001412F062E
  00000001412F0624: 48 8B 43 18        mov         rax,qword ptr [rbx+18h]
  00000001412F0628: 48 39 45 00        cmp         qword ptr [rbp],rax
  00000001412F062C: 73 3C              jae         00000001412F066A
  00000001412F062E: 4D 8B 4C 24 08     mov         r9,qword ptr [r12+8]
  00000001412F0633: 49 8B 41 10        mov         rax,qword ptr [r9+10h]
  00000001412F0637: 80 78 29 00        cmp         byte ptr [rax+29h],0
  00000001412F063B: 74 15              je          00000001412F0652
  00000001412F063D: 45 33 C0           xor         r8d,r8d
  00000001412F0640: 48 8B D7           mov         rdx,rdi
  00000001412F0643: 48 8B CE           mov         rcx,rsi
  00000001412F0646: 48 89 6C 24 20     mov         qword ptr [rsp+20h],rbp
  00000001412F064B: E8 10 F4 FF FF     call        00000001412EFA60
  00000001412F0650: EB 36              jmp         00000001412F0688
  00000001412F0652: 4C 8B CB           mov         r9,rbx
  00000001412F0655: 41 B0 01           mov         r8b,1
  00000001412F0658: 48 8B D7           mov         rdx,rdi
  00000001412F065B: 48 8B CE           mov         rcx,rsi
  00000001412F065E: 48 89 6C 24 20     mov         qword ptr [rsp+20h],rbp
  00000001412F0663: E8 F8 F3 FF FF     call        00000001412EFA60
  00000001412F0668: EB 1E              jmp         00000001412F0688
  00000001412F066A: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  00000001412F066F: 4C 8B C5           mov         r8,rbp
  00000001412F0672: 48 8B CE           mov         rcx,rsi
  00000001412F0675: E8 56 FB FF FF     call        00000001412F01D0
  00000001412F067A: 48 8B 08           mov         rcx,qword ptr [rax]
  00000001412F067D: 48 89 0F           mov         qword ptr [rdi],rcx
  00000001412F0680: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  00000001412F0684: 48 89 47 08        mov         qword ptr [rdi+8],rax
  00000001412F0688: 48 8B 5C 24 70     mov         rbx,qword ptr [rsp+70h]
  00000001412F068D: 48 8B C7           mov         rax,rdi
  00000001412F0690: 48 8B AC 24 88 00  mov         rbp,qword ptr [rsp+0000000000000088h]
                    00 00
  00000001412F0698: 48 83 C4 50        add         rsp,50h
  00000001412F069C: 41 5C              pop         r12
  00000001412F069E: 5F                 pop         rdi
  00000001412F069F: 5E                 pop         rsi
  00000001412F06A0: C3                 ret
  00000001412F06A1: CC                 int         3
  00000001412F06A2: CC                 int         3
  00000001412F06A3: CC                 int         3
  00000001412F06A4: CC                 int         3
  00000001412F06A5: CC                 int         3
  00000001412F06A6: CC                 int         3
  00000001412F06A7: CC                 int         3
  00000001412F06A8: CC                 int         3
  00000001412F06A9: CC                 int         3
  00000001412F06AA: CC                 int         3
  00000001412F06AB: CC                 int         3
  00000001412F06AC: CC                 int         3
  00000001412F06AD: CC                 int         3
  00000001412F06AE: CC                 int         3
  00000001412F06AF: CC                 int         3

  Summary

     1E02000 .text
