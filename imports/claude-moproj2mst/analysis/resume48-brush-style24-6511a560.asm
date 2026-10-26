Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006511A560: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000006511A565: 53                 push        rbx
  000000006511A566: 56                 push        rsi
  000000006511A567: 57                 push        rdi
  000000006511A568: 48 83 EC 40        sub         rsp,40h
  000000006511A56C: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006511A575: 48 8B F2           mov         rsi,rdx
  000000006511A578: 33 DB              xor         ebx,ebx
  000000006511A57A: 89 5C 24 60        mov         dword ptr [rsp+60h],ebx
  000000006511A57E: 48 8B 39           mov         rdi,qword ptr [rcx]
  000000006511A581: 83 7F 04 18        cmp         dword ptr [rdi+4],18h
  000000006511A585: 75 3C              jne         000000006511A5C3
  000000006511A587: 48 39 5F 70        cmp         qword ptr [rdi+70h],rbx
  000000006511A58B: 75 30              jne         000000006511A5BD
  000000006511A58D: 8D 4B 18           lea         ecx,[rbx+18h]
  000000006511A590: E8 DD 6B 4C 00     call        00000000655E1172
  000000006511A595: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  000000006511A59A: 48 85 C0           test        rax,rax
  000000006511A59D: 74 17              je          000000006511A5B6
  000000006511A59F: 89 5C 24 70        mov         dword ptr [rsp+70h],ebx
  000000006511A5A3: 48 8D 57 78        lea         rdx,[rdi+78h]
  000000006511A5A7: 4C 8D 44 24 70     lea         r8,[rsp+70h]
  000000006511A5AC: 48 8B C8           mov         rcx,rax
  000000006511A5AF: E8 5C 4E FB FF     call        00000000650CF410
  000000006511A5B4: EB 03              jmp         000000006511A5B9
  000000006511A5B6: 48 8B C3           mov         rax,rbx
  000000006511A5B9: 48 89 47 70        mov         qword ptr [rdi+70h],rax
  000000006511A5BD: 48 8B 47 70        mov         rax,qword ptr [rdi+70h]
  000000006511A5C1: EB 14              jmp         000000006511A5D7
  000000006511A5C3: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006511A5C8: E8 B3 3D FB FF     call        00000000650CE380
  000000006511A5CD: 90                 nop
  000000006511A5CE: BB 01 00 00 00     mov         ebx,1
  000000006511A5D3: 89 5C 24 60        mov         dword ptr [rsp+60h],ebx
  000000006511A5D7: 48 8B D0           mov         rdx,rax
  000000006511A5DA: 48 8B CE           mov         rcx,rsi
  000000006511A5DD: E8 AE 4F FB FF     call        00000000650CF590
  000000006511A5E2: 83 CB 02           or          ebx,2
  000000006511A5E5: F6 C3 01           test        bl,1
  000000006511A5E8: 74 16              je          000000006511A600
  000000006511A5EA: 83 E3 FE           and         ebx,0FFFFFFFEh
  000000006511A5ED: 89 5C 24 60        mov         dword ptr [rsp+60h],ebx
  000000006511A5F1: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006511A5F6: E8 25 43 FB FF     call        00000000650CE920
  000000006511A5FB: 48 8B C6           mov         rax,rsi
  000000006511A5FE: EB 03              jmp         000000006511A603
  000000006511A600: 48 8B C6           mov         rax,rsi
  000000006511A603: 48 83 C4 40        add         rsp,40h
  000000006511A607: 5F                 pop         rdi
  000000006511A608: 5E                 pop         rsi
  000000006511A609: 5B                 pop         rbx
  000000006511A60A: C3                 ret
  000000006511A60B: CC                 int         3

  Summary

      695000 .text
