Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065372B22: 48 89 7C 24 30     mov         qword ptr [rsp+30h],rdi
  0000000065372B27: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  0000000065372B2B: 48 81 C1 90 00 00  add         rcx,90h
                    00
  0000000065372B32: 48 8B 39           mov         rdi,qword ptr [rcx]
  0000000065372B35: 48 3B FE           cmp         rdi,rsi
  0000000065372B38: 74 09              je          0000000065372B43
  0000000065372B3A: 48 8B D6           mov         rdx,rsi
  0000000065372B3D: FF 15 A5 3D 32 00  call        qword ptr [00000000656968E8h]
  0000000065372B43: 48 8B 83 08 02 00  mov         rax,qword ptr [rbx+0000000000000208h]
                    00
  0000000065372B4A: 48 8B 50 08        mov         rdx,qword ptr [rax+8]
  0000000065372B4E: 80 BA 98 00 00 00  cmp         byte ptr [rdx+0000000000000098h],0
                    00
  0000000065372B55: 74 16              je          0000000065372B6D
  0000000065372B57: 48 85 FF           test        rdi,rdi
  0000000065372B5A: 74 11              je          0000000065372B6D
  0000000065372B5C: 48 3B FE           cmp         rdi,rsi
  0000000065372B5F: 74 0C              je          0000000065372B6D
  0000000065372B61: 48 8B 07           mov         rax,qword ptr [rdi]
  0000000065372B64: 48 8B CF           mov         rcx,rdi
  0000000065372B67: FF 90 A0 01 00 00  call        qword ptr [rax+00000000000001A0h]
  0000000065372B6D: 48 8B 7C 24 30     mov         rdi,qword ptr [rsp+30h]

  Summary

      695000 .text
