Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006539AE30: 44 8B 81 34 02 00  mov         r8d,dword ptr [rcx+0000000000000234h]
                    00
  000000006539AE37: 33 C0              xor         eax,eax
  000000006539AE39: 41 83 F8 03        cmp         r8d,3
  000000006539AE3D: 0F 94 C1           sete        cl
  000000006539AE40: 81 FA 00 00 08 00  cmp         edx,80000h
  000000006539AE46: 0F 8F C2 00 00 00  jg          000000006539AF0E
  000000006539AE4C: 0F 84 B4 00 00 00  je          000000006539AF06
  000000006539AE52: 81 FA 00 40 00 00  cmp         edx,4000h
  000000006539AE58: 7F 68              jg          000000006539AEC2
  000000006539AE5A: 74 5E              je          000000006539AEBA
  000000006539AE5C: 81 FA 00 04 00 00  cmp         edx,400h
  000000006539AE62: 74 41              je          000000006539AEA5
  000000006539AE64: 81 FA 00 08 00 00  cmp         edx,800h
  000000006539AE6A: 74 24              je          000000006539AE90
  000000006539AE6C: 81 FA 00 10 00 00  cmp         edx,1000h
  000000006539AE72: 74 14              je          000000006539AE88
  000000006539AE74: 81 FA 00 20 00 00  cmp         edx,2000h
  000000006539AE7A: 0F 85 4E 01 00 00  jne         000000006539AFCE
  000000006539AE80: 48 8D 05 F5 1C 36  lea         rax,[00000000656FCB7Ch]
                    00
  000000006539AE87: C3                 ret
  000000006539AE88: 48 8D 05 E1 1C 36  lea         rax,[00000000656FCB70h]
                    00
  000000006539AE8F: C3                 ret

  Summary

      695000 .text
