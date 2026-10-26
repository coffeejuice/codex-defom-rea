Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006712AE10: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  000000006712AE15: 48 89 6C 24 10     mov         qword ptr [rsp+10h],rbp
  000000006712AE1A: 48 89 74 24 18     mov         qword ptr [rsp+18h],rsi
  000000006712AE1F: 57                 push        rdi
  000000006712AE20: 48 83 EC 20        sub         rsp,20h
  000000006712AE24: 48 8B 59 08        mov         rbx,qword ptr [rcx+8]
  000000006712AE28: 48 8B EA           mov         rbp,rdx
  000000006712AE2B: 48 8B F1           mov         rsi,rcx
  000000006712AE2E: 48 83 7B 78 00     cmp         qword ptr [rbx+78h],0
  000000006712AE33: 74 12              je          000000006712AE47
  000000006712AE35: 48 8D 4B 30        lea         rcx,[rbx+30h]
  000000006712AE39: E8 92 09 F4 FF     call        000000006706B7D0
  000000006712AE3E: 84 C0              test        al,al
  000000006712AE40: 75 05              jne         000000006712AE47
  000000006712AE42: 40 B7 01           mov         dil,1
  000000006712AE45: EB 03              jmp         000000006712AE4A
  000000006712AE47: 40 32 FF           xor         dil,dil
  000000006712AE4A: 48 8D 4B 30        lea         rcx,[rbx+30h]
  000000006712AE4E: 48 8B D5           mov         rdx,rbp
  000000006712AE51: E8 7A 55 F4 FF     call        00000000670703D0
  000000006712AE56: 40 84 FF           test        dil,dil
  000000006712AE59: 74 0D              je          000000006712AE68
  000000006712AE5B: 48 8B 4B 78        mov         rcx,qword ptr [rbx+78h]
  000000006712AE5F: 48 8B D6           mov         rdx,rsi
  000000006712AE62: FF 15 F8 94 1C 00  call        qword ptr [00000000672F4360h]
  000000006712AE68: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  000000006712AE6D: 48 8B 6C 24 38     mov         rbp,qword ptr [rsp+38h]
  000000006712AE72: 48 8B 74 24 40     mov         rsi,qword ptr [rsp+40h]
  000000006712AE77: 48 83 C4 20        add         rsp,20h
  000000006712AE7B: 5F                 pop         rdi
  000000006712AE7C: C3                 ret

  Summary

      1A7000 .text
