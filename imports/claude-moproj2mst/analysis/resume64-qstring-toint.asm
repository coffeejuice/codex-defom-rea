Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006706C280: 40 53              push        rbx
  000000006706C282: 48 83 EC 20        sub         rsp,20h
  000000006706C286: 48 8B DA           mov         rbx,rdx
  000000006706C289: E8 92 FE FF FF     call        000000006706C120
  000000006706C28E: 48 3D 00 00 00 80  cmp         rax,0FFFFFFFF80000000h
  000000006706C294: 7C 08              jl          000000006706C29E
  000000006706C296: 48 3D FF FF FF 7F  cmp         rax,7FFFFFFFh
  000000006706C29C: 7E 0A              jle         000000006706C2A8
  000000006706C29E: 48 85 DB           test        rbx,rbx
  000000006706C2A1: 74 03              je          000000006706C2A6
  000000006706C2A3: C6 03 00           mov         byte ptr [rbx],0
  000000006706C2A6: 33 C0              xor         eax,eax
  000000006706C2A8: 48 83 C4 20        add         rsp,20h
  000000006706C2AC: 5B                 pop         rbx
  000000006706C2AD: C3                 ret
  000000006706C2AE: CC                 int         3
  000000006706C2AF: CC                 int         3
  000000006706C2B0: 40 53              push        rbx
  000000006706C2B2: 48 83 EC 20        sub         rsp,20h
  000000006706C2B6: 48 8B DA           mov         rbx,rdx
  000000006706C2B9: E8 12 FF FF FF     call        000000006706C1D0
  000000006706C2BE: B9 FF FF FF FF     mov         ecx,0FFFFFFFFh
  000000006706C2C3: 48 3B C1           cmp         rax,rcx
  000000006706C2C6: 76 0A              jbe         000000006706C2D2
  000000006706C2C8: 48 85 DB           test        rbx,rbx
  000000006706C2CB: 74 03              je          000000006706C2D0
  000000006706C2CD: C6 03 00           mov         byte ptr [rbx],0
  000000006706C2D0: 33 C0              xor         eax,eax
  000000006706C2D2: 48 83 C4 20        add         rsp,20h
  000000006706C2D6: 5B                 pop         rbx
  000000006706C2D7: C3                 ret
  000000006706C2D8: CC                 int         3
  000000006706C2D9: CC                 int         3
  000000006706C2DA: CC                 int         3
  000000006706C2DB: CC                 int         3
  000000006706C2DC: CC                 int         3
  000000006706C2DD: CC                 int         3
  000000006706C2DE: CC                 int         3
  000000006706C2DF: CC                 int         3
  000000006706C2E0: 40 53              push        rbx
  000000006706C2E2: 48 83 EC 20        sub         rsp,20h
  000000006706C2E6: 48 8B DA           mov         rbx,rdx
  000000006706C2E9: E8 32 FE FF FF     call        000000006706C120
  000000006706C2EE: 3D 00 80 FF FF     cmp         eax,0FFFF8000h
  000000006706C2F3: 7C 07              jl          000000006706C2FC
  000000006706C2F5: 3D FF 7F 00 00     cmp         eax,7FFFh
  000000006706C2FA: 7E 0A              jle         000000006706C306
  000000006706C2FC: 48 85 DB           test        rbx,rbx
  000000006706C2FF: 74 03              je          000000006706C304
  000000006706C301: C6 03 00           mov         byte ptr [rbx],0
  000000006706C304: 33 C0              xor         eax,eax
  000000006706C306: 48 83 C4 20        add         rsp,20h
  000000006706C30A: 5B                 pop         rbx
  000000006706C30B: C3                 ret
  000000006706C30C: CC                 int         3
  000000006706C30D: CC                 int         3
  000000006706C30E: CC                 int         3
  000000006706C30F: CC                 int         3
  000000006706C310: 40 53              push        rbx
  000000006706C312: 48 83 EC 20        sub         rsp,20h
  000000006706C316: 48 8B DA           mov         rbx,rdx
  000000006706C319: E8 B2 FE FF FF     call        000000006706C1D0
  000000006706C31E: 3D FF FF 00 00     cmp         eax,0FFFFh
  000000006706C323: 76 0A              jbe         000000006706C32F
  000000006706C325: 48 85 DB           test        rbx,rbx
  000000006706C328: 74 03              je          000000006706C32D
  000000006706C32A: C6 03 00           mov         byte ptr [rbx],0
  000000006706C32D: 33 C0              xor         eax,eax
  000000006706C32F: 48 83 C4 20        add         rsp,20h
  000000006706C333: 5B                 pop         rbx
  000000006706C334: C3                 ret
  000000006706C335: CC                 int         3
  000000006706C336: CC                 int         3
  000000006706C337: CC                 int         3
  000000006706C338: CC                 int         3
  000000006706C339: CC                 int         3
  000000006706C33A: CC                 int         3
  000000006706C33B: CC                 int         3
  000000006706C33C: CC                 int         3
  000000006706C33D: CC                 int         3
  000000006706C33E: CC                 int         3
  000000006706C33F: CC                 int         3
  000000006706C340: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  000000006706C345: 57                 push        rdi
  000000006706C346: 48 83 EC 30        sub         rsp,30h
  000000006706C34A: 48 8B F9           mov         rdi,rcx
  000000006706C34D: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  000000006706C352: 48 8B DA           mov         rbx,rdx
  000000006706C355: E8 96 11 FE FF     call        000000006704D4F0
  000000006706C35A: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  000000006706C35F: E8 DC CF FD FF     call        0000000067049340
  000000006706C364: 4C 8D 44 24 50     lea         r8,[rsp+50h]
  000000006706C369: 45 33 C9           xor         r9d,r9d
  000000006706C36C: 48 8B C8           mov         rcx,rax
  000000006706C36F: 48 8B D7           mov         rdx,rdi
  000000006706C372: E8 C9 FF FD FF     call        000000006704C340
  000000006706C377: 80 7C 24 50 00     cmp         byte ptr [rsp+50h],0
  000000006706C37C: 74 13              je          000000006706C391
  000000006706C37E: 48 85 DB           test        rbx,rbx
  000000006706C381: 74 3A              je          000000006706C3BD
  000000006706C383: C6 03 01           mov         byte ptr [rbx],1
  000000006706C386: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  000000006706C38B: 48 83 C4 30        add         rsp,30h
  000000006706C38F: 5F                 pop         rdi
  000000006706C390: C3                 ret
  000000006706C391: 45 33 C0           xor         r8d,r8d
  000000006706C394: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006706C399: 41 8D 50 01        lea         edx,[r8+1]
  000000006706C39D: E8 FE 11 FE FF     call        000000006704D5A0
  000000006706C3A2: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006706C3A7: E8 94 CF FD FF     call        0000000067049340
  000000006706C3AC: 45 33 C9           xor         r9d,r9d
  000000006706C3AF: 4C 8B C3           mov         r8,rbx
  000000006706C3B2: 48 8B C8           mov         rcx,rax
  000000006706C3B5: 48 8B D7           mov         rdx,rdi
  000000006706C3B8: E8 83 FF FD FF     call        000000006704C340
  000000006706C3BD: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  000000006706C3C2: 48 83 C4 30        add         rsp,30h
  000000006706C3C6: 5F                 pop         rdi
  000000006706C3C7: C3                 ret
  000000006706C3C8: CC                 int         3
  000000006706C3C9: CC                 int         3
  000000006706C3CA: CC                 int         3
  000000006706C3CB: CC                 int         3
  000000006706C3CC: CC                 int         3
  000000006706C3CD: CC                 int         3
  000000006706C3CE: CC                 int         3
  000000006706C3CF: CC                 int         3
  000000006706C3D0: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  000000006706C3D5: 57                 push        rdi
  000000006706C3D6: 48 83 EC 30        sub         rsp,30h
  000000006706C3DA: 48 8B F9           mov         rdi,rcx
  000000006706C3DD: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006706C3E2: 48 8B DA           mov         rbx,rdx
  000000006706C3E5: E8 06 11 FE FF     call        000000006704D4F0
  000000006706C3EA: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006706C3EF: E8 4C CF FD FF     call        0000000067049340
  000000006706C3F4: 4C 8D 44 24 50     lea         r8,[rsp+50h]
  000000006706C3F9: 45 33 C9           xor         r9d,r9d
  000000006706C3FC: 48 8B C8           mov         rcx,rax
  000000006706C3FF: 48 8B D7           mov         rdx,rdi
  000000006706C402: E8 39 FF FD FF     call        000000006704C340
  000000006706C407: 80 7C 24 50 00     cmp         byte ptr [rsp+50h],0
  000000006706C40C: 75 35              jne         000000006706C443
  000000006706C40E: 45 33 C0           xor         r8d,r8d
  000000006706C411: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006706C416: 41 8D 50 01        lea         edx,[r8+1]
  000000006706C41A: E8 81 11 FE FF     call        000000006704D5A0
  000000006706C41F: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006706C424: E8 17 CF FD FF     call        0000000067049340
  000000006706C429: 4C 8D 44 24 58     lea         r8,[rsp+58h]
  000000006706C42E: 45 33 C9           xor         r9d,r9d
  000000006706C431: 48 8B C8           mov         rcx,rax
  000000006706C434: 48 8B D7           mov         rdx,rdi
  000000006706C437: E8 04 FF FD FF     call        000000006704C340
  000000006706C43C: 80 7C 24 58 00     cmp         byte ptr [rsp+58h],0
  000000006706C441: 74 2F              je          000000006706C472
  000000006706C443: 66 0F 2F 05 FD 58  comisd      xmm0,mmword ptr [00000000671F1D48h]
                    18 00
  000000006706C44B: 77 25              ja          000000006706C472
  000000006706C44D: F2
  000000006706C44E: 0F
  000000006706C44F: 10
  000000006706C450: 0D

  Summary

      1A7000 .text
