Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000651F2540: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  00000000651F2545: 57                 push        rdi
  00000000651F2546: 48 83 EC 20        sub         rsp,20h
  00000000651F254A: 48 8B F9           mov         rdi,rcx
  00000000651F254D: 48 8B 0A           mov         rcx,qword ptr [rdx]
  00000000651F2550: 48 8B DA           mov         rbx,rdx
  00000000651F2553: 48 8B 17           mov         rdx,qword ptr [rdi]
  00000000651F2556: 48 3B CA           cmp         rcx,rdx
  00000000651F2559: 0F 84 85 00 00 00  je          00000000651F25E4
  00000000651F255F: 48 83 C2 08        add         rdx,8
  00000000651F2563: 48 83 C1 08        add         rcx,8
  00000000651F2567: E8 D4 F2 FF FF     call        00000000651F1840
  00000000651F256C: 84 C0              test        al,al
  00000000651F256E: 74 67              je          00000000651F25D7
  00000000651F2570: 48 8B 13           mov         rdx,qword ptr [rbx]
  00000000651F2573: 4C 8B 07           mov         r8,qword ptr [rdi]
  00000000651F2576: F2 0F 10 42 18     movsd       xmm0,mmword ptr [rdx+18h]
  00000000651F257B: 66 41 0F 2E 40 18  ucomisd     xmm0,mmword ptr [r8+18h]
  00000000651F2581: 7A 54              jp          00000000651F25D7
  00000000651F2583: 75 52              jne         00000000651F25D7
  00000000651F2585: 41 8B 48 48        mov         ecx,dword ptr [r8+48h]
  00000000651F2589: 44 8B 4A 48        mov         r9d,dword ptr [rdx+48h]
  00000000651F258D: 8B C1              mov         eax,ecx
  00000000651F258F: 41 33 C1           xor         eax,r9d
  00000000651F2592: A8 02              test        al,2
  00000000651F2594: 75 41              jne         00000000651F25D7
  00000000651F2596: 8B C1              mov         eax,ecx
  00000000651F2598: 41 33 C1           xor         eax,r9d
  00000000651F259B: A8 04              test        al,4
  00000000651F259D: 75 38              jne         00000000651F25D7
  00000000651F259F: 8B C1              mov         eax,ecx
  00000000651F25A1: 41 33 C1           xor         eax,r9d
  00000000651F25A4: A8 08              test        al,8
  00000000651F25A6: 75 2F              jne         00000000651F25D7
  00000000651F25A8: 8B C1              mov         eax,ecx
  00000000651F25AA: 41 33 C1           xor         eax,r9d
  00000000651F25AD: A8 10              test        al,10h
  00000000651F25AF: 75 26              jne         00000000651F25D7
  00000000651F25B1: 41 33 C9           xor         ecx,r9d
  00000000651F25B4: F6 C1 E0           test        cl,0E0h
  00000000651F25B7: 75 1E              jne         00000000651F25D7
  00000000651F25B9: 41 0F B6 40 4C     movzx       eax,byte ptr [r8+4Ch]
  00000000651F25BE: 32 42 4C           xor         al,byte ptr [rdx+4Ch]
  00000000651F25C1: A8 01              test        al,1
  00000000651F25C3: 75 12              jne         00000000651F25D7
  00000000651F25C5: 41 8B 40 50        mov         eax,dword ptr [r8+50h]
  00000000651F25C9: 39 42 50           cmp         dword ptr [rdx+50h],eax
  00000000651F25CC: 75 09              jne         00000000651F25D7
  00000000651F25CE: 41 8B 40 54        mov         eax,dword ptr [r8+54h]
  00000000651F25D2: 39 42 54           cmp         dword ptr [rdx+54h],eax
  00000000651F25D5: 74 0D              je          00000000651F25E4
  00000000651F25D7: 32 C0              xor         al,al
  00000000651F25D9: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  00000000651F25DE: 48 83 C4 20        add         rsp,20h
  00000000651F25E2: 5F                 pop         rdi
  00000000651F25E3: C3                 ret
  00000000651F25E4: B0 01              mov         al,1
  00000000651F25E6: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  00000000651F25EB: 48 83 C4 20        add         rsp,20h
  00000000651F25EF: 5F                 pop         rdi
  00000000651F25F0: C3                 ret

  Summary

      695000 .text
