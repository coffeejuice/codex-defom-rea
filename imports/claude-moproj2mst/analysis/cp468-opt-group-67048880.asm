Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067048880: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  0000000067048885: 48 89 6C 24 10     mov         qword ptr [rsp+10h],rbp
  000000006704888A: 56                 push        rsi
  000000006704888B: 57                 push        rdi
  000000006704888C: 41 54              push        r12
  000000006704888E: 41 55              push        r13
  0000000067048890: 41 56              push        r14
  0000000067048892: 48 83 EC 20        sub         rsp,20h
  0000000067048896: 4C 8B 29           mov         r13,qword ptr [rcx]
  0000000067048899: 4C 8B F1           mov         r14,rcx
  000000006704889C: 4C 8B E2           mov         r12,rdx
  000000006704889F: 41 8B 75 0C        mov         esi,dword ptr [r13+0Ch]
  00000000670488A3: BA 18 00 00 00     mov         edx,18h
  00000000670488A8: 41 2B 75 08        sub         esi,dword ptr [r13+8]
  00000000670488AC: 42 8D 2C 06        lea         ebp,[rsi+r8]
  00000000670488B0: 8D 0C ED 00 00 00  lea         ecx,[rbp*8+0000000000000000h]
                    00
  00000000670488B7: E8 E4 41 FE FF     call        000000006702CAA0
  00000000670488BC: 4C 63 C0           movsxd      r8,eax
  00000000670488BF: 49 C1 E8 03        shr         r8,3
  00000000670488C3: 44 89 44 24 60     mov         dword ptr [rsp+60h],r8d
  00000000670488C8: 48 63 5C 24 60     movsxd      rbx,dword ptr [rsp+60h]
  00000000670488CD: 48 8D 0C DD 18 00  lea         rcx,[rbx*8+0000000000000018h]
                    00 00
  00000000670488D5: E8 C6 7A FD FF     call        00000000670203A0
  00000000670488DA: 48 8B F8           mov         rdi,rax
  00000000670488DD: 48 85 C0           test        rax,rax
  00000000670488E0: 75 05              jne         00000000670488E7
  00000000670488E2: E8 C9 36 FD FF     call        000000006701BFB0
  00000000670488E7: 83 4F 10 01        or          dword ptr [rdi+10h],1
  00000000670488EB: C7 07 01 00 00 00  mov         dword ptr [rdi],1
  00000000670488F1: 89 5F 04           mov         dword ptr [rdi+4],ebx
  00000000670488F4: 41 8B 04 24        mov         eax,dword ptr [r12]
  00000000670488F8: 85 C0              test        eax,eax
  00000000670488FA: 79 0E              jns         000000006704890A
  00000000670488FC: 41 C7 04 24 00 00  mov         dword ptr [r12],0
                    00 00
  0000000067048904: 2B DD              sub         ebx,ebp
  0000000067048906: D1 FB              sar         ebx,1
  0000000067048908: EB 12              jmp         000000006704891C
  000000006704890A: 3B C6              cmp         eax,esi
  000000006704890C: 7E 06              jle         0000000067048914
  000000006704890E: 41 89 34 24        mov         dword ptr [r12],esi
  0000000067048912: EB 06              jmp         000000006704891A
  0000000067048914: D1 FE              sar         esi,1
  0000000067048916: 3B C6              cmp         eax,esi
  0000000067048918: 7C EA              jl          0000000067048904
  000000006704891A: 33 DB              xor         ebx,ebx
  000000006704891C: 8D 0C 2B           lea         ecx,[rbx+rbp]
  000000006704891F: 48 8B 6C 24 58     mov         rbp,qword ptr [rsp+58h]
  0000000067048924: 89 5F 08           mov         dword ptr [rdi+8],ebx
  0000000067048927: 48 8B 5C 24 50     mov         rbx,qword ptr [rsp+50h]
  000000006704892C: 89 4F 0C           mov         dword ptr [rdi+0Ch],ecx
  000000006704892F: 49 8B C5           mov         rax,r13
  0000000067048932: 49 89 3E           mov         qword ptr [r14],rdi
  0000000067048935: 48 83 C4 20        add         rsp,20h
  0000000067048939: 41 5E              pop         r14
  000000006704893B: 41 5D              pop         r13
  000000006704893D: 41 5C              pop         r12
  000000006704893F: 5F                 pop         rdi
  0000000067048940: 5E                 pop         rsi
  0000000067048941: C3                 ret

  Summary

      1A7000 .text
