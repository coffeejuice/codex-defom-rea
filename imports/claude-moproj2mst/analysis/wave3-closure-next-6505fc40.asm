Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006505FC40: 48 89 5C 24 20     mov         qword ptr [rsp+20h],rbx
  000000006505FC45: 56                 push        rsi
  000000006505FC46: 57                 push        rdi
  000000006505FC47: 41 56              push        r14
  000000006505FC49: 48 83 EC 20        sub         rsp,20h
  000000006505FC4D: 48 8B 71 08        mov         rsi,qword ptr [rcx+8]
  000000006505FC51: 4C 8B F1           mov         r14,rcx
  000000006505FC54: 48 8B FA           mov         rdi,rdx
  000000006505FC57: 48 8B CE           mov         rcx,rsi
  000000006505FC5A: 48 8B DE           mov         rbx,rsi
  000000006505FC5D: FF 15 05 6C 63 00  call        qword ptr [0000000065696868h]
  000000006505FC63: 4C 8B 46 20        mov         r8,qword ptr [rsi+20h]
  000000006505FC67: 41 F6 40 0C 01     test        byte ptr [r8+0Ch],1
  000000006505FC6C: 48 8B C8           mov         rcx,rax
  000000006505FC6F: 75 1B              jne         000000006505FC8C
  000000006505FC71: 48 85 C9           test        rcx,rcx
  000000006505FC74: 74 16              je          000000006505FC8C
  000000006505FC76: 48 8B D9           mov         rbx,rcx
  000000006505FC79: FF 15 E9 6B 63 00  call        qword ptr [0000000065696868h]
  000000006505FC7F: 48 8B C8           mov         rcx,rax
  000000006505FC82: 48 8B 43 20        mov         rax,qword ptr [rbx+20h]
  000000006505FC86: F6 40 0C 01        test        byte ptr [rax+0Ch],1
  000000006505FC8A: 74 E5              je          000000006505FC71
  000000006505FC8C: 48 3B FB           cmp         rdi,rbx
  000000006505FC8F: 0F 84 83 01 00 00  je          000000006505FE18
  000000006505FC95: 49 8B 8E A0 00 00  mov         rcx,qword ptr [r14+00000000000000A0h]
                    00

  Summary

      695000 .text
