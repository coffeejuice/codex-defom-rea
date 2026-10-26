Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065020BE0: 40 57              push        rdi
  0000000065020BE2: 41 54              push        r12
  0000000065020BE4: 41 55              push        r13
  0000000065020BE6: 41 56              push        r14
  0000000065020BE8: 41 57              push        r15
  0000000065020BEA: 48 83 EC 30        sub         rsp,30h
  0000000065020BEE: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000065020BF7: 48 89 5C 24 60     mov         qword ptr [rsp+60h],rbx
  0000000065020BFC: 48 89 6C 24 70     mov         qword ptr [rsp+70h],rbp
  0000000065020C01: 48 89 74 24 78     mov         qword ptr [rsp+78h],rsi
  0000000065020C06: 4C 8B F2           mov         r14,rdx
  0000000065020C09: 4C 8B E1           mov         r12,rcx
  0000000065020C0C: C7 44 24 68 00 00  mov         dword ptr [rsp+68h],0
                    00 00
  0000000065020C14: E8 77 F1 FF FF     call        000000006501FD90
  0000000065020C19: 48 8B E8           mov         rbp,rax
  0000000065020C1C: 4D 85 F6           test        r14,r14
  0000000065020C1F: 0F 84 4D 01 00 00  je          0000000065020D72
  0000000065020C25: 48 85 C0           test        rax,rax
  0000000065020C28: 0F 84 44 01 00 00  je          0000000065020D72
  0000000065020C2E: 4C 8B 00           mov         r8,qword ptr [rax]
  0000000065020C31: 41 83 78 14 00     cmp         dword ptr [r8+14h],0
  0000000065020C36: 0F 84 36 01 00 00  je          0000000065020D72
  0000000065020C3C: 49 8B 16           mov         rdx,qword ptr [r14]
  0000000065020C3F: 49 8B CE           mov         rcx,r14
  0000000065020C42: FF 12              call        qword ptr [rdx]
  0000000065020C44: 48 8B C8           mov         rcx,rax
  0000000065020C47: FF 15 1B 61 67 00  call        qword ptr [0000000065696D68h]
  0000000065020C4D: 48 8B D0           mov         rdx,rax
  0000000065020C50: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  0000000065020C55: FF 15 CD 60 67 00  call        qword ptr [0000000065696D28h]
  0000000065020C5B: 90                 nop
  0000000065020C5C: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  0000000065020C61: FF 15 79 5F 67 00  call        qword ptr [0000000065696BE0h]
  0000000065020C67: 44 8B F8           mov         r15d,eax
  0000000065020C6A: 4C 8B 4D 00        mov         r9,qword ptr [rbp]
  0000000065020C6E: 41 8B 49 20        mov         ecx,dword ptr [r9+20h]
  0000000065020C72: 85 C9              test        ecx,ecx
  0000000065020C74: 0F 84 7F 00 00 00  je          0000000065020CF9
  0000000065020C7A: 33 D2              xor         edx,edx
  0000000065020C7C: F7 F1              div         eax,ecx
  0000000065020C7E: 49 8B 49 08        mov         rcx,qword ptr [r9+8]
  0000000065020C82: 48 8D 34 D1        lea         rsi,[rcx+rdx*8]
  0000000065020C86: 48 8B 0E           mov         rcx,qword ptr [rsi]
  0000000065020C89: 49 3B C9           cmp         rcx,r9
  0000000065020C8C: 74 6E              je          0000000065020CFC
  0000000065020C8E: 66 90              nop
  0000000065020C90: 44 3B 79 08        cmp         r15d,dword ptr [rcx+8]
  0000000065020C94: 75 55              jne         0000000065020CEB
  0000000065020C96: 4C 8D 69 10        lea         r13,[rcx+10h]
  0000000065020C9A: 49 8B CD           mov         rcx,r13
  0000000065020C9D: FF 15 1D 5E 67 00  call        qword ptr [0000000065696AC0h]
  0000000065020CA3: 8B D8              mov         ebx,eax
  0000000065020CA5: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  0000000065020CAA: FF 15 10 5E 67 00  call        qword ptr [0000000065696AC0h]
  0000000065020CB0: 3B C3              cmp         eax,ebx
  0000000065020CB2: 75 37              jne         0000000065020CEB
  0000000065020CB4: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  0000000065020CB9: FF 15 01 5E 67 00  call        qword ptr [0000000065696AC0h]
  0000000065020CBF: 48 63 F8           movsxd      rdi,eax
  0000000065020CC2: 49 8B CD           mov         rcx,r13
  0000000065020CC5: FF 15 8D 5D 67 00  call        qword ptr [0000000065696A58h]
  0000000065020CCB: 48 8B D8           mov         rbx,rax
  0000000065020CCE: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  0000000065020CD3: FF 15 7F 5D 67 00  call        qword ptr [0000000065696A58h]
  0000000065020CD9: 48 8B C8           mov         rcx,rax
  0000000065020CDC: 4C 8B C7           mov         r8,rdi
  0000000065020CDF: 48 8B D3           mov         rdx,rbx
  0000000065020CE2: E8 97 04 5C 00     call        00000000655E117E
  0000000065020CE7: 85 C0              test        eax,eax
  0000000065020CE9: 74 11              je          0000000065020CFC
  0000000065020CEB: 48 8B 36           mov         rsi,qword ptr [rsi]
  0000000065020CEE: 48 8B 0E           mov         rcx,qword ptr [rsi]
  0000000065020CF1: 48 3B 4D 00        cmp         rcx,qword ptr [rbp]
  0000000065020CF5: 75 99              jne         0000000065020C90
  0000000065020CF7: EB 03              jmp         0000000065020CFC
  0000000065020CF9: 48 8B F5           mov         rsi,rbp
  0000000065020CFC: 48 8B 1E           mov         rbx,qword ptr [rsi]
  0000000065020CFF: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  0000000065020D04: FF 15 46 5D 67 00  call        qword ptr [0000000065696A50h]
  0000000065020D0A: 48 8B 4D 00        mov         rcx,qword ptr [rbp]
  0000000065020D0E: 48 3B D9           cmp         rbx,rcx
  0000000065020D11: 74 11              je          0000000065020D24
  0000000065020D13: 48 8D 53 18        lea         rdx,[rbx+18h]
  0000000065020D17: 49 8B CC           mov         rcx,r12
  0000000065020D1A: E8 91 9B 02 00     call        000000006504A8B0
  0000000065020D1F: 49 8B C4           mov         rax,r12
  0000000065020D22: EB 59              jmp         0000000065020D7D
  0000000065020D24: FF 15 6E 5E 67 00  call        qword ptr [0000000065696B98h]
  0000000065020D2A: 48 8B D8           mov         rbx,rax
  0000000065020D2D: 48 3B 45 00        cmp         rax,qword ptr [rbp]
  0000000065020D31: 74 3F              je          0000000065020D72
  0000000065020D33: 48 8D 4B 10        lea         rcx,[rbx+10h]
  0000000065020D37: FF 15 BB 5F 67 00  call        qword ptr [0000000065696CF8h]
  0000000065020D3D: 48 8B D0           mov         rdx,rax
  0000000065020D40: 49 8B CE           mov         rcx,r14
  0000000065020D43: FF 15 17 60 67 00  call        qword ptr [0000000065696D60h]
  0000000065020D49: 84 C0              test        al,al
  0000000065020D4B: 75 14              jne         0000000065020D61
  0000000065020D4D: 48 8B CB           mov         rcx,rbx
  0000000065020D50: FF 15 F2 5D 67 00  call        qword ptr [0000000065696B48h]
  0000000065020D56: 48 8B D8           mov         rbx,rax
  0000000065020D59: 48 3B 45 00        cmp         rax,qword ptr [rbp]
  0000000065020D5D: 75 D4              jne         0000000065020D33
  0000000065020D5F: EB 11              jmp         0000000065020D72
  0000000065020D61: 48 8D 53 18        lea         rdx,[rbx+18h]
  0000000065020D65: 49 8B CC           mov         rcx,r12
  0000000065020D68: E8 43 9B 02 00     call        000000006504A8B0
  0000000065020D6D: 49 8B C4           mov         rax,r12
  0000000065020D70: EB 0B              jmp         0000000065020D7D
  0000000065020D72: 49 8B CC           mov         rcx,r12
  0000000065020D75: E8 F6 61 FF FF     call        0000000065016F70
  0000000065020D7A: 49 8B C4           mov         rax,r12
  0000000065020D7D: 48 8B 5C 24 60     mov         rbx,qword ptr [rsp+60h]
  0000000065020D82: 48 8B 6C 24 70     mov         rbp,qword ptr [rsp+70h]
  0000000065020D87: 48 8B 74 24 78     mov         rsi,qword ptr [rsp+78h]
  0000000065020D8C: 48 83 C4 30        add         rsp,30h
  0000000065020D90: 41 5F              pop         r15
  0000000065020D92: 41 5E              pop         r14
  0000000065020D94: 41 5D              pop         r13
  0000000065020D96: 41 5C              pop         r12
  0000000065020D98: 5F                 pop         rdi
  0000000065020D99: C3                 ret

  Summary

      695000 .text
