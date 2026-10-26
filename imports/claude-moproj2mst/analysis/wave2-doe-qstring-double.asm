Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

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

  Summary

      1A7000 .text
