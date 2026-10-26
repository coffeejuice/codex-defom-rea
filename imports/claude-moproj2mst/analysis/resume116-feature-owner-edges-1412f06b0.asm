Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001412F06B0: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  00000001412F06B5: 48 89 6C 24 10     mov         qword ptr [rsp+10h],rbp
  00000001412F06BA: 48 89 74 24 18     mov         qword ptr [rsp+18h],rsi
  00000001412F06BF: 48 89 7C 24 20     mov         qword ptr [rsp+20h],rdi
  00000001412F06C4: 41 54              push        r12
  00000001412F06C6: 48 83 EC 50        sub         rsp,50h
  00000001412F06CA: 48 8B 41 30        mov         rax,qword ptr [rcx+30h]
  00000001412F06CE: 49 8B F1           mov         rsi,r9
  00000001412F06D1: 49 8B D8           mov         rbx,r8
  00000001412F06D4: 48 8B 38           mov         rdi,qword ptr [rax]
  00000001412F06D7: 49 8B 00           mov         rax,qword ptr [r8]
  00000001412F06DA: 4C 8B E2           mov         r12,rdx
  00000001412F06DD: 48 8B E9           mov         rbp,rcx
  00000001412F06E0: 48 85 C0           test        rax,rax
  00000001412F06E3: 74 05              je          00000001412F06EA
  00000001412F06E5: 48 3B 01           cmp         rax,qword ptr [rcx]
  00000001412F06E8: 74 05              je          00000001412F06EF
  00000001412F06EA: E8 01 FF 17 00     call        00000001414705F0
  00000001412F06EF: 48 39 7B 08        cmp         qword ptr [rbx+8],rdi
  00000001412F06F3: 75 3E              jne         00000001412F0733
  00000001412F06F5: 48 8B 06           mov         rax,qword ptr [rsi]
  00000001412F06F8: 48 8B 7D 30        mov         rdi,qword ptr [rbp+30h]
  00000001412F06FC: 48 85 C0           test        rax,rax
  00000001412F06FF: 74 06              je          00000001412F0707
  00000001412F0701: 48 3B 45 00        cmp         rax,qword ptr [rbp]
  00000001412F0705: 74 05              je          00000001412F070C
  00000001412F0707: E8 E4 FE 17 00     call        00000001414705F0
  00000001412F070C: 48 39 7E 08        cmp         qword ptr [rsi+8],rdi
  00000001412F0710: 75 21              jne         00000001412F0733
  00000001412F0712: 48 8B CD           mov         rcx,rbp
  00000001412F0715: E8 F6 DC FF FF     call        00000001412EE410
  00000001412F071A: 4C 8B 5D 30        mov         r11,qword ptr [rbp+30h]
  00000001412F071E: 49 8B 03           mov         rax,qword ptr [r11]
  00000001412F0721: 49 89 44 24 08     mov         qword ptr [r12+8],rax
  00000001412F0726: 48 8B 45 00        mov         rax,qword ptr [rbp]
  00000001412F072A: 49 89 04 24        mov         qword ptr [r12],rax
  00000001412F072E: E9 03 01 00 00     jmp         00000001412F0836
  00000001412F0733: 48 8B 03           mov         rax,qword ptr [rbx]
  00000001412F0736: 48 85 C0           test        rax,rax
  00000001412F0739: 74 05              je          00000001412F0740
  00000001412F073B: 48 3B 06           cmp         rax,qword ptr [rsi]
  00000001412F073E: 74 05              je          00000001412F0745
  00000001412F0740: E8 AB FE 17 00     call        00000001414705F0
  00000001412F0745: 48 8B 46 08        mov         rax,qword ptr [rsi+8]
  00000001412F0749: 48 39 43 08        cmp         qword ptr [rbx+8],rax
  00000001412F074D: 48 8B 03           mov         rax,qword ptr [rbx]
  00000001412F0750: 0F 84 A8 00 00 00  je          00000001412F07FE
  00000001412F0756: 48 83 3B 00        cmp         qword ptr [rbx],0
  00000001412F075A: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001412F075F: 48 89 01           mov         qword ptr [rcx],rax
  00000001412F0762: 48 8B 43 08        mov         rax,qword ptr [rbx+8]
  00000001412F0766: 48 89 41 08        mov         qword ptr [rcx+8],rax
  00000001412F076A: 75 05              jne         00000001412F0771
  00000001412F076C: E8 7F FE 17 00     call        00000001414705F0
  00000001412F0771: 48 8B 43 08        mov         rax,qword ptr [rbx+8]
  00000001412F0775: 80 78 29 00        cmp         byte ptr [rax+29h],0
  00000001412F0779: 74 07              je          00000001412F0782
  00000001412F077B: E8 70 FE 17 00     call        00000001414705F0
  00000001412F0780: EB 5A              jmp         00000001412F07DC
  00000001412F0782: 48 8B 48 10        mov         rcx,qword ptr [rax+10h]
  00000001412F0786: 80 79 29 00        cmp         byte ptr [rcx+29h],0
  00000001412F078A: 75 22              jne         00000001412F07AE
  00000001412F078C: 48 8B 01           mov         rax,qword ptr [rcx]
  00000001412F078F: 80 78 29 00        cmp         byte ptr [rax+29h],0
  00000001412F0793: 75 43              jne         00000001412F07D8
  00000001412F0795: 66 66 66 0F 1F 84  nop         word ptr [rax+rax+0000000000000000h]
                    00 00 00 00 00
  00000001412F07A0: 48 8B C8           mov         rcx,rax
  00000001412F07A3: 48 8B 00           mov         rax,qword ptr [rax]
  00000001412F07A6: 80 78 29 00        cmp         byte ptr [rax+29h],0
  00000001412F07AA: 74 F4              je          00000001412F07A0
  00000001412F07AC: EB 2A              jmp         00000001412F07D8
  00000001412F07AE: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  00000001412F07B2: 80 79 29 00        cmp         byte ptr [rcx+29h],0
  00000001412F07B6: 75 20              jne         00000001412F07D8
  00000001412F07B8: 0F 1F 84 00 00 00  nop         dword ptr [rax+rax+0000000000000000h]
                    00 00
  00000001412F07C0: 48 8B 41 10        mov         rax,qword ptr [rcx+10h]
  00000001412F07C4: 48 39 43 08        cmp         qword ptr [rbx+8],rax
  00000001412F07C8: 75 0E              jne         00000001412F07D8
  00000001412F07CA: 48 89 4B 08        mov         qword ptr [rbx+8],rcx
  00000001412F07CE: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  00000001412F07D2: 80 79 29 00        cmp         byte ptr [rcx+29h],0
  00000001412F07D6: 74 E8              je          00000001412F07C0
  00000001412F07D8: 48 89 4B 08        mov         qword ptr [rbx+8],rcx
  00000001412F07DC: 0F 28 44 24 20     movaps      xmm0,xmmword ptr [rsp+20h]
  00000001412F07E1: 4C 8D 44 24 30     lea         r8,[rsp+30h]
  00000001412F07E6: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  00000001412F07EB: 48 8B CD           mov         rcx,rbp
  00000001412F07EE: 66 0F 7F 44 24 30  movdqa      xmmword ptr [rsp+30h],xmm0
  00000001412F07F4: E8 E7 F5 FF FF     call        00000001412EFDE0
  00000001412F07F9: E9 35 FF FF FF     jmp         00000001412F0733
  00000001412F07FE: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412F0803: 48 89 01           mov         qword ptr [rcx],rax
  00000001412F0806: 48 8B 43 08        mov         rax,qword ptr [rbx+8]
  00000001412F080A: 48 89 41 08        mov         qword ptr [rcx+8],rax
  00000001412F080E: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  00000001412F0813: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001412F0818: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001412F081D: 48 8B 45 00        mov         rax,qword ptr [rbp]
  00000001412F0821: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001412F0826: 48 8B 01           mov         rax,qword ptr [rcx]
  00000001412F0829: 49 89 04 24        mov         qword ptr [r12],rax
  00000001412F082D: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  00000001412F0831: 49 89 44 24 08     mov         qword ptr [r12+8],rax
  00000001412F0836: 48 8B 5C 24 60     mov         rbx,qword ptr [rsp+60h]
  00000001412F083B: 48 8B 6C 24 68     mov         rbp,qword ptr [rsp+68h]
  00000001412F0840: 48 8B 74 24 70     mov         rsi,qword ptr [rsp+70h]
  00000001412F0845: 48 8B 7C 24 78     mov         rdi,qword ptr [rsp+78h]
  00000001412F084A: 49 8B C4           mov         rax,r12
  00000001412F084D: 48 83 C4 50        add         rsp,50h
  00000001412F0851: 41 5C              pop         r12
  00000001412F0853: C3                 ret

  Summary

     1E02000 .text
