Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006706C280: 40 53              push        rbx
  000000006706C282: 48 83 EC 20        sub         rsp,20h
  000000006706C286: 48 8B DA           mov         rbx,rdx
  000000006706C289: E8 92 FE FF FF     call        000000006706C120
  000000006706C28E: 48 3D 00 00 00 80  cmp         rax,0FFFFFFFF80000000h
  000000006706C294: 7C 08              jl          000000006706C29E
  000000006706C296: 48 3D FF FF FF 7F  cmp         rax,7FFFFFFFh
  000000006706C29C: 7E 0A              jle         000000006706C2A8
  000000006706C29E: 48 85 DB           test        rbx,rbx
  000000006706C2A1: 74 03              je          000000006706C2A6
  000000006706C2A3: C6 03 00           mov         byte ptr [rbx],0
  000000006706C2A6: 33 C0              xor         eax,eax
  000000006706C2A8: 48 83 C4 20        add         rsp,20h
  000000006706C2AC: 5B                 pop         rbx
  000000006706C2AD: C3                 ret

  Summary

      1A7000 .text
