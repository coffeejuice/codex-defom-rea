Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065370DF0: 48 89 5C 24 10     mov         qword ptr [rsp+10h],rbx
  0000000065370DF5: 57                 push        rdi
  0000000065370DF6: 48 83 EC 30        sub         rsp,30h
  0000000065370DFA: 48 8B 79 08        mov         rdi,qword ptr [rcx+8]
  0000000065370DFE: 48 8B D9           mov         rbx,rcx
  0000000065370E01: 48 8B CF           mov         rcx,rdi
  0000000065370E04: E8 57 C8 CE FF     call        000000006505D660
  0000000065370E09: 45 33 C9           xor         r9d,r9d
  0000000065370E0C: 45 33 C0           xor         r8d,r8d
  0000000065370E0F: 4C 8B 10           mov         r10,qword ptr [rax]
  0000000065370E12: 41 8D 51 31        lea         edx,[r9+31h]
  0000000065370E16: 48 8B C8           mov         rcx,rax
  0000000065370E19: 48 C7 44 24 20 00  mov         qword ptr [rsp+20h],0
                    00 00 00
  0000000065370E22: 41 FF 92 E8 00 00  call        qword ptr [r10+00000000000000E8h]
                    00
  0000000065370E29: 48 8B CF           mov         rcx,rdi
  0000000065370E2C: 8B D0              mov         edx,eax
  0000000065370E2E: E8 2D CF CE FF     call        000000006505DD60
  0000000065370E33: 8B 93 20 02 00 00  mov         edx,dword ptr [rbx+0000000000000220h]
  0000000065370E39: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000065370E3E: C7 44 24 40 01 00  mov         dword ptr [rsp+40h],1
                    00 00
  0000000065370E46: E8 45 2E CD FF     call        0000000065043C90
  0000000065370E4B: 8B 54 24 40        mov         edx,dword ptr [rsp+40h]
  0000000065370E4F: 48 8B CF           mov         rcx,rdi
  0000000065370E52: E8 C9 CC CF FF     call        000000006506DB20
  0000000065370E57: 45 33 C0           xor         r8d,r8d
  0000000065370E5A: 48 8B CF           mov         rcx,rdi
  0000000065370E5D: 41 8D 50 44        lea         edx,[r8+44h]
  0000000065370E61: E8 7A 81 CF FF     call        0000000065068FE0
  0000000065370E66: BA 08 00 00 00     mov         edx,8
  0000000065370E6B: 48 8B CF           mov         rcx,rdi
  0000000065370E6E: E8 4D 5A CF FF     call        00000000650668C0
  0000000065370E73: BA 01 00 00 00     mov         edx,1
  0000000065370E78: 48 8B CF           mov         rcx,rdi
  0000000065370E7B: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  0000000065370E80: 48 83 C4 30        add         rsp,30h
  0000000065370E84: 5F                 pop         rdi
  0000000065370E85: E9 06 5A CF FF     jmp         0000000065066890

  Summary

      695000 .text
