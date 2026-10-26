Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000654760A0: 40 55              push        rbp
  00000000654760A2: 56                 push        rsi
  00000000654760A3: 57                 push        rdi
  00000000654760A4: 48 83 EC 30        sub         rsp,30h
  00000000654760A8: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000654760B1: 48 89 5C 24 60     mov         qword ptr [rsp+60h],rbx
  00000000654760B6: 48 8B FA           mov         rdi,rdx
  00000000654760B9: 48 8B 69 08        mov         rbp,qword ptr [rcx+8]
  00000000654760BD: 48 8D 54 24 58     lea         rdx,[rsp+58h]
  00000000654760C2: 48 8B 8D 58 02 00  mov         rcx,qword ptr [rbp+0000000000000258h]
                    00
  00000000654760C9: E8 F2 5E F2 FF     call        000000006539BFC0
  00000000654760CE: 48 8B D8           mov         rbx,rax
  00000000654760D1: 48 8B C8           mov         rcx,rax
  00000000654760D4: FF 15 6E 08 22 00  call        qword ptr [0000000065696948h]
  00000000654760DA: 48 8B F0           mov         rsi,rax
  00000000654760DD: 48 8B CB           mov         rcx,rbx
  00000000654760E0: FF 15 6A 08 22 00  call        qword ptr [0000000065696950h]
  00000000654760E6: 48 3B C6           cmp         rax,rsi
  00000000654760E9: 74 19              je          0000000065476104
  00000000654760EB: 0F 1F 44 00 00     nop         dword ptr [rax+rax]
  00000000654760F0: 48 83 E8 08        sub         rax,8
  00000000654760F4: 48 39 38           cmp         qword ptr [rax],rdi
  00000000654760F7: 74 07              je          0000000065476100
  00000000654760F9: 48 3B C6           cmp         rax,rsi
  00000000654760FC: 75 F2              jne         00000000654760F0
  00000000654760FE: EB 04              jmp         0000000065476104
  0000000065476100: B0 01              mov         al,1
  0000000065476102: EB 02              jmp         0000000065476106
  0000000065476104: 32 C0              xor         al,al
  0000000065476106: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  000000006547610B: 33 D2              xor         edx,edx
  000000006547610D: 84 C0              test        al,al
  000000006547610F: 48 0F 44 CA        cmove       rcx,rdx
  0000000065476113: 48 85 C9           test        rcx,rcx
  0000000065476116: 0F 94 C3           sete        bl
  0000000065476119: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  000000006547611E: FF 15 54 08 22 00  call        qword ptr [0000000065696978h]
  0000000065476124: 84 C0              test        al,al
  0000000065476126: 75 0B              jne         0000000065476133
  0000000065476128: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  000000006547612D: FF 15 2D 08 22 00  call        qword ptr [0000000065696960h]
  0000000065476133: 84 DB              test        bl,bl
  0000000065476135: 75 1E              jne         0000000065476155
  0000000065476137: 48 89 BD 70 02 00  mov         qword ptr [rbp+0000000000000270h],rdi
                    00
  000000006547613E: B2 01              mov         dl,1
  0000000065476140: 48 8B CF           mov         rcx,rdi
  0000000065476143: E8 48 13 F8 FF     call        00000000653F7490
  0000000065476148: BA 07 00 00 00     mov         edx,7
  000000006547614D: 48 8B CF           mov         rcx,rdi
  0000000065476150: E8 0B E6 BE FF     call        0000000065064760
  0000000065476155: 48 8B 5C 24 60     mov         rbx,qword ptr [rsp+60h]
  000000006547615A: 48 83 C4 30        add         rsp,30h
  000000006547615E: 5F                 pop         rdi
  000000006547615F: 5E                 pop         rsi
  0000000065476160: 5D                 pop         rbp
  0000000065476161: C3                 ret

  Summary

      695000 .text
