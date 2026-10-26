Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670C2030: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000000670C2035: 57                 push        rdi
  00000000670C2036: 48 83 EC 30        sub         rsp,30h
  00000000670C203A: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670C2043: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  00000000670C2048: 48 8B D9           mov         rbx,rcx
  00000000670C204B: 80 79 20 00        cmp         byte ptr [rcx+20h],0
  00000000670C204F: 74 1D              je          00000000670C206E
  00000000670C2051: B2 01              mov         dl,1
  00000000670C2053: 48 8B 09           mov         rcx,qword ptr [rcx]
  00000000670C2056: E8 85 90 06 00     call        000000006712B0E0
  00000000670C205B: 48 8B 0B           mov         rcx,qword ptr [rbx]
  00000000670C205E: 48 85 C9           test        rcx,rcx
  00000000670C2061: 74 0B              je          00000000670C206E
  00000000670C2063: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000670C2066: BA 01 00 00 00     mov         edx,1
  00000000670C206B: FF 50 18           call        qword ptr [rax+18h]
  00000000670C206E: 48 8B BB 80 00 00  mov         rdi,qword ptr [rbx+0000000000000080h]
                    00
  00000000670C2075: 48 85 FF           test        rdi,rdi
  00000000670C2078: 74 11              je          00000000670C208B
  00000000670C207A: 48 8B CF           mov         rcx,rdi
  00000000670C207D: E8 7E 94 08 00     call        000000006714B500
  00000000670C2082: 48 8B CF           mov         rcx,rdi
  00000000670C2085: E8 CC 68 0B 00     call        0000000067178956
  00000000670C208A: 90                 nop
  00000000670C208B: 48 8B 8B 98 00 00  mov         rcx,qword ptr [rbx+0000000000000098h]
                    00
  00000000670C2092: F0 83 01 FF        lock add    dword ptr [rcx],0FFFFFFFFh
  00000000670C2096: 75 0D              jne         00000000670C20A5
  00000000670C2098: 48 8B 8B 98 00 00  mov         rcx,qword ptr [rbx+0000000000000098h]
                    00
  00000000670C209F: E8 2C E0 FA FF     call        00000000670700D0
  00000000670C20A4: 90                 nop
  00000000670C20A5: 48 8B 83 90 00 00  mov         rax,qword ptr [rbx+0000000000000090h]
                    00
  00000000670C20AC: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670C20B0: 75 0D              jne         00000000670C20BF
  00000000670C20B2: 48 8B 8B 90 00 00  mov         rcx,qword ptr [rbx+0000000000000090h]
                    00
  00000000670C20B9: E8 12 E0 FA FF     call        00000000670700D0
  00000000670C20BE: 90                 nop
  00000000670C20BF: 48 8D 4B 60        lea         rcx,[rbx+60h]
  00000000670C20C3: E8 38 94 08 00     call        000000006714B500
  00000000670C20C8: 90                 nop
  00000000670C20C9: 48 8D 4B 40        lea         rcx,[rbx+40h]
  00000000670C20CD: E8 2E 94 08 00     call        000000006714B500
  00000000670C20D2: 90                 nop
  00000000670C20D3: 48 8D 4B 08        lea         rcx,[rbx+8]
  00000000670C20D7: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  00000000670C20DC: 48 83 C4 30        add         rsp,30h
  00000000670C20E0: 5F                 pop         rdi
  00000000670C20E1: E9 8A D7 06 00     jmp         000000006712F870

  Summary

      1A7000 .text
