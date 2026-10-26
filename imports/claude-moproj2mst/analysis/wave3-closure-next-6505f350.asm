Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006505F350: 48 8B C4           mov         rax,rsp
  000000006505F353: 48 89 50 10        mov         qword ptr [rax+10h],rdx
  000000006505F357: 55                 push        rbp
  000000006505F358: 56                 push        rsi
  000000006505F359: 57                 push        rdi
  000000006505F35A: 48 83 EC 50        sub         rsp,50h
  000000006505F35E: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006505F367: 48 89 58 18        mov         qword ptr [rax+18h],rbx
  000000006505F36B: 41 8B E8           mov         ebp,r8d
  000000006505F36E: 48 8B FA           mov         rdi,rdx
  000000006505F371: 48 8B D9           mov         rbx,rcx
  000000006505F374: C7 40 20 00 00 00  mov         dword ptr [rax+20h],0
                    00
  000000006505F37B: 48 8B 71 08        mov         rsi,qword ptr [rcx+8]
  000000006505F37F: 48 8B D6           mov         rdx,rsi
  000000006505F382: 48 8B CF           mov         rcx,rdi
  000000006505F385: E8 46 21 FC FF     call        00000000650214D0
  000000006505F38A: C7 84 24 88 00 00  mov         dword ptr [rsp+0000000000000088h],1
                    00 01 00 00 00
  000000006505F395: 48 8B 4E 08        mov         rcx,qword ptr [rsi+8]
  000000006505F399: 8B 81 D0 01 00 00  mov         eax,dword ptr [rcx+00000000000001D0h]
  000000006505F39F: D1 E8              shr         eax,1
  000000006505F3A1: 24 01              and         al,1
  000000006505F3A3: 0F 85 3E 01 00 00  jne         000000006505F4E7
  000000006505F3A9: 48 8B 46 20        mov         rax,qword ptr [rsi+20h]
  000000006505F3AD: 0F B6 40 0C        movzx       eax,byte ptr [rax+0Ch]
  000000006505F3B1: A8 01              test        al,1
  000000006505F3B3: 74 26              je          000000006505F3DB
  000000006505F3B5: 0F B6 81 CE 01 00  movzx       eax,byte ptr [rcx+00000000000001CEh]
                    00
  000000006505F3BC: 24 01              and         al,1
  000000006505F3BE: 75 1B              jne         000000006505F3DB
  000000006505F3C0: 48 8B 83 88 00 00  mov         rax,qword ptr [rbx+0000000000000088h]
                    00
  000000006505F3C7: 48 85 C0           test        rax,rax
  000000006505F3CA: 0F 84 17 01 00 00  je          000000006505F4E7
  000000006505F3D0: 48 83 78 10 00     cmp         qword ptr [rax+10h],0
  000000006505F3D5: 0F 84 0C 01 00 00  je          000000006505F4E7
  000000006505F3DB: 48 8B CE           mov         rcx,rsi
  000000006505F3DE: FF 15 84 74 63 00  call        qword ptr [0000000065696868h]
  000000006505F3E4: 48 8B F0           mov         rsi,rax
  000000006505F3E7: 48 85 C0           test        rax,rax
  000000006505F3EA: 0F 84 9D 00 00 00  je          000000006505F48D
  000000006505F3F0: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  000000006505F3F4: 8B 91 D0 01 00 00  mov         edx,dword ptr [rcx+00000000000001D0h]
  000000006505F3FA: D1 EA              shr         edx,1
  000000006505F3FC: 80 E2 01           and         dl,1
  000000006505F3FF: 0F 85 E2 00 00 00  jne         000000006505F4E7
  000000006505F405: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006505F40A: E8 21 BF FB FF     call        000000006501B330
  000000006505F40F: 90                 nop
  000000006505F410: 48 8B D0           mov         rdx,rax
  000000006505F413: 48 8B CF           mov         rcx,rdi
  000000006505F416: E8 35 64 1A 00     call        0000000065205850
  000000006505F41B: 84 C0              test        al,al
  000000006505F41D: 0F 94 C3           sete        bl
  000000006505F420: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006505F425: E8 F6 76 1A 00     call        0000000065206B20
  000000006505F42A: 84 DB              test        bl,bl
  000000006505F42C: 74 4D              je          000000006505F47B
  000000006505F42E: 48 8B 56 20        mov         rdx,qword ptr [rsi+20h]
  000000006505F432: 48 83 C2 38        add         rdx,38h
  000000006505F436: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006505F43B: E8 80 9C 1D 00     call        00000000652390C0
  000000006505F440: 90                 nop
  000000006505F441: 89 6C 24 30        mov         dword ptr [rsp+30h],ebp
  000000006505F445: 4C 8B C7           mov         r8,rdi
  000000006505F448: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  000000006505F44D: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006505F452: E8 49 48 19 00     call        00000000651F3CA0
  000000006505F457: 90                 nop
  000000006505F458: 48 8B D0           mov         rdx,rax
  000000006505F45B: 48 8B CF           mov         rcx,rdi
  000000006505F45E: E8 FD 3B 19 00     call        00000000651F3060
  000000006505F463: 90                 nop
  000000006505F464: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000006505F469: E8 B2 76 1A 00     call        0000000065206B20
  000000006505F46E: 90                 nop
  000000006505F46F: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006505F474: E8 A7 76 1A 00     call        0000000065206B20
  000000006505F479: EB 6C              jmp         000000006505F4E7
  000000006505F47B: 48 8B 56 20        mov         rdx,qword ptr [rsi+20h]
  000000006505F47F: 48 83 C2 38        add         rdx,38h
  000000006505F483: 48 8B CF           mov         rcx,rdi
  000000006505F486: E8 D5 3B 19 00     call        00000000651F3060
  000000006505F48B: EB 5A              jmp         000000006505F4E7
  000000006505F48D: 48 8B 8B 88 00 00  mov         rcx,qword ptr [rbx+0000000000000088h]
                    00
  000000006505F494: 48 85 C9           test        rcx,rcx
  000000006505F497: 74 4E              je          000000006505F4E7
  000000006505F499: 48 83 79 10 00     cmp         qword ptr [rcx+10h],0
  000000006505F49E: 74 47              je          000000006505F4E7
  000000006505F4A0: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  000000006505F4A5: 48 8B 49 10        mov         rcx,qword ptr [rcx+10h]
  000000006505F4A9: E8 02 15 53 00     call        00000000655909B0
  000000006505F4AE: 90                 nop
  000000006505F4AF: 89 6C 24 30        mov         dword ptr [rsp+30h],ebp
  000000006505F4B3: 4C 8B C7           mov         r8,rdi
  000000006505F4B6: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  000000006505F4BB: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006505F4C0: E8 DB 47 19 00     call        00000000651F3CA0
  000000006505F4C5: 90                 nop
  000000006505F4C6: 48 8B D0           mov         rdx,rax
  000000006505F4C9: 48 8B CF           mov         rcx,rdi
  000000006505F4CC: E8 8F 3B 19 00     call        00000000651F3060
  000000006505F4D1: 90                 nop
  000000006505F4D2: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000006505F4D7: E8 44 76 1A 00     call        0000000065206B20
  000000006505F4DC: 90                 nop
  000000006505F4DD: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006505F4E2: E8 39 76 1A 00     call        0000000065206B20
  000000006505F4E7: C7 47 08 00 00 00  mov         dword ptr [rdi+8],0
                    00
  000000006505F4EE: 48 8B C7           mov         rax,rdi
  000000006505F4F1: 48 8B 9C 24 80 00  mov         rbx,qword ptr [rsp+0000000000000080h]
                    00 00
  000000006505F4F9: 48 83 C4 50        add         rsp,50h
  000000006505F4FD: 5F                 pop         rdi
  000000006505F4FE: 5E                 pop         rsi
  000000006505F4FF: 5D                 pop         rbp
  000000006505F500: C3                 ret
  000000006505F501: CC                 int         3

  Summary

      695000 .text
