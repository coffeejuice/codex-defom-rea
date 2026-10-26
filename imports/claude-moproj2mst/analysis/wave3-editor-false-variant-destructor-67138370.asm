Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067138370: 48 83 EC 28        sub         rsp,28h
  0000000067138374: 0F BA 61 08 1E     bt          dword ptr [rcx+8],1Eh
  0000000067138379: 73 0A              jae         0000000067138385
  000000006713837B: 48 8B 11           mov         rdx,qword ptr [rcx]
  000000006713837E: F0 83 42 08 FF     lock add    dword ptr [rdx+8],0FFFFFFFFh
  0000000067138383: 74 18              je          000000006713839D
  0000000067138385: 8B 41 08           mov         eax,dword ptr [rcx+8]
  0000000067138388: 0F BA E0 1E        bt          eax,1Eh
  000000006713838C: 72 19              jb          00000000671383A7
  000000006713838E: 25 FF FF FF 3F     and         eax,3FFFFFFFh
  0000000067138393: 83 F8 07           cmp         eax,7
  0000000067138396: 76 0F              jbe         00000000671383A7
  0000000067138398: 83 F8 7F           cmp         eax,7Fh
  000000006713839B: 73 0A              jae         00000000671383A7
  000000006713839D: 48 8B 05 A4 9C 1B  mov         rax,qword ptr [00000000672F2048h]
                    00
  00000000671383A4: FF 50 08           call        qword ptr [rax+8]
  00000000671383A7: 48 83 C4 28        add         rsp,28h
  00000000671383AB: C3                 ret

  Summary

      1A7000 .text
