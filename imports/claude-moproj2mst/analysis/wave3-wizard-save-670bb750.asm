Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670BB750: 48 89 5C 24 20     mov         qword ptr [rsp+20h],rbx
  00000000670BB755: 57                 push        rdi
  00000000670BB756: 41 54              push        r12
  00000000670BB758: 41 56              push        r14
  00000000670BB75A: 48 83 EC 20        sub         rsp,20h
  00000000670BB75E: 48 8B 59 08        mov         rbx,qword ptr [rcx+8]
  00000000670BB762: 49 8B F8           mov         rdi,r8
  00000000670BB765: 4C 8B E2           mov         r12,rdx
  00000000670BB768: 8B 83 88 00 00 00  mov         eax,dword ptr [rbx+0000000000000088h]
  00000000670BB76E: 4C 8B F1           mov         r14,rcx
  00000000670BB771: A8 02              test        al,2
  00000000670BB773: 75 23              jne         00000000670BB798
  00000000670BB775: 85 C0              test        eax,eax
  00000000670BB777: 74 0C              je          00000000670BB785
  00000000670BB779: 48 8D 0D 48 F7 16  lea         rcx,[000000006722AEC8h]
                    00
  00000000670BB780: E8 5B 17 F6 FF     call        000000006701CEE0
  00000000670BB785: 48 83 C8 FF        or          rax,0FFFFFFFFFFFFFFFFh
  00000000670BB789: 48 8B 5C 24 58     mov         rbx,qword ptr [rsp+58h]
  00000000670BB78E: 48 83 C4 20        add         rsp,20h
  00000000670BB792: 41 5E              pop         r14
  00000000670BB794: 41 5C              pop         r12
  00000000670BB796: 5F                 pop         rdi
  00000000670BB797: C3                 ret
  00000000670BB798: 4D 85 C0           test        r8,r8
  00000000670BB79B: 79 1F              jns         00000000670BB7BC
  00000000670BB79D: 48 8D 0D F4 F6 16  lea         rcx,[000000006722AE98h]
                    00
  00000000670BB7A4: E8 37 17 F6 FF     call        000000006701CEE0
  00000000670BB7A9: 48 83 C8 FF        or          rax,0FFFFFFFFFFFFFFFFh
  00000000670BB7AD: 48 8B 5C 24 58     mov         rbx,qword ptr [rsp+58h]
  00000000670BB7B2: 48 83 C4 20        add         rsp,20h
  00000000670BB7B6: 41 5E              pop         r14
  00000000670BB7B8: 41 5C              pop         r12
  00000000670BB7BA: 5F                 pop         rdi
  00000000670BB7BB: C3                 ret
  00000000670BB7BC: 83 BB E4 00 00 00  cmp         dword ptr [rbx+00000000000000E4h],0
                    00

  Summary

      1A7000 .text
