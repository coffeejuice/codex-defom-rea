Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067074730: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  0000000067074735: 48 89 74 24 10     mov         qword ptr [rsp+10h],rsi
  000000006707473A: 57                 push        rdi
  000000006707473B: 48 83 EC 20        sub         rsp,20h
  000000006707473F: 49 63 F8           movsxd      rdi,r8d
  0000000067074742: 48 8B F2           mov         rsi,rdx
  0000000067074745: 48 8B D9           mov         rbx,rcx
  0000000067074748: 8B D7              mov         edx,edi
  000000006707474A: E8 71 E2 FF FF     call        00000000670729C0
  000000006707474F: 48 85 F6           test        rsi,rsi
  0000000067074752: 74 19              je          000000006707476D
  0000000067074754: 85 FF              test        edi,edi
  0000000067074756: 74 15              je          000000006707476D
  0000000067074758: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000006707475B: 4C 8B C7           mov         r8,rdi
  000000006707475E: 48 8B D6           mov         rdx,rsi
  0000000067074761: 48 8B 49 10        mov         rcx,qword ptr [rcx+10h]
  0000000067074765: 4D 03 C0           add         r8,r8
  0000000067074768: E8 31 43 10 00     call        0000000067178A9E
  000000006707476D: 48 8B 74 24 38     mov         rsi,qword ptr [rsp+38h]
  0000000067074772: 48 8B C3           mov         rax,rbx
  0000000067074775: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  000000006707477A: 48 83 C4 20        add         rsp,20h
  000000006707477E: 5F                 pop         rdi
  000000006707477F: C3                 ret

  Summary

      1A7000 .text
