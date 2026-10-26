Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065065FF7: 8B 83 D8 01 00 00  mov         eax,dword ptr [rbx+00000000000001D8h]
  0000000065065FFD: 0F BA E0 11        bt          eax,11h
  0000000065066001: 73 E4              jae         0000000065065FE7
  0000000065066003: 0F BA F0 11        btr         eax,11h
  0000000065066007: 48 8B CB           mov         rcx,rbx
  000000006506600A: 89 83 D8 01 00 00  mov         dword ptr [rbx+00000000000001D8h],eax
  0000000065066010: 48 8B 74 24 30     mov         rsi,qword ptr [rsp+30h]
  0000000065066015: 48 8B 7C 24 38     mov         rdi,qword ptr [rsp+38h]
  000000006506601A: 48 83 C4 20        add         rsp,20h
  000000006506601E: 5B                 pop         rbx
  000000006506601F: E9 1C 19 03 00     jmp         0000000065097940
  0000000065066024: 8B 83 D8 01 00 00  mov         eax,dword ptr [rbx+00000000000001D8h]
  000000006506602A: 0F BA E0 11        bt          eax,11h
  000000006506602E: 72 BC              jb          0000000065065FEC
  0000000065066030: 0F BA E8 11        bts         eax,11h
  0000000065066034: 48 8B CB           mov         rcx,rbx
  0000000065066037: 89 83 D8 01 00 00  mov         dword ptr [rbx+00000000000001D8h],eax
  000000006506603D: 48 8B 7C 24 38     mov         rdi,qword ptr [rsp+38h]
  0000000065066042: 48 83 C4 20        add         rsp,20h
  0000000065066046: 5B                 pop         rbx
  0000000065066047: E9
  0000000065066048: F4                 hlt
  0000000065066049: 18

  Summary

      695000 .text
