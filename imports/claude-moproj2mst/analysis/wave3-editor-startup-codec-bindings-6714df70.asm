Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006714DF70: 40 57              push        rdi
  000000006714DF72: 48 83 EC 30        sub         rsp,30h
  000000006714DF76: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006714DF7F: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  000000006714DF84: 48 89 6C 24 50     mov         qword ptr [rsp+50h],rbp
  000000006714DF89: 48 89 74 24 58     mov         qword ptr [rsp+58h],rsi
  000000006714DF8E: 48 8B 05 A3 64 1A  mov         rax,qword ptr [00000000672F4438h]
                    00
  000000006714DF95: 48 85 C0           test        rax,rax
  000000006714DF98: 0F 85 90 00 00 00  jne         000000006714E02E
  000000006714DF9E: E8 9D DB FF FF     call        000000006714BB40
  000000006714DFA3: 48 8B D8           mov         rbx,rax
  000000006714DFA6: 33 F6              xor         esi,esi
  000000006714DFA8: 8D 6E 01           lea         ebp,[rsi+1]
  000000006714DFAB: 48 85 C0           test        rax,rax
  000000006714DFAE: 74 32              je          000000006714DFE2
  000000006714DFB0: 48 8B 08           mov         rcx,qword ptr [rax]
  000000006714DFB3: 40 84 69 04        test        byte ptr [rcx+4],bpl
  000000006714DFB7: 74 0A              je          000000006714DFC3
  000000006714DFB9: 48 8B C8           mov         rcx,rax
  000000006714DFBC: E8 EF 7D ED FF     call        0000000067025DB0
  000000006714DFC1: EB 15              jmp         000000006714DFD8
  000000006714DFC3: 33 C0              xor         eax,eax
  000000006714DFC5: F0 0F B1 29        lock cmpxchg dword ptr [rcx],ebp
  000000006714DFC9: 0F 94 C1           sete        cl
  000000006714DFCC: 84 C9              test        cl,cl
  000000006714DFCE: 75 08              jne         000000006714DFD8
  000000006714DFD0: 48 8B CB           mov         rcx,rbx
  000000006714DFD3: E8 D8 7B ED FF     call        0000000067025BB0
  000000006714DFD8: 48 0B DD           or          rbx,rbp
  000000006714DFDB: 48 89 5C 24 40     mov         qword ptr [rsp+40h],rbx
  000000006714DFE0: EB 08              jmp         000000006714DFEA
  000000006714DFE2: 48 8B DE           mov         rbx,rsi
  000000006714DFE5: 48 89 5C 24 40     mov         qword ptr [rsp+40h],rbx
  000000006714DFEA: E8 51 E8 FF FF     call        000000006714C840
  000000006714DFEF: 48 8B 3D 42 64 1A  mov         rdi,qword ptr [00000000672F4438h]
                    00
  000000006714DFF6: F6 C3 01           test        bl,1
  000000006714DFF9: 74 30              je          000000006714E02B
  000000006714DFFB: 48 83 E3 FE        and         rbx,0FFFFFFFFFFFFFFFEh
  000000006714DFFF: 48 89 5C 24 40     mov         qword ptr [rsp+40h],rbx
  000000006714E004: 48 83 E3 FE        and         rbx,0FFFFFFFFFFFFFFFEh
  000000006714E008: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000006714E00B: F6 41 04 01        test        byte ptr [rcx+4],1
  000000006714E00F: 74 0A              je          000000006714E01B
  000000006714E011: 48 8B CB           mov         rcx,rbx
  000000006714E014: E8 17 7B ED FF     call        0000000067025B30
  000000006714E019: EB 10              jmp         000000006714E02B
  000000006714E01B: 8B C5              mov         eax,ebp
  000000006714E01D: F0 0F B1 31        lock cmpxchg dword ptr [rcx],esi
  000000006714E021: 74 08              je          000000006714E02B
  000000006714E023: 48 8B CB           mov         rcx,rbx
  000000006714E026: E8 45 7B ED FF     call        0000000067025B70
  000000006714E02B: 48 8B C7           mov         rax,rdi
  000000006714E02E: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  000000006714E033: 48 8B 6C 24 50     mov         rbp,qword ptr [rsp+50h]
  000000006714E038: 48 8B 74 24 58     mov         rsi,qword ptr [rsp+58h]
  000000006714E03D: 48 83 C4 30        add         rsp,30h
  000000006714E041: 5F                 pop         rdi
  000000006714E042: C3                 ret

  Summary

      1A7000 .text
