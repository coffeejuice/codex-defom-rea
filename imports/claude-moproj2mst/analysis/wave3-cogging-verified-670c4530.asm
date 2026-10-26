Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670C4530: 40 57              push        rdi
  00000000670C4532: 48 83 EC 30        sub         rsp,30h
  00000000670C4536: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670C453F: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  00000000670C4544: 48 89 6C 24 50     mov         qword ptr [rsp+50h],rbp
  00000000670C4549: 48 89 74 24 58     mov         qword ptr [rsp+58h],rsi
  00000000670C454E: 48 8B C2           mov         rax,rdx
  00000000670C4551: 48 8B F1           mov         rsi,rcx
  00000000670C4554: 48 8B 69 08        mov         rbp,qword ptr [rcx+8]
  00000000670C4558: 48 83 7D 28 00     cmp         qword ptr [rbp+28h],0
  00000000670C455D: 75 18              jne         00000000670C4577
  00000000670C455F: 48 83 7D 00 00     cmp         qword ptr [rbp],0
  00000000670C4564: 75 11              jne         00000000670C4577
  00000000670C4566: 48 8D 0D 5B 7E 16  lea         rcx,[000000006722C3C8h]
                    00
  00000000670C456D: E8 6E 89 F5 FF     call        000000006701CEE0
  00000000670C4572: 48 8B C6           mov         rax,rsi
  00000000670C4575: EB 37              jmp         00000000670C45AE
  00000000670C4577: 83 CA FF           or          edx,0FFFFFFFFh
  00000000670C457A: 48 8B C8           mov         rcx,rax
  00000000670C457D: E8 BE 79 FA FF     call        000000006706BF40
  00000000670C4582: 48 8B D8           mov         rbx,rax
  00000000670C4585: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000000670C458A: 45 33 C0           xor         r8d,r8d
  00000000670C458D: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  00000000670C4592: 48 8B CD           mov         rcx,rbp
  00000000670C4595: E8 96 EF FF FF     call        00000000670C3530
  00000000670C459A: 90                 nop
  00000000670C459B: F0 83 03 FF        lock add    dword ptr [rbx],0FFFFFFFFh
  00000000670C459F: 75 0A              jne         00000000670C45AB
  00000000670C45A1: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000000670C45A6: E8 25 BB FA FF     call        00000000670700D0
  00000000670C45AB: 48 8B C6           mov         rax,rsi
  00000000670C45AE: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  00000000670C45B3: 48 8B 6C 24 50     mov         rbp,qword ptr [rsp+50h]
  00000000670C45B8: 48 8B 74 24 58     mov         rsi,qword ptr [rsp+58h]
  00000000670C45BD: 48 83 C4 30        add         rsp,30h
  00000000670C45C1: 5F                 pop         rdi
  00000000670C45C2: C3                 ret

  Summary

      1A7000 .text
