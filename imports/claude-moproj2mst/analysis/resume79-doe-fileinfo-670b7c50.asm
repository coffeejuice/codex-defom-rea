Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670B7C50: 40 53              push        rbx
  00000000670B7C52: 48 83 EC 20        sub         rsp,20h
  00000000670B7C56: 48 8B 09           mov         rcx,qword ptr [rcx]
  00000000670B7C59: 48 8B DA           mov         rbx,rdx
  00000000670B7C5C: C7 44 24 30 00 00  mov         dword ptr [rsp+30h],0
                    00 00
  00000000670B7C64: F6 81 B4 00 00 00  test        byte ptr [rcx+00000000000000B4h],1
                    01
  00000000670B7C6B: 74 1B              je          00000000670B7C88
  00000000670B7C6D: 48 8D 0D CC BF 13  lea         rcx,[00000000671F3C40h]
                    00
  00000000670B7C74: 83 CA FF           or          edx,0FFFFFFFFh
  00000000670B7C77: E8 C4 42 FB FF     call        000000006706BF40
  00000000670B7C7C: 48 89 03           mov         qword ptr [rbx],rax
  00000000670B7C7F: 48 8B C3           mov         rax,rbx
  00000000670B7C82: 48 83 C4 20        add         rsp,20h
  00000000670B7C86: 5B                 pop         rbx
  00000000670B7C87: C3                 ret
  00000000670B7C88: 48 83 C1 08        add         rcx,8
  00000000670B7C8C: E8 FF 79 03 00     call        00000000670EF690
  00000000670B7C91: 48 8B C3           mov         rax,rbx
  00000000670B7C94: 48 83 C4 20        add         rsp,20h
  00000000670B7C98: 5B                 pop         rbx
  00000000670B7C99: C3                 ret

  Summary

      1A7000 .text
