Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001417624C0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001417624C5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001417624CA: 48 83 EC 38        sub         rsp,38h
  00000001417624CE: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001417624D3: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  00000001417624D8: 48 8B 00           mov         rax,qword ptr [rax]
  00000001417624DB: 48 39 01           cmp         qword ptr [rcx],rax
  00000001417624DE: 74 72              je          0000000141762552
  00000001417624E0: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  00000001417624E5: 48 8B 00           mov         rax,qword ptr [rax]
  00000001417624E8: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001417624ED: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001417624F2: 48 83 C1 68        add         rcx,68h
  00000001417624F6: FF 15 FC 1D 6A 00  call        qword ptr [0000000141E042F8h]
  00000001417624FC: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141762501: 48 8B 09           mov         rcx,qword ptr [rcx]
  0000000141762504: 48 83 C1 68        add         rcx,68h
  0000000141762508: FF 15 DA 1D 6A 00  call        qword ptr [0000000141E042E8h]
  000000014176250E: 0F B6 C0           movzx       eax,al
  0000000141762511: 85 C0              test        eax,eax
  0000000141762513: 75 12              jne         0000000141762527
  0000000141762515: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  000000014176251A: 48 8B 12           mov         rdx,qword ptr [rdx]
  000000014176251D: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141762522: E8 E9 FD FF FF     call        0000000141762310
  0000000141762527: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014176252C: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000141762531: 48 89 01           mov         qword ptr [rcx],rax
  0000000141762534: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000141762539: 48 8B 00           mov         rax,qword ptr [rax]
  000000014176253C: 8B 40 78           mov         eax,dword ptr [rax+78h]
  000000014176253F: D1 E8              shr         eax,1
  0000000141762541: 83 E0 01           and         eax,1
  0000000141762544: 85 C0              test        eax,eax
  0000000141762546: 75 0A              jne         0000000141762552
  0000000141762548: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014176254D: E8 6E FC FF FF     call        00000001417621C0
  0000000141762552: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000141762557: 48 83 C4 38        add         rsp,38h
  000000014176255B: C3                 ret

  Summary

     1E02000 .text
