Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006539ADBB: B8 01 00 00 00     mov         eax,1
  000000006539ADC0: C3                 ret
  000000006539ADC1: B8 06 00 00 00     mov         eax,6
  000000006539ADC6: C3                 ret
  000000006539ADC7: B8 05 00 00 00     mov         eax,5
  000000006539ADCC: C3                 ret
  000000006539ADCD: 81 F9 00 00 00 01  cmp         ecx,1000000h
  000000006539ADD3: 7F 2E              jg          000000006539AE03
  000000006539ADD5: 74 26              je          000000006539ADFD
  000000006539ADD7: 81 F9 00 00 10 00  cmp         ecx,100000h
  000000006539ADDD: 74 B9              je          000000006539AD98
  000000006539ADDF: 81 F9 00 00 20 00  cmp         ecx,200000h
  000000006539ADE5: 74 D4              je          000000006539ADBB
  000000006539ADE7: 81 F9 00 00 40 00  cmp         ecx,400000h
  000000006539ADED: 74 CC              je          000000006539ADBB
  000000006539ADEF: 81 F9 00 00 80 00  cmp         ecx,800000h
  000000006539ADF5: 75 24              jne         000000006539AE1B
  000000006539ADF7: B8 02 00 00 00     mov         eax,2
  000000006539ADFC: C3                 ret
  000000006539ADFD: B8 04 00 00 00     mov         eax,4
  000000006539AE02: C3                 ret
  000000006539AE03: 81 F9 00 00 00 02  cmp         ecx,2000000h
  000000006539AE09: 74 1A              je          000000006539AE25
  000000006539AE0B: 81 F9 00 00 00 04  cmp         ecx,4000000h
  000000006539AE11: 74 0C              je          000000006539AE1F
  000000006539AE13: 81 F9 00 00 00 08  cmp         ecx,8000000h
  000000006539AE19: 74 04              je          000000006539AE1F
  000000006539AE1B: 83 C8 FF           or          eax,0FFFFFFFFh
  000000006539AE1E: C3                 ret
  000000006539AE1F: B8 07 00 00 00     mov         eax,7
  000000006539AE24: C3                 ret
  000000006539AE25: B8 08 00 00 00     mov         eax,8
  000000006539AE2A: C3                 ret
  000000006539AE2B: CC                 int         3
  000000006539AE2C: CC                 int         3
  000000006539AE2D: CC                 int         3
  000000006539AE2E: CC                 int         3
  000000006539AE2F: CC                 int         3

  Summary

      695000 .text
