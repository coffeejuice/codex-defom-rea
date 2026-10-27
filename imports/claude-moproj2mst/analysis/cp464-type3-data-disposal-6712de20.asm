Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006712DE20: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000006712DE25: 53                 push        rbx
  000000006712DE26: 56                 push        rsi
  000000006712DE27: 57                 push        rdi
  000000006712DE28: 41 54              push        r12
  000000006712DE2A: 41 55              push        r13
  000000006712DE2C: 41 56              push        r14
  000000006712DE2E: 41 57              push        r15
  000000006712DE30: 48 83 EC 30        sub         rsp,30h
  000000006712DE34: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006712DE3D: 4C 8B E9           mov         r13,rcx
  000000006712DE40: 45 33 F6           xor         r14d,r14d
  000000006712DE43: 4C 89 74 24 78     mov         qword ptr [rsp+78h],r14
  000000006712DE48: 4C 89 B4 24 80 00  mov         qword ptr [rsp+0000000000000080h],r14
                    00 00
  000000006712DE50: E8 6B F8 FF FF     call        000000006712D6C0
  000000006712DE55: 4C 8B E0           mov         r12,rax
  000000006712DE58: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  000000006712DE5D: E8 AE CE FF FF     call        000000006712AD10
  000000006712DE62: 48 8B D8           mov         rbx,rax
  000000006712DE65: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  000000006712DE6D: EB 15              jmp         000000006712DE84
  000000006712DE6F: 45 33 F6           xor         r14d,r14d
  000000006712DE72: 4C 8B 6C 24 70     mov         r13,qword ptr [rsp+70h]
  000000006712DE77: 4C 8B 64 24 78     mov         r12,qword ptr [rsp+78h]
  000000006712DE7C: 48 8B 9C 24 80 00  mov         rbx,qword ptr [rsp+0000000000000080h]
                    00 00
  000000006712DE84: 4D 85 E4           test        r12,r12
  000000006712DE87: 0F 84 8D 01 00 00  je          000000006712E01A
  000000006712DE8D: 49 8B 04 24        mov         rax,qword ptr [r12]
  000000006712DE91: 83 78 14 00        cmp         dword ptr [rax+14h],0
  000000006712DE95: 0F 84 7F 01 00 00  je          000000006712E01A
  000000006712DE9B: 48 85 DB           test        rbx,rbx
  000000006712DE9E: 74 40              je          000000006712DEE0
  000000006712DEA0: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000006712DEA3: F6 41 04 01        test        byte ptr [rcx+4],1
  000000006712DEA7: 74 10              je          000000006712DEB9
  000000006712DEA9: 48 8B CB           mov         rcx,rbx
  000000006712DEAC: E8 FF 7E EF FF     call        0000000067025DB0
  000000006712DEB1: 41 BF 01 00 00 00  mov         r15d,1
  000000006712DEB7: EB 1C              jmp         000000006712DED5
  000000006712DEB9: 33 C0              xor         eax,eax
  000000006712DEBB: 41 BF 01 00 00 00  mov         r15d,1
  000000006712DEC1: F0 44 0F B1 39     lock cmpxchg dword ptr [rcx],r15d
  000000006712DEC6: 0F 94 C1           sete        cl
  000000006712DEC9: 84 C9              test        cl,cl
  000000006712DECB: 75 08              jne         000000006712DED5
  000000006712DECD: 48 8B CB           mov         rcx,rbx
  000000006712DED0: E8 DB 7C EF FF     call        0000000067025BB0
  000000006712DED5: 48 83 CB 01        or          rbx,1
  000000006712DED9: 48 89 5C 24 78     mov         qword ptr [rsp+78h],rbx
  000000006712DEDE: EB 0B              jmp         000000006712DEEB
  000000006712DEE0: 4C 89 74 24 78     mov         qword ptr [rsp+78h],r14
  000000006712DEE5: 41 BF 01 00 00 00  mov         r15d,1
  000000006712DEEB: 49 8B 04 24        mov         rax,qword ptr [r12]
  000000006712DEEF: 8B 40 10           mov         eax,dword ptr [rax+10h]
  000000006712DEF2: 83 F8 01           cmp         eax,1
  000000006712DEF5: 74 08              je          000000006712DEFF
  000000006712DEF7: 49 8B CC           mov         rcx,r12
  000000006712DEFA: E8 C1 C8 FF FF     call        000000006712A7C0
  000000006712DEFF: 4D 8B C5           mov         r8,r13
  000000006712DF02: 49 C1 E8 1F        shr         r8,1Fh
  000000006712DF06: 45 33 C5           xor         r8d,r13d
  000000006712DF09: 4D 8B 0C 24        mov         r9,qword ptr [r12]
  000000006712DF0D: 41 8B 49 20        mov         ecx,dword ptr [r9+20h]
  000000006712DF11: 85 C9              test        ecx,ecx
  000000006712DF13: 74 34              je          000000006712DF49
  000000006712DF15: 33 D2              xor         edx,edx
  000000006712DF17: 41 8B C0           mov         eax,r8d
  000000006712DF1A: F7 F1              div         eax,ecx
  000000006712DF1C: 49 8B 41 08        mov         rax,qword ptr [r9+8]
  000000006712DF20: 48 8D 1C D0        lea         rbx,[rax+rdx*8]
  000000006712DF24: 48 8B 03           mov         rax,qword ptr [rbx]
  000000006712DF27: 49 3B C1           cmp         rax,r9
  000000006712DF2A: 74 20              je          000000006712DF4C
  000000006712DF2C: 0F 1F 40 00        nop         dword ptr [rax]
  000000006712DF30: 44 3B 40 08        cmp         r8d,dword ptr [rax+8]
  000000006712DF34: 75 06              jne         000000006712DF3C
  000000006712DF36: 4C 3B 68 10        cmp         r13,qword ptr [rax+10h]
  000000006712DF3A: 74 10              je          000000006712DF4C
  000000006712DF3C: 48 8B D8           mov         rbx,rax
  000000006712DF3F: 48 8B 00           mov         rax,qword ptr [rax]
  000000006712DF42: 49 3B C1           cmp         rax,r9
  000000006712DF45: 75 E9              jne         000000006712DF30
  000000006712DF47: EB 03              jmp         000000006712DF4C
  000000006712DF49: 49 8B DC           mov         rbx,r12
  000000006712DF4C: 48 8B 1B           mov         rbx,qword ptr [rbx]
  000000006712DF4F: 41 8B 41 10        mov         eax,dword ptr [r9+10h]
  000000006712DF53: 83 F8 01           cmp         eax,1
  000000006712DF56: 74 08              je          000000006712DF60
  000000006712DF58: 49 8B CC           mov         rcx,r12
  000000006712DF5B: E8 60 C8 FF FF     call        000000006712A7C0
  000000006712DF60: 49 8B 3C 24        mov         rdi,qword ptr [r12]
  000000006712DF64: 4C 39 6B 10        cmp         qword ptr [rbx+10h],r13
  000000006712DF68: 75 7A              jne         000000006712DFE4
  000000006712DF6A: 66 0F 1F 44 00 00  nop         word ptr [rax+rax]
  000000006712DF70: 48 3B DF           cmp         rbx,rdi
  000000006712DF73: 74 6F              je          000000006712DFE4
  000000006712DF75: 48 8B 43 18        mov         rax,qword ptr [rbx+18h]
  000000006712DF79: 4C 89 30           mov         qword ptr [rax],r14
  000000006712DF7C: 49 3B 1C 24        cmp         rbx,qword ptr [r12]
  000000006712DF80: 75 0A              jne         000000006712DF8C
  000000006712DF82: 48 89 9C 24 80 00  mov         qword ptr [rsp+0000000000000080h],rbx
                    00 00
  000000006712DF8A: EB 52              jmp         000000006712DFDE
  000000006712DF8C: 48 8B CB           mov         rcx,rbx
  000000006712DF8F: E8 1C 9D F1 FF     call        0000000067047CB0
  000000006712DF94: 48 8B F0           mov         rsi,rax
  000000006712DF97: 4D 8B 04 24        mov         r8,qword ptr [r12]
  000000006712DF9B: 33 D2              xor         edx,edx
  000000006712DF9D: 8B 43 08           mov         eax,dword ptr [rbx+8]
  000000006712DFA0: 41 F7 70 20        div         eax,dword ptr [r8+20h]
  000000006712DFA4: 49 8B 40 08        mov         rax,qword ptr [r8+8]
  000000006712DFA8: 48 8D 14 D0        lea         rdx,[rax+rdx*8]
  000000006712DFAC: 48 39 1A           cmp         qword ptr [rdx],rbx
  000000006712DFAF: 74 08              je          000000006712DFB9
  000000006712DFB1: 48 8B 12           mov         rdx,qword ptr [rdx]
  000000006712DFB4: 48 39 1A           cmp         qword ptr [rdx],rbx
  000000006712DFB7: 75 F8              jne         000000006712DFB1
  000000006712DFB9: 48 8B 03           mov         rax,qword ptr [rbx]
  000000006712DFBC: 48 89 02           mov         qword ptr [rdx],rax
  000000006712DFBF: 48 8B D3           mov         rdx,rbx
  000000006712DFC2: 49 8B 0C 24        mov         rcx,qword ptr [r12]
  000000006712DFC6: E8 C5 96 F1 FF     call        0000000067047690
  000000006712DFCB: 4D 8B 1C 24        mov         r11,qword ptr [r12]
  000000006712DFCF: 41 FF 4B 14        dec         dword ptr [r11+14h]
  000000006712DFD3: 48 8B DE           mov         rbx,rsi
  000000006712DFD6: 48 89 9C 24 80 00  mov         qword ptr [rsp+0000000000000080h],rbx
                    00 00
  000000006712DFDE: 4C 39 6B 10        cmp         qword ptr [rbx+10h],r13
  000000006712DFE2: 74 8C              je          000000006712DF70
  000000006712DFE4: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  000000006712DFE9: F6 C1 01           test        cl,1
  000000006712DFEC: 74 2C              je          000000006712E01A
  000000006712DFEE: 48 83 E1 FE        and         rcx,0FFFFFFFFFFFFFFFEh
  000000006712DFF2: 48 89 4C 24 78     mov         qword ptr [rsp+78h],rcx
  000000006712DFF7: 48 83 E1 FE        and         rcx,0FFFFFFFFFFFFFFFEh
  000000006712DFFB: 48 8B 11           mov         rdx,qword ptr [rcx]
  000000006712DFFE: F6 42 04 01        test        byte ptr [rdx+4],1
  000000006712E002: 74 07              je          000000006712E00B
  000000006712E004: E8 27 7B EF FF     call        0000000067025B30
  000000006712E009: EB 0F              jmp         000000006712E01A
  000000006712E00B: 41 8B C7           mov         eax,r15d
  000000006712E00E: F0 44 0F B1 32     lock cmpxchg dword ptr [rdx],r14d
  000000006712E013: 74 05              je          000000006712E01A
  000000006712E015: E8 56 7B EF FF     call        0000000067025B70
  000000006712E01A: 48 83 C4 30        add         rsp,30h
  000000006712E01E: 41 5F              pop         r15
  000000006712E020: 41 5E              pop         r14
  000000006712E022: 41 5D              pop         r13
  000000006712E024: 41 5C              pop         r12
  000000006712E026: 5F                 pop         rdi
  000000006712E027: 5E                 pop         rsi
  000000006712E028: 5B                 pop         rbx
  000000006712E029: C3                 ret

  Summary

      1A7000 .text
