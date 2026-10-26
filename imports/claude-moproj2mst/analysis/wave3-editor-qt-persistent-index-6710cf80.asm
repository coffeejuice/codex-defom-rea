Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006710CF80: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006710CF83: 48 85 C0           test        rax,rax
  000000006710CF86: 74 15              je          000000006710CF9D
  000000006710CF88: 83 38 00           cmp         dword ptr [rax],0
  000000006710CF8B: 7C 10              jl          000000006710CF9D
  000000006710CF8D: 83 78 04 00        cmp         dword ptr [rax+4],0
  000000006710CF91: 7C 0A              jl          000000006710CF9D
  000000006710CF93: 48 83 78 10 00     cmp         qword ptr [rax+10h],0
  000000006710CF98: 74 03              je          000000006710CF9D
  000000006710CF9A: B0 01              mov         al,1
  000000006710CF9C: C3                 ret
  000000006710CF9D: 32 C0              xor         al,al
  000000006710CF9F: C3                 ret
  000000006710CFA0: 48 83 EC 28        sub         rsp,28h
  000000006710CFA4: E8 27 B4 02 00     call        00000000671383D0
  000000006710CFA9: FF C8              dec         eax
  000000006710CFAB: 3D 86 00 00 00     cmp         eax,86h
  000000006710CFB0: 77 2E              ja          000000006710CFE0
  000000006710CFB2: 48 8D 15 47 30 EF  lea         rdx,[0000000067000000h]
                    FF
  000000006710CFB9: 48 98              cdqe
  000000006710CFBB: 0F
  000000006710CFBC: B6 84              mov         dh,84h
  000000006710CFBE: 02 F8              add         bh,al

  Summary

      1A7000 .text
