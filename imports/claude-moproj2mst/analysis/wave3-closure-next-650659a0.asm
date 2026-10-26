Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000650659A0: 40 55              push        rbp
  00000000650659A2: 56                 push        rsi
  00000000650659A3: 57                 push        rdi
  00000000650659A4: 48 83 EC 40        sub         rsp,40h
  00000000650659A8: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000650659B1: 48 89 5C 24 70     mov         qword ptr [rsp+70h],rbx
  00000000650659B6: 48 8B F2           mov         rsi,rdx
  00000000650659B9: 48 8B E9           mov         rbp,rcx
  00000000650659BC: 48 8B 79 08        mov         rdi,qword ptr [rcx+8]
  00000000650659C0: 48 89 7C 24 60     mov         qword ptr [rsp+60h],rdi
  00000000650659C5: 48 8B 05 3C 3D 96  mov         rax,qword ptr [00000000659C9708h]
                    00
  00000000650659CC: 48 85 C0           test        rax,rax
  00000000650659CF: 74 13              je          00000000650659E4
  00000000650659D1: 48 3B C7           cmp         rax,rdi
  00000000650659D4: 74 0E              je          00000000650659E4
  00000000650659D6: 48 8B 47 20        mov         rax,qword ptr [rdi+20h]
  00000000650659DA: 80 78 0C 11        cmp         byte ptr [rax+0Ch],11h
  00000000650659DE: 75 04              jne         00000000650659E4
  00000000650659E0: B3 01              mov         bl,1
  00000000650659E2: EB 02              jmp         00000000650659E6
  00000000650659E4: 32 DB              xor         bl,bl
  00000000650659E6: 48 8B 0D 4B 3F 96  mov         rcx,qword ptr [00000000659C9938h]
                    00
  00000000650659ED: 48 85 C9           test        rcx,rcx
  00000000650659F0: 74 16              je          0000000065065A08
  00000000650659F2: 48 8D 95 48 01 00  lea         rdx,[rbp+0000000000000148h]
                    00
  00000000650659F9: 48 83 3A 00        cmp         qword ptr [rdx],0
  00000000650659FD: 74 09              je          0000000065065A08
  00000000650659FF: 84 DB              test        bl,bl
  0000000065065A01: 75 05              jne         0000000065065A08
  0000000065065A03: E8 08 25 01 00     call        0000000065077F10
  0000000065065A08: 4C 8D 85 48 01 00  lea         r8,[rbp+0000000000000148h]
                    00
  0000000065065A0F: 49 8B 28           mov         rbp,qword ptr [r8]
  0000000065065A12: 49 89 30           mov         qword ptr [r8],rsi
  0000000065065A15: 48 8B 0D 1C 3F 96  mov         rcx,qword ptr [00000000659C9938h]
                    00
  0000000065065A1C: 48 85 C9           test        rcx,rcx
  0000000065065A1F: 74 18              je          0000000065065A39
  0000000065065A21: 48 85 F6           test        rsi,rsi
  0000000065065A24: 74 13              je          0000000065065A39
  0000000065065A26: 84 DB              test        bl,bl
  0000000065065A28: 75 0F              jne         0000000065065A39
  0000000065065A2A: 4C 8D 4C 24 60     lea         r9,[rsp+60h]
  0000000065065A2F: 48 8D 54 24 68     lea         rdx,[rsp+68h]
  0000000065065A34: E8 B7 13 14 00     call        00000000651A6DF0
  0000000065065A39: 48 3B EE           cmp         rbp,rsi
  0000000065065A3C: 74 2B              je          0000000065065A69
  0000000065065A3E: BA CB 00 00 00     mov         edx,0CBh
  0000000065065A43: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  0000000065065A48: FF 15 CA 0D 63 00  call        qword ptr [0000000065696818h]
  0000000065065A4E: 90                 nop
  0000000065065A4F: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  0000000065065A54: 48 8B CF           mov         rcx,rdi
  0000000065065A57: FF 15 B3 0D 63 00  call        qword ptr [0000000065696810h]
  0000000065065A5D: 90                 nop
  0000000065065A5E: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  0000000065065A63: FF 15 9F 0D 63 00  call        qword ptr [0000000065696808h]
  0000000065065A69: 48 8B 5C 24 70     mov         rbx,qword ptr [rsp+70h]
  0000000065065A6E: 48 83 C4 40        add         rsp,40h
  0000000065065A72: 5F                 pop         rdi
  0000000065065A73: 5E                 pop         rsi
  0000000065065A74: 5D                 pop         rbp
  0000000065065A75: C3                 ret

  Summary

      695000 .text
