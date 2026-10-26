Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006539AD60: 81 F9 00 00 08 00  cmp         ecx,80000h
  000000006539AD66: 7F 65              jg          000000006539ADCD
  000000006539AD68: 74 2E              je          000000006539AD98
  000000006539AD6A: 81 F9 00 40 00 00  cmp         ecx,4000h
  000000006539AD70: 7F 29              jg          000000006539AD9B
  000000006539AD72: 74 53              je          000000006539ADC7
  000000006539AD74: 81 F9 00 04 00 00  cmp         ecx,400h
  000000006539AD7A: 74 1C              je          000000006539AD98
  000000006539AD7C: 81 F9 00 08 00 00  cmp         ecx,800h
  000000006539AD82: 74 14              je          000000006539AD98
  000000006539AD84: 81 F9 00 10 00 00  cmp         ecx,1000h
  000000006539AD8A: 74 0C              je          000000006539AD98
  000000006539AD8C: 81 F9 00 20 00 00  cmp         ecx,2000h
  000000006539AD92: 0F 85 83 00 00 00  jne         000000006539AE1B
  000000006539AD98: 33 C0              xor         eax,eax
  000000006539AD9A: C3                 ret
  000000006539AD9B: 81 F9 00 80 00 00  cmp         ecx,8000h
  000000006539ADA1: 74 24              je          000000006539ADC7
  000000006539ADA3: 81 F9 00 00 01 00  cmp         ecx,10000h
  000000006539ADA9: 74 16              je          000000006539ADC1
  000000006539ADAB: 81 F9 00 00 02 00  cmp         ecx,20000h
  000000006539ADB1: 74 0E              je          000000006539ADC1
  000000006539ADB3: 81 F9 00 00 04 00  cmp         ecx,40000h
  000000006539ADB9: 75 60              jne         000000006539AE1B
  000000006539ADBB: B8 01 00 00 00     mov         eax,1

  Summary

      695000 .text
