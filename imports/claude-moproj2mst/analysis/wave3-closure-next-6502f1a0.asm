Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006502F1A0: 40 53              push        rbx
  000000006502F1A2: 48 83 EC 20        sub         rsp,20h
  000000006502F1A6: 48 8B D9           mov         rbx,rcx
  000000006502F1A9: 48 8B 09           mov         rcx,qword ptr [rcx]
  000000006502F1AC: 48 85 C9           test        rcx,rcx
  000000006502F1AF: 74 22              je          000000006502F1D3
  000000006502F1B1: FF 15 C1 77 66 00  call        qword ptr [0000000065696978h]
  000000006502F1B7: 84 C0              test        al,al
  000000006502F1B9: 75 18              jne         000000006502F1D3
  000000006502F1BB: 48 8B 1B           mov         rbx,qword ptr [rbx]
  000000006502F1BE: 48 85 DB           test        rbx,rbx
  000000006502F1C1: 74 10              je          000000006502F1D3
  000000006502F1C3: 48 8B CB           mov         rcx,rbx
  000000006502F1C6: E8 A5 C1 05 00     call        000000006508B370
  000000006502F1CB: 48 8B CB           mov         rcx,rbx
  000000006502F1CE: E8 DB 1E 5B 00     call        00000000655E10AE
  000000006502F1D3: 48 83 C4 20        add         rsp,20h
  000000006502F1D7: 5B                 pop         rbx
  000000006502F1D8: C3                 ret

  Summary

      695000 .text
