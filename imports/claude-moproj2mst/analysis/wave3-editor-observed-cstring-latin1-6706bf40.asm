Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006706BF40: 40 53              push        rbx
  000000006706BF42: 57                 push        rdi
  000000006706BF43: 48 83 EC 28        sub         rsp,28h
  000000006706BF47: 8B FA              mov         edi,edx
  000000006706BF49: 48 8B D9           mov         rbx,rcx
  000000006706BF4C: 48 85 C9           test        rcx,rcx
  000000006706BF4F: 75 16              jne         000000006706BF67
  000000006706BF51: F0 83 05 D7 4B 28  lock add    dword ptr [00000000672F0B30h],1
                    00 01
  000000006706BF59: 48 8D 05 D0 4B 28  lea         rax,[00000000672F0B30h]
                    00
  000000006706BF60: 48 83 C4 28        add         rsp,28h
  000000006706BF64: 5F                 pop         rdi
  000000006706BF65: 5B                 pop         rbx
  000000006706BF66: C3                 ret
  000000006706BF67: 85 D2              test        edx,edx
  000000006706BF69: 74 09              je          000000006706BF74
  000000006706BF6B: 80 39 00           cmp         byte ptr [rcx],0
  000000006706BF6E: 75 1A              jne         000000006706BF8A
  000000006706BF70: 85 D2              test        edx,edx
  000000006706BF72: 79 2C              jns         000000006706BFA0
  000000006706BF74: F0 83 05 D4 4B 28  lock add    dword ptr [00000000672F0B50h],1
                    00 01
  000000006706BF7C: 48 8D 05 CD 4B 28  lea         rax,[00000000672F0B50h]
                    00
  000000006706BF83: 48 83 C4 28        add         rsp,28h
  000000006706BF87: 5F                 pop         rdi
  000000006706BF88: 5B                 pop         rbx
  000000006706BF89: C3                 ret
  000000006706BF8A: 85 D2              test        edx,edx
  000000006706BF8C: 79 12              jns         000000006706BFA0
  000000006706BF8E: 48 83 C9 FF        or          rcx,0FFFFFFFFFFFFFFFFh
  000000006706BF92: 33 C0              xor         eax,eax
  000000006706BF94: 48 8B FB           mov         rdi,rbx
  000000006706BF97: F2 AE              repne scas  byte ptr [rdi]
  000000006706BF99: 48 F7 D1           not         rcx
  000000006706BF9C: 48 8D 79 FF        lea         rdi,[rcx-1]

  Summary

      1A7000 .text
