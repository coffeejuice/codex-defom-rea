Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065473AD0: 85 C9              test        ecx,ecx
  0000000065473AD2: 74 60              je          0000000065473B34
  0000000065473AD4: F7 C1 00 FC FF FF  test        ecx,0FFFFFC00h
  0000000065473ADA: 75 58              jne         0000000065473B34
  0000000065473ADC: 0F B6 C1           movzx       eax,cl
  0000000065473ADF: FF C8              dec         eax
  0000000065473AE1: 83 F8 08           cmp         eax,8
  0000000065473AE4: 77 4B              ja          0000000065473B31
  0000000065473AE6: 48 8D 15 13 C5 B8  lea         rdx,[0000000065000000h]
                    FF
  0000000065473AED: 48 98              cdqe
  0000000065473AEF: 8B 8C 82 40 3B 47  mov         ecx,dword ptr [rdx+rax*4+0000000000473B40h]
                    00
  0000000065473AF6: 48 03 CA           add         rcx,rdx
  0000000065473AF9: FF E1              jmp         rcx
  0000000065473AFB: B8 00 04 00 00     mov         eax,400h
  0000000065473B00: C3                 ret
  0000000065473B01: B8 00 00 40 00     mov         eax,400000h
  0000000065473B06: C3                 ret
  0000000065473B07: B8 00 40 00 00     mov         eax,4000h
  0000000065473B0C: C3                 ret
  0000000065473B0D: B8
  0000000065473B0E: 00 00              add         byte ptr [rax],al

  Summary

      695000 .text
