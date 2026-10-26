Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000654A4EF0: 40 53              push        rbx
  00000000654A4EF2: 48 83 EC 20        sub         rsp,20h
  00000000654A4EF6: 83 3A 00           cmp         dword ptr [rdx],0
  00000000654A4EF9: 48 8B 59 08        mov         rbx,qword ptr [rcx+8]
  00000000654A4EFD: 7C 2B              jl          00000000654A4F2A
  00000000654A4EFF: 83 7A 04 00        cmp         dword ptr [rdx+4],0
  00000000654A4F03: 7C 25              jl          00000000654A4F2A
  00000000654A4F05: 48 8B 42 10        mov         rax,qword ptr [rdx+10h]
  00000000654A4F09: 48 85 C0           test        rax,rax
  00000000654A4F0C: 74 1C              je          00000000654A4F2A
  00000000654A4F0E: 48 3B 83 80 02 00  cmp         rax,qword ptr [rbx+0000000000000280h]
                    00
  00000000654A4F15: 74 13              je          00000000654A4F2A
  00000000654A4F17: 48 8D 0D E2 60 28  lea         rcx,[000000006572B000h]
                    00
  00000000654A4F1E: 48 83 C4 20        add         rsp,20h
  00000000654A4F22: 5B                 pop         rbx
  00000000654A4F23: 48 FF 25 6E 1A 1F  jmp         qword ptr [0000000065696998h]
                    00
  00000000654A4F2A: 48 8D 8B 08 03 00  lea         rcx,[rbx+0000000000000308h]
                    00
  00000000654A4F31: FF 15 01 39 1F 00  call        qword ptr [0000000065698838h]
  00000000654A4F37: 80 BB A2 03 00 00  cmp         byte ptr [rbx+00000000000003A2h],0
                    00
  00000000654A4F3E: 75 1A              jne         00000000654A4F5A
  00000000654A4F40: 4C 8B 43 08        mov         r8,qword ptr [rbx+8]
  00000000654A4F44: 48 8D 8B A4 03 00  lea         rcx,[rbx+00000000000003A4h]
                    00
  00000000654A4F4B: 33 D2              xor         edx,edx
  00000000654A4F4D: C6 83 A2 03 00 00  mov         byte ptr [rbx+00000000000003A2h],1
                    01
  00000000654A4F54: FF 15 1E 1E 1F 00  call        qword ptr [0000000065696D78h]
  00000000654A4F5A: 48 83 C4 20        add         rsp,20h
  00000000654A4F5E: 5B                 pop         rbx
  00000000654A4F5F: C3                 ret

  Summary

      695000 .text
