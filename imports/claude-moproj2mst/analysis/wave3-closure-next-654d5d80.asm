Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000654D5D80: 48 89 5C 24 20     mov         qword ptr [rsp+20h],rbx
  00000000654D5D85: 56                 push        rsi
  00000000654D5D86: 48 83 EC 20        sub         rsp,20h
  00000000654D5D8A: 48 8B F1           mov         rsi,rcx
  00000000654D5D8D: 48 8B 09           mov         rcx,qword ptr [rcx]
  00000000654D5D90: 48 8B DA           mov         rbx,rdx
  00000000654D5D93: 83 79 14 00        cmp         dword ptr [rcx+14h],0
  00000000654D5D97: 75 0D              jne         00000000654D5DA6
  00000000654D5D99: 33 C0              xor         eax,eax
  00000000654D5D9B: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  00000000654D5DA0: 48 83 C4 20        add         rsp,20h
  00000000654D5DA4: 5E                 pop         rsi
  00000000654D5DA5: C3                 ret
  00000000654D5DA6: 48 83 C1 10        add         rcx,10h
  00000000654D5DAA: BA 01 00 00 00     mov         edx,1
  00000000654D5DAF: 48 89 7C 24 38     mov         qword ptr [rsp+38h],rdi
  00000000654D5DB4: 4C 89 64 24 40     mov         qword ptr [rsp+40h],r12
  00000000654D5DB9: FF 15 B1 0C 1C 00  call        qword ptr [0000000065696A70h]
  00000000654D5DBF: 84 C0              test        al,al
  00000000654D5DC1: 74 08              je          00000000654D5DCB
  00000000654D5DC3: 48 8B CE           mov         rcx,rsi
  00000000654D5DC6: E8 C5 5F D8 FF     call        000000006525BD90
  00000000654D5DCB: 4C 8B 06           mov         r8,qword ptr [rsi]
  00000000654D5DCE: 4C 8B 13           mov         r10,qword ptr [rbx]
  00000000654D5DD1: 41 8B 48 20        mov         ecx,dword ptr [r8+20h]
  00000000654D5DD5: 45 8B 60 14        mov         r12d,dword ptr [r8+14h]
  00000000654D5DD9: 4D 8B CA           mov         r9,r10
  00000000654D5DDC: 49 C1 E9 1F        shr         r9,1Fh
  00000000654D5DE0: 45 33 CA           xor         r9d,r10d
  00000000654D5DE3: 85 C9              test        ecx,ecx
  00000000654D5DE5: 74 32              je          00000000654D5E19
  00000000654D5DE7: 33 D2              xor         edx,edx
  00000000654D5DE9: 41 8B C1           mov         eax,r9d
  00000000654D5DEC: F7 F1              div         eax,ecx
  00000000654D5DEE: 49 8B 40 08        mov         rax,qword ptr [r8+8]
  00000000654D5DF2: 48 8D 3C D0        lea         rdi,[rax+rdx*8]
  00000000654D5DF6: 48 8B 04 D0        mov         rax,qword ptr [rax+rdx*8]
  00000000654D5DFA: 49 3B C0           cmp         rax,r8
  00000000654D5DFD: 74 1D              je          00000000654D5E1C
  00000000654D5DFF: 90                 nop
  00000000654D5E00: 44 3B 48 08        cmp         r9d,dword ptr [rax+8]
  00000000654D5E04: 75 06              jne         00000000654D5E0C
  00000000654D5E06: 4C 3B 50 10        cmp         r10,qword ptr [rax+10h]
  00000000654D5E0A: 74 10              je          00000000654D5E1C
  00000000654D5E0C: 48 8B F8           mov         rdi,rax
  00000000654D5E0F: 48 8B 00           mov         rax,qword ptr [rax]
  00000000654D5E12: 49 3B C0           cmp         rax,r8
  00000000654D5E15: 75 E9              jne         00000000654D5E00
  00000000654D5E17: EB 03              jmp         00000000654D5E1C
  00000000654D5E19: 48 8B FE           mov         rdi,rsi
  00000000654D5E1C: 4C 39 07           cmp         qword ptr [rdi],r8
  00000000654D5E1F: 74 51              je          00000000654D5E72

  Summary

      695000 .text
