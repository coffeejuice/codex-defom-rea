Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006704C190: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  000000006704C195: 53                 push        rbx
  000000006704C196: 55                 push        rbp
  000000006704C197: 56                 push        rsi
  000000006704C198: 57                 push        rdi
  000000006704C199: 41 55              push        r13
  000000006704C19B: 41 56              push        r14
  000000006704C19D: 48 83 EC 38        sub         rsp,38h
  000000006704C1A1: 48 8B 02           mov         rax,qword ptr [rdx]
  000000006704C1A4: 33 DB              xor         ebx,ebx
  000000006704C1A6: 49 8B F1           mov         rsi,r9
  000000006704C1A9: 48 63 68 08        movsxd      rbp,dword ptr [rax+8]
  000000006704C1AD: 4C 8B 68 10        mov         r13,qword ptr [rax+10h]
  000000006704C1B1: 41 8B F8           mov         edi,r8d
  000000006704C1B4: 4C 8B F1           mov         r14,rcx
  000000006704C1B7: 85 ED              test        ebp,ebp
  000000006704C1B9: 7E 1C              jle         000000006704C1D7
  000000006704C1BB: 0F 1F 44 00 00     nop         dword ptr [rax+rax]
  000000006704C1C0: 48 63 C3           movsxd      rax,ebx
  000000006704C1C3: 49 8D 4C 45 00     lea         rcx,[r13+rax*2]
  000000006704C1C8: E8 B3 DA 01 00     call        0000000067069C80
  000000006704C1CD: 84 C0              test        al,al
  000000006704C1CF: 74 06              je          000000006704C1D7
  000000006704C1D1: FF C3              inc         ebx
  000000006704C1D3: 3B DD              cmp         ebx,ebp
  000000006704C1D5: 7C E9              jl          000000006704C1C0
  000000006704C1D7: 3B DD              cmp         ebx,ebp
  000000006704C1D9: 75 0F              jne         000000006704C1EA
  000000006704C1DB: 32 C0              xor         al,al
  000000006704C1DD: 48 83 C4 38        add         rsp,38h
  000000006704C1E1: 41 5E              pop         r14
  000000006704C1E3: 41 5D              pop         r13
  000000006704C1E5: 5F                 pop         rdi
  000000006704C1E6: 5E                 pop         rsi
  000000006704C1E7: 5D                 pop         rbp
  000000006704C1E8: 5B                 pop         rbx
  000000006704C1E9: C3                 ret

  Summary

      1A7000 .text
