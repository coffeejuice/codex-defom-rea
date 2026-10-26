Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006704C2D0: 48 63 C3           movsxd      rax,ebx
  000000006704C2D3: 49 8D 4C 45 00     lea         rcx,[r13+rax*2]
  000000006704C2D8: E8 A3 D9 01 00     call        0000000067069C80
  000000006704C2DD: 84 C0              test        al,al
  000000006704C2DF: 0F 84 F6 FE FF FF  je          000000006704C1DB
  000000006704C2E5: FF C3              inc         ebx
  000000006704C2E7: 3B DD              cmp         ebx,ebp
  000000006704C2E9: 7C E5              jl          000000006704C2D0
  000000006704C2EB: 8B 56 04           mov         edx,dword ptr [rsi+4]
  000000006704C2EE: 3B 16              cmp         edx,dword ptr [rsi]
  000000006704C2F0: 75 0C              jne         000000006704C2FE
  000000006704C2F2: 44 8D 04 12        lea         r8d,[rdx+rdx]
  000000006704C2F6: 48 8B CE           mov         rcx,rsi
  000000006704C2F9: E8 32 88 FB FF     call        0000000067004B30
  000000006704C2FE: 48 63 4E 04        movsxd      rcx,dword ptr [rsi+4]
  000000006704C302: 8D 41 01           lea         eax,[rcx+1]
  000000006704C305: 89 46 04           mov         dword ptr [rsi+4],eax
  000000006704C308: 48 8B 46 08        mov         rax,qword ptr [rsi+8]
  000000006704C30C: C6 04 01 00        mov         byte ptr [rcx+rax],0
  000000006704C310: 83 FF 01           cmp         edi,1
  000000006704C313: 75 10              jne         000000006704C325
  000000006704C315: 48 8B CE           mov         rcx,rsi
  000000006704C318: E8 53 87 00 00     call        0000000067054A70
  000000006704C31D: 84 C0              test        al,al
  000000006704C31F: 0F 84 B6 FE FF FF  je          000000006704C1DB
  000000006704C325: B0 01              mov         al,1
  000000006704C327: 48 83 C4 38        add         rsp,38h
  000000006704C32B: 41 5E              pop         r14
  000000006704C32D: 41 5D              pop         r13
  000000006704C32F: 5F                 pop         rdi
  000000006704C330: 5E                 pop         rsi
  000000006704C331: 5D                 pop         rbp
  000000006704C332: 5B                 pop         rbx
  000000006704C333: C3                 ret

  Summary

      1A7000 .text
