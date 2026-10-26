Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000654767C0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000000654767C5: 56                 push        rsi
  00000000654767C6: 57                 push        rdi
  00000000654767C7: 41 54              push        r12
  00000000654767C9: 48 83 EC 40        sub         rsp,40h
  00000000654767CD: 48 C7 44 24 28 FE  mov         qword ptr [rsp+28h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000654767D6: 48 89 5C 24 68     mov         qword ptr [rsp+68h],rbx
  00000000654767DB: 48 89 6C 24 70     mov         qword ptr [rsp+70h],rbp
  00000000654767E0: 41 8B F1           mov         esi,r9d
  00000000654767E3: 49 8B E8           mov         rbp,r8
  00000000654767E6: 4C 8B E2           mov         r12,rdx
  00000000654767E9: 48 8B F9           mov         rdi,rcx
  00000000654767EC: B9 C0 02 00 00     mov         ecx,2C0h
  00000000654767F1: E8 7C A9 16 00     call        00000000655E1172
  00000000654767F6: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000000654767FB: 48 85 C0           test        rax,rax
  00000000654767FE: 74 0D              je          000000006547680D
  0000000065476800: 48 8B C8           mov         rcx,rax
  0000000065476803: E8 18 EE FF FF     call        0000000065475620
  0000000065476808: 48 8B D0           mov         rdx,rax
  000000006547680B: EB 02              jmp         000000006547680F
  000000006547680D: 33 D2              xor         edx,edx
  000000006547680F: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000065476817: 8B 00              mov         eax,dword ptr [rax]
  0000000065476819: 0F BA E8 0C        bts         eax,0Ch
  000000006547681D: 0F BA E8 0D        bts         eax,0Dh
  0000000065476821: 0F BA E8 1B        bts         eax,1Bh
  0000000065476825: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  0000000065476829: 4C 8D 4C 24 20     lea         r9,[rsp+20h]
  000000006547682E: 4C 8B 84 24 98 00  mov         r8,qword ptr [rsp+0000000000000098h]
                    00 00
  0000000065476836: 48 8B CF           mov         rcx,rdi
  0000000065476839: E8 42 28 FE FF     call        0000000065459080
  000000006547683E: 90                 nop
  000000006547683F: 48 8D 05 72 A6 2A  lea         rax,[0000000065720EB8h]
                    00
  0000000065476846: 48 89 07           mov         qword ptr [rdi],rax
  0000000065476849: 48 8D 05 30 A6 2A  lea         rax,[0000000065720E80h]
                    00
  0000000065476850: 48 89 47 10        mov         qword ptr [rdi+10h],rax
  0000000065476854: 48 8B 5F 08        mov         rbx,qword ptr [rdi+8]
  0000000065476858: 4C 8B C5           mov         r8,rbp
  000000006547685B: 49 8B D4           mov         rdx,r12
  000000006547685E: 48 8B CB           mov         rcx,rbx
  0000000065476861: E8 FA EE FF FF     call        0000000065475760
  0000000065476866: 8B D6              mov         edx,esi
  0000000065476868: 48 8B CF           mov         rcx,rdi
  000000006547686B: E8 30 F9 FF FF     call        00000000654761A0
  0000000065476870: 44 8B 8C 24 90 00  mov         r9d,dword ptr [rsp+0000000000000090h]
                    00 00
  0000000065476878: 44 8B 84 24 88 00  mov         r8d,dword ptr [rsp+0000000000000088h]
                    00 00
  0000000065476880: 8B 94 24 80 00 00  mov         edx,dword ptr [rsp+0000000000000080h]
                    00
  0000000065476887: 48 8B CB           mov         rcx,rbx
  000000006547688A: E8 B1 FD FF FF     call        0000000065476640
  000000006547688F: 90                 nop
  0000000065476890: 48 8B C7           mov         rax,rdi
  0000000065476893: 48 8B 5C 24 68     mov         rbx,qword ptr [rsp+68h]
  0000000065476898: 48 8B 6C 24 70     mov         rbp,qword ptr [rsp+70h]
  000000006547689D: 48 83 C4 40        add         rsp,40h
  00000000654768A1: 41 5C              pop         r12
  00000000654768A3: 5F                 pop         rdi
  00000000654768A4: 5E                 pop         rsi
  00000000654768A5: C3                 ret

  Summary

      695000 .text
