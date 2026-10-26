Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065401EF0: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  0000000065401EF5: 48 89 74 24 10     mov         qword ptr [rsp+10h],rsi
  0000000065401EFA: 57                 push        rdi
  0000000065401EFB: 48 83 EC 20        sub         rsp,20h
  0000000065401EFF: 48 8B 79 08        mov         rdi,qword ptr [rcx+8]
  0000000065401F03: 41 0F B6 F0        movzx       esi,r8b
  0000000065401F07: 8B DA              mov         ebx,edx
  0000000065401F09: 85 D2              test        edx,edx
  0000000065401F0B: 78 46              js          0000000065401F53
  0000000065401F0D: 48 8D 8F 10 02 00  lea         rcx,[rdi+0000000000000210h]
                    00
  0000000065401F14: FF 15 9E 49 29 00  call        qword ptr [00000000656968B8h]
  0000000065401F1A: 3B D8              cmp         ebx,eax
  0000000065401F1C: 7D 35              jge         0000000065401F53
  0000000065401F1E: 48 8D 8F 10 02 00  lea         rcx,[rdi+0000000000000210h]
                    00
  0000000065401F25: 8B D3              mov         edx,ebx
  0000000065401F27: FF 15 13 4A 29 00  call        qword ptr [0000000065696940h]
  0000000065401F2D: 48 8B 08           mov         rcx,qword ptr [rax]
  0000000065401F30: 33 C0              xor         eax,eax
  0000000065401F32: 40 84 F6           test        sil,sil
  0000000065401F35: 0F 95 C0           setne       al
  0000000065401F38: 03 C0              add         eax,eax
  0000000065401F3A: 33 41 14           xor         eax,dword ptr [rcx+14h]
  0000000065401F3D: 83 E0 06           and         eax,6
  0000000065401F40: 31 41 14           xor         dword ptr [rcx+14h],eax
  0000000065401F43: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  0000000065401F48: 48 8B 74 24 38     mov         rsi,qword ptr [rsp+38h]
  0000000065401F4D: 48 83 C4 20        add         rsp,20h
  0000000065401F51: 5F                 pop         rdi
  0000000065401F52: C3                 ret
  0000000065401F53: 48 8D 0D EE 91 30  lea         rcx,[000000006570B148h]
                    00
  0000000065401F5A: 8B D3              mov         edx,ebx
  0000000065401F5C: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  0000000065401F61: 48 8B 74 24 38     mov         rsi,qword ptr [rsp+38h]
  0000000065401F66: 48 83 C4 20        add         rsp,20h
  0000000065401F6A: 5F                 pop         rdi
  0000000065401F6B: 48 FF 25 26 4A 29  jmp         qword ptr [0000000065696998h]
                    00

  Summary

      695000 .text
