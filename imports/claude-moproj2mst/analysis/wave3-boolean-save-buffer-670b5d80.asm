Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670B5D80: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  00000000670B5D85: 57                 push        rdi
  00000000670B5D86: 48 83 EC 20        sub         rsp,20h
  00000000670B5D8A: 83 7A 10 00        cmp         dword ptr [rdx+10h],0
  00000000670B5D8E: 48 8B DA           mov         rbx,rdx
  00000000670B5D91: 4C 8B D1           mov         r10,rcx
  00000000670B5D94: 75 05              jne         00000000670B5D9B
  00000000670B5D96: 8B 52 0C           mov         edx,dword ptr [rdx+0Ch]
  00000000670B5D99: EB 0F              jmp         00000000670B5DAA
  00000000670B5D9B: 48 8B 0A           mov         rcx,qword ptr [rdx]
  00000000670B5D9E: 48 63 41 08        movsxd      rax,dword ptr [rcx+8]
  00000000670B5DA2: 48 8B 4C C1 18     mov         rcx,qword ptr [rcx+rax*8+18h]
  00000000670B5DA7: 8B 51 08           mov         edx,dword ptr [rcx+8]
  00000000670B5DAA: 48 8B 0B           mov         rcx,qword ptr [rbx]
  00000000670B5DAD: 4C 63 43 08        movsxd      r8,dword ptr [rbx+8]
  00000000670B5DB1: 48 63 41 08        movsxd      rax,dword ptr [rcx+8]
  00000000670B5DB5: 41 2B D0           sub         edx,r8d
  00000000670B5DB8: 39 41 0C           cmp         dword ptr [rcx+0Ch],eax
  00000000670B5DBB: 75 05              jne         00000000670B5DC2
  00000000670B5DBD: 45 33 C9           xor         r9d,r9d
  00000000670B5DC0: EB 0C              jmp         00000000670B5DCE
  00000000670B5DC2: 48 8B 4C C1 18     mov         rcx,qword ptr [rcx+rax*8+18h]
  00000000670B5DC7: 4D 8B C8           mov         r9,r8
  00000000670B5DCA: 4C 03 49 10        add         r9,qword ptr [rcx+10h]
  00000000670B5DCE: 49 8B 02           mov         rax,qword ptr [r10]
  00000000670B5DD1: 4C 63 C2           movsxd      r8,edx
  00000000670B5DD4: 49 8B CA           mov         rcx,r10
  00000000670B5DD7: 49 8B D1           mov         rdx,r9
  00000000670B5DDA: FF 90 F0 00 00 00  call        qword ptr [rax+00000000000000F0h]
  00000000670B5DE0: 48 8B F8           mov         rdi,rax
  00000000670B5DE3: 48 85 C0           test        rax,rax
  00000000670B5DE6: 7E 0D              jle         00000000670B5DF5
  00000000670B5DE8: 8B D0              mov         edx,eax
  00000000670B5DEA: 48 8B CB           mov         rcx,rbx
  00000000670B5DED: E8 2E FC FF FF     call        00000000670B5A20
  00000000670B5DF2: 48 8B C7           mov         rax,rdi
  00000000670B5DF5: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  00000000670B5DFA: 48 83 C4 20        add         rsp,20h
  00000000670B5DFE: 5F                 pop         rdi
  00000000670B5DFF: C3                 ret

  Summary

      1A7000 .text
